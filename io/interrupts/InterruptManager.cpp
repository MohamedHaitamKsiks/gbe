#include "InterruptManager.h"

#include <cassert>
    #include <iostream>

namespace GBE
{
    InterruptManager::InterruptManager()
    {
        SetReadWriteFlags(true);
    }

    InterruptManager::~InterruptManager()
    {
    }


    void InterruptManager::QueueInterrupt(InterruptFlag interrupt)
    {
        m_InterruptFlag |= 1 << static_cast<uint8_t>(interrupt);
    }
    
    void InterruptManager::_SetImp(uint16_t address, uint8_t value)
    {
        if (address == 0)
        {
            m_InterruptFlag = value;
        }
        else
        {
            m_InterruptEnabled = value;
        }
    }
    
    uint8_t InterruptManager::_GetImp(uint16_t address) const
    {
        if (address == 0)
        {
            return m_InterruptFlag ;
        }

        return m_InterruptEnabled;
    }
} // namespace GBE
