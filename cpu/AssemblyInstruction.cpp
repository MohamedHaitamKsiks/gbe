#include "AssemblyInstruction.h"

#include "util/Binary.h"

#include <sstream>
#include <iomanip>

#include <magic_enum.hpp>

namespace GBE
{
    void AssemblyInstruction::SetAddress(uint16_t address)
    {
        m_Address = address;
    }

    void AssemblyInstruction::SetOpcode(uint8_t opcode)
    {
        m_Opcode = opcode;
    }

    void AssemblyInstruction::SetOperation(std::string_view op)
    {
        m_Operation = op;
    }

    void AssemblyInstruction::AddOperand(std::string_view operand)
    {
        m_Operands.push_back(std::string(operand));
    }

    void AssemblyInstruction::AddR8(OperandR8 r8)
    {
        if (r8 == OperandR8::ADR_HL)
        {
            AddOperand("[HL]");
            return;
        }

        AddOperand(magic_enum::enum_name(r8));
    }

    void AssemblyInstruction::AddR16(OperandR16 r16)
    {
        AddOperand(magic_enum::enum_name(r16));
    }

    void AssemblyInstruction::AddR16Mem(OperandR16Mem r16mem)
    {
        std::stringstream ss;
        ss << "[" << magic_enum::enum_name(r16mem) << "]";

        AddOperand(ss.str());
    }

    void AssemblyInstruction::AddR16Stk(OperandR16Stk r16stk)
    {
        AddOperand(magic_enum::enum_name(r16stk));
    }
    void AssemblyInstruction::AddImm8(uint8_t imm8, bool isAddress)
    {
        std::stringstream ss;
        if (isAddress) 
            ss << "[";

        ss << Binary::ToHex(imm8);

        if (isAddress) 
            ss << "]";

        AddOperand(ss.str());
    }
    void AssemblyInstruction::AddImm16(uint16_t imm16, bool isAddress)
    {
        std::stringstream ss;
        if (isAddress)
            ss << "[";

        ss << Binary::ToHex(imm16);

        if (isAddress)
            ss << "]";

        AddOperand(ss.str());
    }

    std::string AssemblyInstruction::ToString() const
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
