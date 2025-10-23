#include "MemoryArea.h"
#include <unordered_map>

namespace GBE
{
    void MemoryArea::SetReadWriteFlags(bool flags)
    {
        SetWriteFlag(flags);
        SetReadFlag(flags);
    }

    void MemoryArea::Set(uint16_t address, uint8_t value)
    {
        if (!m_WriteFlag)
            return;
        
        _SetImp(address, value);
        
    }

    uint8_t MemoryArea::Get(uint16_t address) const
    {
        if (!m_ReadFlag)
            return 0xFF;

        return _GetImp(address);
    }
} // namespace GBE
