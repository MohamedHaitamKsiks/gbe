#pragma once

#include <vector>
#include <string>
#include <string_view>

#include <magic_enum.hpp>

#include "util/Binary.h"
#include "util/Foreach.h"

#include "Operand.h"

#define __GBE_ADD_OPERAND(operand) __asmInst.AddOperand(operand);



#define GBE_ASM(asmInst, op, ...)                            \
    {                                                        \
        auto &__asmInst = asmInst;                           \
        __asmInst.SetOperation(op);                          \
        __VA_OPT__(FOREACH(__GBE_ADD_OPERAND, __VA_ARGS__);) \
    };

// #define GBE_ASM(...)

namespace GBE
{
    class Assembly
    {
    public:
        void SetAddress(uint16_t address);

        void SetOpcode(uint8_t opcode);

        void SetOperation(std::string_view op);

        template <typename T>
        void AddOperand(T value)
        {
            using Type = std::decay_t<T>;
            if constexpr (std::is_enum_v<Type>)
            {
                _AddOperand(magic_enum::enum_name(value));
            }
            else if constexpr (std::is_integral_v<Type>)
            {
                _AddOperand(Binary::ToHex(value));
            }
            else
            {
                _AddOperand(value);
            }
        }

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

        void _AddOperand(std::string_view operand);

    };
} // namespace GBE
