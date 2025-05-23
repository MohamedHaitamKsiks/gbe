#include "CpuRegister.h"

namespace GBE
{
    CpuRegister::~CpuRegister()
    {
    }

    void CpuRegister::Set16(uint16_t value)
    {
        m_Low = value & 0xff;
        m_High = value >> 8;
    }

    uint16_t CpuRegister::Get16() const
    {
        uint16_t low = m_Low;
        uint16_t high = m_High << 8;

        return low | high;
    }

} // namespace GBE
