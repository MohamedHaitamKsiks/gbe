#include "Gameboy.h"

#include "io/joypad/Joypad.h"
#include "io/timer/Timer.h"

#include "io/graphics/lcd/LcdControl.h"
#include "io/graphics/lcd/LcdPalettesMemory.h"
#include "io/graphics/vram/Vram.h"
#include "io/graphics/oam/ObjectAttributesMemory.h"
#include "io/graphics/Ppu.h"

#include "cartridge/Cartridge.h"

#include "memory/Memory.h"
#include "memory/Ram.h"

#include "cpu/Cpu.h"
#include "cpu/instruction/InstructionDecoder.h"
#include "cpu/instruction/InstructionResult.h"
#include "cpu/disassembler/Disassembler.h"

#include <print>

namespace GBE
{
    Gameboy::Gameboy()
    {
        // memory interface
        m_Memory = std::make_shared<Memory>();
        
        // ram
        m_WorkRam = std::make_shared<Ram>(MMAP_WRAM.GetSize());
        m_HighRam = std::make_shared<Ram>(MMAP_HRAM.GetSize());
        
        // io
        m_InterruptManager = std::make_shared<InterruptManager>();
        m_Joypad = std::make_shared<Joypad>(m_InterruptManager);
        m_Timer = std::make_shared<Timer>(m_InterruptManager);

        // graphics
        m_Vram = std::make_shared<Vram>();
        m_Oam = std::make_shared<ObjectAttributesMemory>();
        m_Palettes = std::make_shared<LcdPalettesMemory>();
        m_LcdControl = std::make_shared<LcdControl>(m_Oam);
        m_Ppu = std::make_unique<Ppu>(m_Vram, m_Oam, m_Palettes, m_LcdControl, m_InterruptManager);

        // cpu
        m_Decoder = std::make_shared<InstructionDecoder>();
        m_Cpu = std::make_unique<Cpu>(m_Memory, m_Decoder);
        m_Disassembler = std::make_unique<Disassembler>(m_Memory, m_Decoder);
    }

    Gameboy::~Gameboy()
    {
    }

    void Gameboy::Start(std::shared_ptr<Cartridge> cartridge)
    {
        if (m_IsRunning)
            return;

        m_IsRunning = true;
        m_Cartridge = cartridge;

        m_Cpu->Init();
        m_Ppu->Init(); 

        _InitMemoryMapping();
        m_Memory->Init();
    }

    uint16_t Gameboy::Tick()
    {
        if (!m_IsRunning)
            return 0;

        CpuDebugger& debugger = m_Cpu->GetDebugger();
        CpuRegistersSet& regs = m_Cpu->GetRegisters();

        uint16_t instructionCycles = 0;
        uint32_t dots = 0;
        while (dots < FRAME_DOTS)
        {
            m_Joypad->Tick();

            uint16_t pc = regs.GetReg16(Reg16::PC);

            InstructionResult result{};
            m_Cpu->Run(result);

            for (uint16_t i = 0; i < result.Cycles; i++)
                m_Timer->Tick();

            uint32_t instructionDots = result.Cycles * 4;
            m_Ppu->Tick(instructionDots);
            
            // manage oam transfer
            m_LcdControl->Tick(*m_Memory, instructionDots);

            dots += instructionDots;
            instructionCycles += result.Cycles;

            if (debugger.IsEnabled() && debugger.IsBreaked())
                break;
        }
        return instructionCycles;
    }

    void Gameboy::Stop()
    {
        m_IsRunning = false;
        // m_Ppu->GetLcdScreen().Clear();
        m_Memory->Reset();
    }

    void Gameboy::_InitMemoryMapping()
    {
        // ROM
        m_Memory->MapMemoryArea(
            std::vector<MemoryMap>{MMAP_ROM_BANK_0, MMAP_ROM_BANK_1_N },
            m_Cartridge
        );
        
        // VRAM
        m_Memory->MapMemoryArea(
            {MMAP_VRAM},
            m_Vram
        );

        // OAM
        m_Memory->MapMemoryArea(
            {MMAP_OAM},
            m_Oam
        );

        // LCD Control
        m_Memory->MapMemoryArea(
            MMAP_LCD_CONTROL,
            m_LcdControl
        );

        // palettes
        m_Memory->MapMemoryArea(
            {MMAP_LCD_PALETTES},
            m_Palettes
        );

        // WRAM
        m_Memory->MapMemoryArea(
            {MMAP_WRAM},
            m_WorkRam
        );

        // HRAM
        m_Memory->MapMemoryArea(
            {MMAP_HRAM},
            m_HighRam
        );

        // interrupts
        m_Memory->MapMemoryArea(
            {MMAP_IF, MMAP_IE},
            m_InterruptManager
        );

        // joypad
        m_Memory->MapMemoryArea(
            {MMAP_P1_JOYP},
            m_Joypad
        );

        // timer
        m_Memory->MapMemoryArea(
            {MMAP_TIMER},
            m_Timer
        );
    }

    void Gameboy::_CpuTick()
    {
    }

} // namespace GBE
