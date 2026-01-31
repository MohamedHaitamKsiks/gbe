#pragma once

#include <cstdint>

namespace GBE
{
    class InstructionDecoder;
    class Memory;
    class Assembly;

    class Disassembler
    {
    public:
        static Assembly Disassemble(
            uint16_t instAddr, 
            const Memory& memory, 
            const InstructionDecoder& decoder
        );
    private:

    };
} // namespace GBE
