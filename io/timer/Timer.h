#pragma once

#include "memory/MemoryMap.h"
#include "memory/MemoryArea.h"

#include "io/interrupts/InterruptManager.h"

#include <memory>
#include <cstdint>
#include <array>

namespace GBE
{
    constexpr MemoryMap MMAP_TIMER = MemoryMap{0xFF04, 0xFF07};

    enum class TimerRegister
    {
        DIV = 0x0,  // Divider register
        TIMA = 0x1, // Timer counter
        TMA = 0x2,  // Timer modulo
        TAC = 0x3,  // Timer control
    };

    class Timer: public MemoryArea
    {
    public:
        Timer(const std::shared_ptr<InterruptManager>& interruptManager);
        ~Timer();

        void Init();

        // tick for one m-cycle
        void Tick();

        // 
        void ResetDIV();
    private:
        std::shared_ptr<InterruptManager> m_InterruptManager = nullptr;
        std::array<uint8_t, MMAP_TIMER.GetSize()> m_Data{};

        uint16_t m_DIVClock = 0;
        uint16_t m_Clock = 0;

        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;
    
        uint16_t _GetClockCycles() const;
        bool _IsTimerControlEnabled() const;

        inline uint8_t _GetRegister(TimerRegister reg) const
        {
            return m_Data.at(static_cast<size_t>(reg));
        }

        inline void _SetRegister(TimerRegister reg, uint8_t value)
        {
            m_Data[static_cast<size_t>(reg)] = value;
        }
    };
} // namespace GBE
