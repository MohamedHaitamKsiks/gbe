#include "GuiCpuState.h"

#include "imgui.h"

#include "gameboy/Gameboy.h"
#include "cpu/Cpu.h"
#include "cartridge/Cartridge.h"


namespace GBE
{
    GuiCpuState::GuiCpuState(
        std::shared_ptr<Window> window, 
        std::shared_ptr<Renderer> renderer, 
        std::shared_ptr<Gameboy> gameboy
    ):
        GuiWindow(window, renderer, gameboy)
    {
        SetName("Cpu State");
    }

    void GuiCpuState::_RenderWindow()
    {
        Cpu &cpu = m_Gameboy->GetCpu();
        Disassembler& disassembler = m_Gameboy->GetDisassembler();

        bool ime = cpu.GetIME();
        bool isHalted = cpu.IsHalted();
        CpuRegistersSet &regs = cpu.GetRegisters();

        ImGui::Text(regs.ToString().c_str());
        ImGui::NewLine();
        ImGui::Separator();

        ImGui::Text("IME: %s\n", ime ? "1" : "0");
        ImGui::Text("Halted: %s\n", isHalted ? "true" : "false");

        Assembly nextInstruction{};
        disassembler.DisassembleInstruction(regs.GetReg16(Reg16::PC), nextInstruction);
        
        ImGui::Text("Next instruction: %s\n", nextInstruction.ToString().c_str());
    }

} // namespace GBE
