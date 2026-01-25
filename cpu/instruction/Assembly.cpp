#include "Assembly.h"

#include "util/Binary.h"

#include <sstream>
#include <iomanip>

#include <magic_enum.hpp>

namespace GBE
{
    void Assembly::SetAddress(uint16_t address)
    {
        m_Address = address;
    }

    void Assembly::SetOpcode(uint8_t opcode)
    {
        m_Opcode = opcode;
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
    void Assembly::AddImm8(uint8_t imm8, bool isAddress)
    {
        std::stringstream ss;
        if (isAddress) 
            ss << "[";

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

    std::string Assembly::ToString() const
    {
        std::stringstream ss;
        // address
        ss << Binary::ToHex(m_Address) << ": ";

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

        // opcode
        ss << std::setfill(' ') << std::setw(34 - ss.str().size());
        ss << "; (" << Binary::ToHex(m_Opcode) << ")";

        return ss.str();
    }

} // namespace GBE
