#include "Ram.h"


namespace GBE
{

    Ram::Ram(uint16_t size)
    {
        m_Data.resize(size, 0xFF);
    }

    uint8_t Ram::_GetImp(uint16_t address) const
    {
        return m_Data.at(address);
    }

    void Ram::_SetImp(uint16_t address, uint8_t value)
    {
        m_Data[address] = value;
    }

} // namespace GBE
