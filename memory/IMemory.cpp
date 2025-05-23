#include "IMemory.h"

namespace GBE
{
    void IMemory::Set16(uint16_t address, uint16_t value)
    {
        Set(address, value & 0xff);
        Set(address + 1, value >> 8);
    }

    uint16_t IMemory::Get16(uint16_t address) const
    {
        uint16_t little = Get(address);
        uint16_t big = Get(address + 1);

        return little + (big << 8);
    }

} // namespace GBE
