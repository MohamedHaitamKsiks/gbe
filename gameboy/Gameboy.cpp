#include "Gameboy.h"
#include "cpu/InstructionResult.h"

namespace GBE
{
    Gameboy::Gameboy()
    {
        m_InterruptManager = std::make_shared<InterruptManager>();
        m_Ppu = std::make_shared<Ppu>(m_InterruptManager);
        m_WorkRam = std::make_shared<Ram>(MMAP_WRAM.GetSize());
        m_HighRam = std::make_shared<Ram>(MMAP_HRAM.GetSize());
        m_Joypad = std::make_shared<Joypad>(m_InterruptManager);
    }

    Gameboy::~Gameboy()
    {
    }

    void Gameboy::Start(std::shared_ptr<Cartridge> cartridge)
    {
        m_Cartridge = cartridge;
        m_Cartridge->SetReadFlag(true);

        m_WorkRam->SetReadWriteFlags(true);
        m_HighRam->SetReadWriteFlags(true);

        m_Ppu->Init();

        m_Cpu.Init();

        m_InterruptManager->SetInterruptFlag(0xE1);
        m_InterruptManager->SetInterruptEnabled(0x00);

        _InitMemoryMapping();
    }

    void Gameboy::Tick()
    {
        uint32_t dots = 0;
        while (dots < FRAME_DOTS)
        {
            m_Joypad->Tick();

            InstructionResult result{};
            m_Cpu.Run(m_Memory, result);
            
            uint32_t instructionDots = result.Cycles * 4;
            m_Ppu->Tick(instructionDots);
            dots += instructionDots;
        }
    }

    void Gameboy::Stop()
    {
    }

    void Gameboy::_InitMemoryMapping()
    {
        // ROM
        m_Memory.MapMemoryArea(
            std::vector<MemoryMap>{MMAP_ROM_BANK_0, MMAP_ROM_BANK_1_N },
            m_Cartridge
        );
        
        // VRAM
        m_Memory.MapMemoryArea(
            {MMAP_VRAM},
            m_Ppu->GetVram()
        );

        // OAM
        m_Memory.MapMemoryArea(
            {MMAP_OAM},
            m_Ppu->GetOam()
        );

        // LCD Control
        m_Memory.MapMemoryArea(
            MMAP_LCD_CONTROL,
            m_Ppu->GetLcdControl()
        );

        // palettes
        m_Memory.MapMemoryArea(
            {MMAP_LCD_PALETTES},
            m_Ppu->GetLcdPalettes()
        );

        // WRAM
        m_Memory.MapMemoryArea(
            {MMAP_WRAM},
            m_WorkRam
        );

        // HRAM
        m_Memory.MapMemoryArea(
            {MMAP_HRAM},
            m_HighRam
        );

        // interrupts
        m_Memory.MapMemoryArea(
            {MMAP_IF, MMAP_IE},
            m_InterruptManager
        );

        // joypad
        m_Memory.MapMemoryArea(
            {MMAP_P1_JOYP},
            m_Joypad
        );
    }

    void Gameboy::_CpuTick()
    {   
        InstructionResult result{};
        m_Cpu.Run(m_Memory, result);

        // std::cout << result.Asm.ToString() << "\n";
        
        uint32_t dots = result.Cycles * 4;
        
        // tick ppu
        m_Ppu->Tick(dots);

        // manage oam transfer
        const auto& oam = m_Ppu->GetOam();
        const auto& lcdControl = m_Ppu->GetLcdControl();
        lcdControl->Tick(m_Memory, oam, dots);

    }

} // namespace GBE
