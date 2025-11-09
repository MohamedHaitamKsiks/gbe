#pragma once

#include "io/graphics/Ppu.h"
#include "io/interrupts/InterruptManager.h"
#include "io/joypad/Joypad.h"

#include "cpu/Cpu.h"
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

        void Start(std::shared_ptr<Cartridge> cartridge);

        void Tick();

        void Stop();

        inline const std::shared_ptr<Ppu>& GetPpu() const
        {
            return m_Ppu;
        }

        inline const std::shared_ptr<InterruptManager>& GetInterruptManager() const
        {
            return m_InterruptManager;
        }

        inline const std::shared_ptr<Joypad>& GetJoypad() const
        {
            return m_Joypad;
        };
    private:
        Cpu m_Cpu{};
        Memory m_Memory{};

        std::shared_ptr<Ppu> m_Ppu = nullptr;
        std::shared_ptr<Cartridge> m_Cartridge = nullptr;
        std::shared_ptr<InterruptManager> m_InterruptManager = nullptr;
        std::shared_ptr<Ram> m_WorkRam = nullptr;
        std::shared_ptr<Ram> m_HighRam = nullptr;
        std::shared_ptr<Joypad> m_Joypad = nullptr;

        void _InitMemoryMapping();
        void _CpuTick();
    };
} // namespace GBE
