#pragma once

#include <vector>
#include <string>
#include <string_view>

#include <magic_enum.hpp>

#include "util/Binary.h"
#include "util/Foreach.h"

#include "Operand.h"
#include "InstructionType.h"

namespace GBE
{
    class Memory;
    class Assembly
    {
    public:
        Assembly() = default;
        ~Assembly() = default;

        void SetAddress(uint16_t address);
        void SetOpcode(uint8_t opcode);
        void SetOperation(InstructionType op);

        void AddOperand(Operand op);
        void AddImm8(uint8_t imm8, bool isAddress = false);
        void AddImm16(uint16_t imm16, bool isAddress = false);

        // get string
        std::string ToString() const;

    private:
        uint16_t m_Address = 0x0;
        uint8_t m_Opcode = 0x0;
        uint16_t m_Size = 1;
        std::string m_Operation = "";
        std::vector<std::string> m_Operands = {};

        void _AddOperand(std::string_view operand);
        void _AddR8(OperandR8 r8);
        void _AddR16(OperandR16 r16);
        void _AddR16Mem(OperandR16Mem r16mem);
        void _AddR16Stk(OperandR16Stk r16stk);
    };
} // namespace GBE
