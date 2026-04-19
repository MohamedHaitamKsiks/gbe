#pragma once

#include <memory>

#include "cpu/Cpu.h"
#include "cpu/disassembler/Disassembler.h"
#include "io/graphics/Ppu.h"
#include "util/Class.h"

namespace GBE
{
    class Cartridge;
    
    class Vram;
    class ObjectAttributesMemory;
    class LcdControl;
    class LcdPalettesMemory;
    
    class InterruptManager;
    class Ram;
    class Timer;
    class InstructionDecoder;
    class Joypad;

    class Gameboy
    {
    public:
        GBE_CLASS_NO_COPY_NO_MOVE(Gameboy)
        
        Gameboy();
        ~Gameboy();

        void Start(std::shared_ptr<Cartridge> cartridge);
        uint16_t Tick();
        void Stop();

        inline bool IsRunning() const noexcept
        {
            return m_IsRunning;
        }

        inline Ppu& GetPpu() noexcept
        {
            return *m_Ppu;
        }

        inline InterruptManager& GetInterruptManager() noexcept
        {
            return *m_InterruptManager;
        }

        inline Joypad& GetJoypad() noexcept
        {
            return *m_Joypad;
        };

        inline Disassembler& GetDisassembler() noexcept
        {
            return *m_Disassembler;
        }

        inline Cpu& GetCpu() noexcept
        {
            return *m_Cpu;
        }

        // get memory interface to read/write memory
        inline Memory& GetMemory() noexcept
        {
            return *m_Memory;
        }

    private:
        bool m_IsRunning = false;

        std::unique_ptr<Cpu> m_Cpu = nullptr;
        std::unique_ptr<Disassembler> m_Disassembler = nullptr;
        std::shared_ptr<InstructionDecoder> m_Decoder = nullptr;

        std::shared_ptr<Memory> m_Memory = nullptr;

        std::unique_ptr<Ppu> m_Ppu = nullptr;
        std::shared_ptr<Vram> m_Vram = nullptr;
        std::shared_ptr<ObjectAttributesMemory> m_Oam = nullptr;
        std::shared_ptr<LcdPalettesMemory> m_Palettes = nullptr;
        std::shared_ptr<LcdControl> m_LcdControl = nullptr;

        std::shared_ptr<Cartridge> m_Cartridge = nullptr;
        std::shared_ptr<Ram> m_HighRam = nullptr;
        std::shared_ptr<Ram> m_WorkRam = nullptr;

        std::shared_ptr<InterruptManager> m_InterruptManager = nullptr;
        std::shared_ptr<Joypad> m_Joypad = nullptr;
        std::shared_ptr<Timer> m_Timer = nullptr;

        void _InitMemoryMapping();
        void _CpuTick();
    };
} // namespace GBE
