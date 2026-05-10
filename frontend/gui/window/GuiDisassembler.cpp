#include "GuiDisassembler.h"

#include "frontend/gui/GuiUtils.h"
#include "gameboy/Gameboy.h"

#include "imgui.h"

#include <print>

namespace GBE
{
    GuiDisassembler::GuiDisassembler(
        std::shared_ptr<Window> window, 
        std::shared_ptr<Renderer> renderer,
        std::shared_ptr<Gameboy> gameboy
    ):
        GuiWindow(window, renderer, gameboy)
    {
        SetName("Disassembler");

        m_StartPC = 0x100;
        m_MaxSection[0] = 0x0;
        m_MaxSection[1] = 0x7FFF;
    }

    GuiDisassembler::~GuiDisassembler()
    {
    }

    void GuiDisassembler::_Disassemble()
    {
        Disassembler& disassembler = m_Gameboy->GetDisassembler();
        uint16_t startPC = static_cast<uint16_t>(m_StartPC);
        AssemblySection maxSection = {
            .StartAddress   = static_cast<uint16_t>(m_MaxSection[0]), 
            .EndAddress     = static_cast<uint16_t>(m_MaxSection[1])
        };

        disassembler.Disassemble(m_StartPC, maxSection);
    }

   
    void GuiDisassembler::_RenderAssembly(const Assembly &assembly)
    {
        Cpu &cpu = m_Gameboy->GetCpu();
        uint16_t pc = cpu.GetRegisters().GetReg16(Reg16::PC);

        static constexpr ImVec4 defaultColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        static constexpr ImVec4 selectColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f);

        if (pc == assembly.GetAddress())
            ImGui::SetScrollHereY();
            
        const ImVec4& currentColor = (pc == assembly.GetAddress()) ? selectColor : defaultColor;

        ImGui::TextColored(currentColor, "%s : %s", 
            Binary::ToHex(assembly.GetAddress()).c_str(),
            assembly.ToString().c_str()
        );
        
    }
    

    void GuiDisassembler::_RenderWindow()
    {
        Disassembler &disassembler = m_Gameboy->GetDisassembler();
        GuiUtils::InputHex("Start PC", &m_StartPC, 1);
       
        ImGui::Text("Assembly section: ");

        ImGui::PushID("#DisassemblyMaxSection");
        GuiUtils::InputHex("Section", m_MaxSection.begin(), m_MaxSection.size());
        ImGui::PopID();

        if (ImGui::Button("Disassemble!"))
        {
            _Disassemble();
        }

        ImGui::Separator();
        ImGui::BeginChild("Instructions: ");

        for (const auto& [address, section] : disassembler.GetAssemblySections())
        {
            ImGui::Separator();
            ImGui::Text("%s :", Binary::ToHex(address).c_str());

            uint16_t currentAddress = address;
            while (currentAddress <= section.EndAddress)
            {
                const Assembly& assembly = disassembler.GetAssemblyInstruction(currentAddress);
                _RenderAssembly(assembly);

                uint16_t nextAddress = assembly.GetNextInstructionAddress();
                if (nextAddress <= currentAddress)
                    break;

                currentAddress = nextAddress;
            }

            ImGui::Separator();
        }

        ImGui::EndChild();
    }

} // namespace GBE
