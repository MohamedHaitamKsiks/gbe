#pragma once

#include "io/graphics/Ppu.h"
#include "io/interrupts/InterruptManager.h"

#include "cpu/Cpu.h"
#include "memory/IMemory.h"
#include "memory/Memory.h"
#include "memory/Ram.h"
#include "cartridge/Cartridge.h"

#include <memory>

namespace GBE
{
    class Gameboy
    {
    public:
        Gameboy();
        ~Gameboy();     

        void Start(std::shared_ptr<Cartridge>&& cartridge);

        void Tick();

        void Stop();

        inline std::shared_ptr<Ppu> GetPpu() const
        {
            return m_Ppu;
        }

        inline std::shared_ptr<InterruptManager> GetInterruptManager() const
        {
            return m_InterruptManager;
        }
    private:
        Cpu m_Cpu{};
        Memory m_Memory{};

        std::shared_ptr<Ppu> m_Ppu = nullptr;
        std::shared_ptr<Cartridge> m_Cartridge = nullptr;
        std::shared_ptr<InterruptManager> m_InterruptManager = nullptr;
        std::shared_ptr<Ram> m_WorkRam{};

        void _InitMemoryMapping();
        void _CpuTick();
    };
} // namespace GBE
