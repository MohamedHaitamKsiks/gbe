#pragma once

#include <vector>
#include <string>
#include <string_view>

#include "CpuOperand.h"

namespace GBE
{
    class AssemblyInstruction
    {
    public:
        void SetAddress(uint16_t address);

        void SetOpcode(uint8_t opcode);

        void SetOperation(std::string_view op);

        void AddOperand(std::string_view operand);

        void AddR8(OperandR8 r8);

        void AddR16(OperandR16 r16);

        void AddR16Mem(OperandR16Mem r16mem);

        void AddR16Stk(OperandR16Stk r16stk);

        void AddImm8(uint8_t imm8, bool isAddress = false);

        void AddImm16(uint16_t imm16, bool isAddress = false);

        // get string
        std::string ToString() const;

    private:
        uint16_t m_Address;
        uint8_t m_Opcode;
        std::string m_Operation;
        std::vector<std::string> m_Operands;

    };
} // namespace GBE
