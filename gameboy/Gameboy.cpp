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
        m_Timer = std::make_shared<Timer>(m_InterruptManager);
    }

    Gameboy::~Gameboy()
    {
    }

    static bool s_DoDebug = false;

    void Gameboy::Start(std::shared_ptr<Cartridge> cartridge)
    {
        s_DoDebug = false;
        m_Cartridge = cartridge;
        m_Cartridge->SetReadFlag(true);

        m_WorkRam->SetReadWriteFlags(true);
        m_HighRam->SetReadWriteFlags(true);

        m_Ppu->Init();

        m_Cpu.Init();

        m_InterruptManager->SetInterruptFlag(0xE1);
        m_InterruptManager->SetInterruptEnabled(0x00);

        m_Timer->Init();

        _InitMemoryMapping();
    }


    void Gameboy::Tick()
    {
        uint32_t dots = 0;
        while (dots < FRAME_DOTS)
        {
            m_Joypad->Tick();

            
            uint16_t pc = m_Cpu.GetRegisters().GetReg16(Reg16::PC);
            //if (m_Memory.Get(pc) == 0x27)
            //    std::cout << m_Cpu.GetRegisters().ToString() << "\n";
            
            InstructionResult result{};
            m_Cpu.Run(m_Memory, result);
            if (pc == 49988)
                s_DoDebug = false;

            if ( /* m_Memory.Get(pc) == 0x27 || */ s_DoDebug)
            {
                std::cout << result.Asm.ToString() << "\n";
                std::cout << m_Cpu.GetRegisters().ToString() << "\n";
                std::cout << "";    
            }

            for (uint16_t i = 0; i < result.Cycles; i++)
                m_Timer->Tick();
            
            uint32_t instructionDots = result.Cycles * 4;
            m_Ppu->Tick(instructionDots);
            
            // manage oam transfer
            const auto &oam = m_Ppu->GetOam();
            const auto &lcdControl = m_Ppu->GetLcdControl();
            lcdControl->Tick(m_Memory, oam, instructionDots);

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

        // timer
        m_Memory.MapMemoryArea(
            {MMAP_TIMER},
            m_Timer
        );
    }

    void Gameboy::_CpuTick()
    {


    }

} // namespace GBE
