#pragma once

#include <cstdint>
#include <array>
#include "Instruction.h"

namespace GBE
{
    class InstructionDecoder
    {
    public:
        InstructionDecoder() = default;
        ~InstructionDecoder() = default;
        
        const Instruction& Decode(uint8_t opcode) const;
        const Instruction& DecodePrefix(uint8_t opcode) const;

    private:
        static void _DecodeBlock0(Instruction &instr);
        static void _DecodeBlock1(Instruction &instr);
        static void _DecodeBlock2(Instruction &instr);
        static void _DecodeBlock3(Instruction &instr);
        static void _DecodePrefixInstruction(Instruction &instr);

        // invalid instruction
        static void _ThrowInvalidInstruction();

        // cache
        mutable std::array<Instruction, UINT8_MAX + 1> m_Instructions{ {} };
        mutable std::array<Instruction, UINT8_MAX + 1> m_PrefixInstructions{ {} };
    };
} // namespace GBE
