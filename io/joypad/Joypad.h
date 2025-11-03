#pragma once

#include "memory/MemoryArea.h"
#include "memory/MemoryMap.h"

#include "io/interrupts/InterruptManager.h"

#include <cstdint>
#include <memory>

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

    class Joypad: public MemoryArea
    {
    public:
        Joypad(const std::shared_ptr<InterruptManager>& interruptManager);
        ~Joypad();

        void FireJoypadEvent(JoypadButton btn);

    private:
        uint8_t m_JoypadFlags = 0x0;
        std::shared_ptr<InterruptManager> m_InterruptManager = nullptr;

        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;
    };
} // namespace GBE
