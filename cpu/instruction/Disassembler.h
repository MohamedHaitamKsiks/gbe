#pragma once

#include <cstdint>

namespace GBE
{
    class InstructionDecoder;
    class Memory;
    class Asm;

    namespace Disassembler
    {
        static Asm GetAsm(
            uint16_t instAddr, 
            const Memory& memory, 
            const InstructionDecoder& decoder
        );
    };
} // namespace GBE
