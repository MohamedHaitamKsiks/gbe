
#include <cstdint>
#include <sstream>
#include "AssemblyInstruction.h"

namespace GBE
{
    struct InstructionResult
    {
        uint16_t Cycles = 1;
        AssemblyInstruction Asm{}; // the assembly instruction
    };
} // namespace GBE
