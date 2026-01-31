#include "CpuRegistersSet.h"

#include <sstream>
#include <array>

#include <magic_enum.hpp>

#include "util/Binary.h"

namespace GBE
{
    CpuRegistersSet::CpuRegistersSet()
    {
        m_AF.SetLowMask(0xF0);
    }

    uint8_t CpuRegistersSet::GetReg8(Reg8 r8) const
    {
        switch (r8)
        {
        case Reg8::A:
            return m_AF.GetHigh();
        case Reg8::B:
            return m_BC.GetHigh();
        case Reg8::C:
            return m_BC.GetLow();
        case Reg8::D:
            return m_DE.GetHigh();
        case Reg8::E:
            return m_DE.GetLow();
        case Reg8::H :
            return m_HL.GetHigh();
        case Reg8::L:
            return m_HL.GetLow();
        }

        return 0xff;
    }

    uint16_t CpuRegistersSet::GetReg16(Reg16 r16) const
    {
        switch (r16)
        {
        case Reg16::BC:
            return m_BC.Get16();
        case Reg16::DE:
            return m_DE.Get16();
        case Reg16::HL:
            return m_HL.Get16();
        case Reg16::AF:
            return m_AF.Get16();
        case Reg16::PC:
            return m_PC;
        case Reg16::SP:
            return m_SP;
        }

        return 0xffff;
    }

    void CpuRegistersSet::SetReg8(Reg8 r8, uint8_t value)
    {
        switch (r8)
        {
        case Reg8::A:
            m_AF.SetHigh(value);
            break;
        case Reg8::B:
            m_BC.SetHigh(value);
            break;
        case Reg8::C:
            m_BC.SetLow(value);
            break;
        case Reg8::D:
            m_DE.SetHigh(value);
            break;
        case Reg8::E:
            m_DE.SetLow(value);
            break;
        case Reg8::H:
            m_HL.SetHigh(value);
            break;
        case Reg8::L:
            m_HL.SetLow(value);
            break;
        }
    }

    void CpuRegistersSet::SetReg16(Reg16 r16, uint16_t value)
    {
        switch (r16)
        {
        case Reg16::BC:
            m_BC.Set16(value);
            break;
        case Reg16::DE:
            m_DE.Set16(value);
            break;
        case Reg16::HL:
            m_HL.Set16(value);
            break;
        case Reg16::AF:
            m_AF.Set16(value);
            break;
        case Reg16::PC:
            m_PC = value;
            break;
        case Reg16::SP:
            m_SP = value;
            break;
        }
    }

    uint8_t CpuRegistersSet::GetFlags() const
    {
        return m_AF.GetLow() & 0xF0;
    }

    bool CpuRegistersSet::GetFlag(CpuFlag flag) const
    {
        return (GetFlags() & flag);
    }

    void CpuRegistersSet::SetFlag(CpuFlag flag, bool value)
    {
        uint8_t flagValue = static_cast<uint8_t>(flag);
        uint8_t mask = ~flagValue;
        
        uint8_t oldValue = m_AF.GetLow();
        uint8_t newValue = (oldValue & mask) | ((value)? flagValue : 0);

        m_AF.SetLow(newValue);
    }

    void CpuRegistersSet::SetFlags(uint8_t flags, uint8_t values)
    {
        uint8_t mask = ~flags;

        uint8_t oldValue = m_AF.GetLow();
        uint8_t newValue = (oldValue & mask) | (values);

        m_AF.SetLow(newValue);
    }

    std::string CpuRegistersSet::ToString() const
    {
        std::stringstream ss;
        ss << "==== CPU REGISTERS ====\n";
        ss << m_AF.ToString("AF") << "\n";
        ss << m_BC.ToString("BC") << "\n";
        ss << m_DE.ToString("DE") << "\n";
        ss << m_HL.ToString("HL") << "\n";
        ss << "\n";
        
        ss << "PC: " << Binary::ToHex(m_PC) << "\n";
        ss << "SP: " << Binary::ToHex(m_SP) << "\n";
        ss << "\n";

        ss << _FlagsToString() << "\n";
        ss << "=======================\n";

        return ss.str();
    }

    std::string CpuRegistersSet::_FlagsToString() const
    {
        std::stringstream ss;
        ss << "Flags: ["
           << (GetFlag(CpuFlag::Z) ? "Z" : "-")
           << (GetFlag(CpuFlag::N) ? "N" : "-")
           << (GetFlag(CpuFlag::H) ? "H" : "-")
           << (GetFlag(CpuFlag::C) ? "C" : "-")
           << "]";
        return ss.str();
    }

} // namespace GBE
