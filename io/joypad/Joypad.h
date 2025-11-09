#pragma once

#include "memory/MemoryArea.h"
#include "memory/MemoryMap.h"

#include "io/interrupts/InterruptManager.h"

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

namespace GBE
{
    constexpr MemoryMap MMAP_P1_JOYP = MemoryMap{0xFF00, 0xFF00};

    enum class JoypadButton
    {
        NONE = 0,
        A, 
        B,
        SELECT,
        START,
        RIGHT,
        LEFT,
        UP,
        DOWN
    };

    struct JoypadEvent
    {
        JoypadButton Button = JoypadButton::NONE;
        bool Pressed = false;
    };



    class Joypad: public MemoryArea
    {
    public:
        Joypad(const std::shared_ptr<InterruptManager>& interruptManager);
        ~Joypad();

        void QueueJoypadEvent(JoypadEvent event);
        void Tick();

    private:
        enum class JoypadButtonType
        {
            SELECT_DPAD = 4,
            SELECT_BUTTONS = 5
        };

        enum class JoypadButtonFlag
        {
            A_RIGHT = 0,
            B_LEFT = 1,
            SELECT_UP = 2,
            START_DOWN = 3,
            SIZE = 4
        };

        struct JoypadButtonInfo
        {
            JoypadButtonFlag Flag;
            JoypadButtonType Type;
        };

        uint8_t m_JoypadFlags = 0x0;

        std::vector<JoypadEvent> m_Events{};
        std::map<JoypadButtonType, uint8_t> m_JoypadMatrix{};
        std::map<JoypadButton, JoypadButtonInfo> m_JoypadInfos{};

        std::shared_ptr<InterruptManager> m_InterruptManager = nullptr;


        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;

        JoypadButtonType _GetType() const;

    };
} // namespace GBE
