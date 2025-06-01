#include "InterruptManager.h"

namespace GBE
{
    InterruptManager::~InterruptManager()
    {
    }

    void InterruptManager::QueueInterrupt(InterruptFlag interrupt)
    {
        m_InterruptFlag |= 1 << static_cast<uint8_t>(interrupt);
    }
} // namespace GBE
