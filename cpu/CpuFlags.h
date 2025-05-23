#pragma once
#include <cstdint>

namespace GBE
{
    // cpu flags in the lowe bits of AF
    enum CpuFlag
    {
        Z = 128,
        N = 64,
        H = 32,
        C = 16
    };
} // namespace GBE
