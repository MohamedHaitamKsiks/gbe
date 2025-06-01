#pragma once

namespace GBE
{
    enum class InterruptFlag
    {
        NONE = -1,
        
        V_BLANK = 0,
        LCD = 1,
        TIMER = 2,
        SERIAL = 3,
        JOYPAD = 4
    };
} // namespace GBE
