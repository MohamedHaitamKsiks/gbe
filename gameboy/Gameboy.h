#pragma once

#include "cpu/Cpu.h"
#include "memory/IMemory.h"

namespace GBE
{
    class Gameboy
    {
    public:
        ~Gameboy();        
    private:
        Cpu cpu{};
    };
} // namespace GBE
