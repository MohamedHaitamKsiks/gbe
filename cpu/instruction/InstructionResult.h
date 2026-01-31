
#include <cstdint>
#include <sstream>
#include "Instruction.h"

namespace GBE
{
    struct InstructionResult
    {
        uint16_t Cycles = 1;
        // Assembly Asm{}; no longer needed
    };
} // namespace GBE
