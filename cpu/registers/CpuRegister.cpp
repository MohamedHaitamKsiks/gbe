#include "CpuRegister.h"

#include "util/Binary.h"
#include <sstream>

namespace GBE
{
    CpuRegister::~CpuRegister()
    {
    }

    void CpuRegister::Set16(uint16_t value)
    {
        SetLow(value & 0xff);
        SetHigh(value >> 8);
    }

    uint16_t CpuRegister::Get16() const
    {
        uint16_t low = GetLow();
        uint16_t high = GetHigh() << 8;

        return low | high;
    }

    std::string CpuRegister::ToString(std::string_view name, bool showHighLow) const
    {
        // 16 bits
        std::stringstream ss;
        ss << name << ": " << Binary::ToHex(Get16());

        // show hight / low 
        if (showHighLow && name.size() == 2)
        {
            char highName = name[0];
            char lowName = name[1];

            uint8_t highVal = m_High;
            uint8_t lowVal = m_Low;

            ss << "  ("
               << highName << ": " << Binary::ToHex(m_High) << ", "
               << lowName << ": " << Binary::ToHex(m_Low) <<  ")";
        }

        return ss.str();
    }

} // namespace GBE
