#pragma once

#include "memory/MemoryMap.h"
#include "memory/MemoryArea.h"

#include "InterruptFlag.h"
#include <cstdint>

namespace GBE
{

    // Interrupt Enable register (IE)
    static constexpr MemoryMap MMAP_IF(0xFF0F, 0xFF0F);
    static constexpr MemoryMap MMAP_IE(0xFFFF, 0xFFFF);

    class InterruptManager: public MemoryArea
    {
    public:
        InterruptManager();
        ~InterruptManager();
        
        inline uint8_t GetInterruptFlag() const
        {
            return m_InterruptFlag;
        }

        inline void SetInterruptFlag(uint8_t interruptFlag)
        {
            m_InterruptFlag = interruptFlag;
        }

        inline uint8_t GetInterruptEnabled() const
        {
            return m_InterruptEnabled;
        }

        inline void SetInterruptEnabled(uint8_t interruptEnabled)
        {
            m_InterruptEnabled = interruptEnabled;
        }

        // queue interrupt to be executed
        void QueueInterrupt(InterruptFlag interrupt);

        
    private:
        uint8_t m_InterruptFlag = 0;
        uint8_t m_InterruptEnabled = 0;

        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;
    };
} // namespace GBE
