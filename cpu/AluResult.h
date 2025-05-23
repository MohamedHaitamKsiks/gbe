#pragma once

#include <cstdint>

namespace GBE
{
    // result of ALU function
    struct AluResult
    {
        union
        {
            uint8_t Result8;
            uint16_t Result16;
        };

        uint8_t AffectedFlags = 0;
        uint8_t Flags = 0;
    };

} // namespace GBE
