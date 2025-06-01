#pragma once

#include "InterruptFlag.h"
#include <cstdint>

namespace GBE
{
    class InterruptManager
    {
    public:
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
    };
} // namespace GBE
