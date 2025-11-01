#include "Gameboy.h"
#include "cpu/InstructionResult.h"

namespace GBE
{
    Gameboy::Gameboy()
    {
        m_InterruptManager = std::make_shared<InterruptManager>();
        m_Ppu = std::make_shared<Ppu>(m_InterruptManager);
        m_WorkRam = std::make_shared<Ram>(MMAP_WRAM.GetSize());
    }

    Gameboy::~Gameboy()
    {
    }

    void Gameboy::Start(std::shared_ptr<Cartridge>&& cartridge)
    {
        m_Cartridge = cartridge;
        m_Cartridge->SetReadFlag(true);

        m_WorkRam->SetReadWriteFlags(true);

        m_Ppu->Init();

        m_Cpu.Init();

        _InitMemoryMapping();
    }

    void Gameboy::Tick()
    {
        uint32_t dots = 0;
        while (dots < FRAME_DOTS)
        {
            InstructionResult result{};
            m_Cpu.Run(m_Memory, result);

            // std::cout << result.Asm.ToString() << "\n";

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
            {MMAP_ROM_BANK_0, MMAP_ROM_BANK_1_N },
            m_Cartridge
        );

        // VRAM
        m_Memory.MapMemoryArea(
            {MMAP_VRAM, MMAP_ECHO_RAM},
            m_Ppu->GetVram()
        );

        // OAM
        m_Memory.MapMemoryArea(
            {MMAP_OAM},
            m_Ppu->GetOam()
        );

        // LCD Control
        m_Memory.MapMemoryArea(
            {MMAP_LCD_CONTROL},
            m_Ppu->GetLcdControl()
        );

        // WRAM
        m_Memory.MapMemoryArea(
            {MMAP_WRAM},
            m_WorkRam
        );

        // interrupts
        m_Memory.MapMemoryArea(
            {MMAP_IF, MMAP_IE},
            m_InterruptManager
        );

        m_InterruptManager->SetInterruptFlag(0xE1);
        m_InterruptManager->SetInterruptEnabled(0x00);
    }

    void Gameboy::_CpuTick()
    {   
        InstructionResult result{};
        m_Cpu.Run(m_Memory, result);

        // std::cout << result.Asm.ToString() << "\n";

        m_Ppu->Tick(result.Cycles * 4);
    }

} // namespace GBE
