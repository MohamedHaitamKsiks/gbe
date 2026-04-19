#include "GuiDisassembler.h"

#include "imgui.h"

#include "gameboy/Gameboy.h"

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
        SetVisible(true);
    }

    GuiDisassembler::~GuiDisassembler()
    {
    }

    /*void GuiDisassembler::_Disassemble()
    {
        m_AsmInstrutions.clear();

        Cpu &cpu = m_Gameboy->GetCpu();
        const Memory& memory = m_Gameboy->GetMemory();
        const InstructionDecoder &decoder = cpu.GetDecoder();

        uint16_t pc = static_cast<uint16_t>(m_StartPC);

        for (size_t i = 0; i < 100; i++)
        {
            Assembly assembly{};
            Disassembler::Disassemble(
                pc,
                memory,
                decoder,
                assembly
            );

            m_AsmInstrutions[pc] = assembly;
            const auto& nextAddr = assembly.GetNextAddresses();
            if (nextAddr.size() == 0)
                break;

            pc = nextAddr.at(0);
        }
    }*/

   /*
    void GuiDisassembler::_RenderAssembly(const Assembly &assembly)
    {
        Cpu &cpu = m_Gameboy->GetCpu();
        uint16_t pc = cpu.GetRegisters().GetReg16(Reg16::PC);

        static constexpr ImVec4 defaultColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        static constexpr ImVec4 selectColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f);

        const ImVec4& currentColor = (pc == assembly.GetAddress()) ? selectColor : defaultColor;

        ImGui::TextColored(currentColor, "%s : %s", 
            Binary::ToHex(assembly.GetAddress()).c_str(),
            assembly.ToString().c_str()
        );
        
    }
    */

    void GuiDisassembler::_RenderWindow()
    {
        ImGui::Text("0x");
        ImGui::SameLine();
        ImGui::InputInt("Start PC", &m_StartPC, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

        if (ImGui::Button("Disassemble!"))
        {
            // _Disassemble();
        }

        ImGui::Separator();
        ImGui::BeginChild("Instructions: ");
        // for (const auto&[pc, assembly] : m_AsmInstrutions)
        // {
        //     _RenderAssembly(assembly);
        // }
        ImGui::EndChild();
    }

} // namespace GBE
