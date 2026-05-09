#include "Assembly.h"

#include "util/Binary.h"

#include <sstream>
#include <iomanip>
#include <print>

#include <magic_enum.hpp>

namespace GBE
{
    uint16_t Assembly::GetNextInstructionAddress() const
    {
        for (const auto &nextAddress : m_NextAddresses)
        {
            if (!nextAddress.IsJump)
                return nextAddress.Address;
        }

        return m_Address;
    }

    void Assembly::SetOperation(InstructionType op)
    {
        m_Operation = magic_enum::enum_name(op);
    }

    void Assembly::AddOperand(Operand op)
    {
        switch (op.GetType())
        {
        case OperandType::R8:
            _AddR8(op.Get<OperandR8>());
            break;
        case OperandType::R16:
            _AddR16(op.Get<OperandR16>());
            break;
        case OperandType::R16_MEM:
            _AddR16Mem(op.Get<OperandR16Mem>());
            break;
        case OperandType::R16_STK:
            _AddR16Stk(op.Get<OperandR16Stk>());
            break;
        case OperandType::BIT3:
            _AddBit3(op.Get<OperandBit3>());
            break;
        case OperandType::COND:
            _AddCond(op.Get<OperandCond>());
            break;
        case OperandType::TGT3:
            _AddTgt3(op.Get<OperandTgt3>());
            break;
        default:
            break;
        }
    }

    void Assembly::_AddOperand(std::string_view operand)
    {
        m_Operands.push_back(std::string(operand));
    }

    void Assembly::_AddR8(OperandR8 r8)
    {
        if (r8 == OperandR8::ADR_HL)
        {
            _AddOperand("[HL]");
            return;
        }

        _AddOperand(magic_enum::enum_name(r8));
    }

    void Assembly::_AddR16(OperandR16 r16)
    {
        _AddOperand(magic_enum::enum_name(r16));
    }

    void Assembly::_AddR16Mem(OperandR16Mem r16mem)
    {
        std::stringstream ss;
        ss << "[" << magic_enum::enum_name(r16mem) << "]";

        _AddOperand(ss.str());
    }

    void Assembly::_AddR16Stk(OperandR16Stk r16stk)
    {
        _AddOperand(magic_enum::enum_name(r16stk));
    }

    void Assembly::_AddCond(OperandCond cc)
    {
        _AddOperand(magic_enum::enum_name(cc));
    }

    void Assembly::_AddBit3(OperandBit3 bit3)
    {
        _AddOperand(Binary::ToHex(bit3.Value));
    }

    void Assembly::_AddTgt3(OperandTgt3 tgt3)
    {
        _AddOperand(Binary::ToHex(tgt3.Value * 8));
    }

    void Assembly::AddImm8(uint8_t imm8, bool isAddress, bool isSigned)
    {
        std::stringstream ss;
        if (isAddress) 
            ss << "[";

        if (isSigned)
            ss << static_cast<int>(static_cast<int8_t>(imm8));
        else
            ss << Binary::ToHex(imm8);

        if (isAddress) 
            ss << "]";

        _AddOperand(ss.str());
    }

    void Assembly::AddImm16(uint16_t imm16, bool isAddress)
    {
        std::stringstream ss;
        if (isAddress)
            ss << "[";

        ss << Binary::ToHex(imm16);

        if (isAddress)
            ss << "]";

        _AddOperand(ss.str());
    }

    const std::string& Assembly::ToString() const
    {   
        if (m_CachedString.size() > 0)
            return m_CachedString;

        std::stringstream ss;

        // operation
        ss << m_Operation;

        // operands
        if (m_Operands.size() > 0)
            ss << " ";

        for (size_t i = 0; i < m_Operands.size(); i++)
        {
            if (i != 0)
                ss << ", ";
            
            ss << m_Operands[i];
        }

        m_CachedString = ss.str();

        return m_CachedString;
    }

} // namespace GBE
