#pragma once

#include <cstdint>

namespace GBE
{
    class InstructionDecoder;
    class Memory;
    class Assembly;
    class Instruction;

    class Disassembler
    {
    public:
        static Assembly Disassemble(
            uint16_t instAddr, 
            const Memory& memory, 
            const InstructionDecoder& decoder
        );
    private:
        static Assembly _CreateInstructioAssembly(
            uint16_t instAddr, 
            const Memory& memory, 
            const Instruction& instr
        );
    };
} // namespace GBE
