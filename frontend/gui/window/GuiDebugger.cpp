#include "GuiDebugger.h"

#include "imgui.h"

#include "gameboy/Gameboy.h"
#include "cpu/Cpu.h"
#include "cpu/debugger/CpuDebugger.h"
#include "util/Binary.h"

namespace GBE
{
    GuiDebugger::GuiDebugger(
        std::shared_ptr<Window> window, 
        std::shared_ptr<Renderer> renderer, 
        std::shared_ptr<Gameboy> gameboy
    ):
        GuiWindow(window, renderer, gameboy)
    {
        SetName("Debugger");
    }

    GuiDebugger::~GuiDebugger()
    {
    }

    void GuiDebugger::_RenderWindow()
    {
        Cpu &cpu = m_Gameboy->GetCpu();
        CpuDebugger &debugger = cpu.GetDebugger();
        _RenderDebuggerActions(debugger);

        ImGui::NewLine();
        ImGui::Separator();

        _RenderListBreakpoints(debugger);
    }

    void GuiDebugger::_RenderDebuggerActions(CpuDebugger &debugger)
    {
        if (!debugger.IsEnabled())
        {
            if (ImGui::Button("Start"))
            {
                debugger.Start();
            }
            return;
        }

        if (ImGui::Button("Step"))
        {
            debugger.Step();
        }
        ImGui::SameLine();

        if (ImGui::Button("Continue"))
        {
            debugger.Continue();
        }
        ImGui::SameLine();

        if (ImGui::Button("Stop"))
        {
            debugger.Stop();
        }
    }

    void GuiDebugger::_RenderListBreakpoints(CpuDebugger &debugger)
    {
        ImGui::Text("Breakpoints:");
        ImGui::NewLine();

        ImGui::Text("0x");
        ImGui::SameLine();
        ImGui::InputInt("##", &m_NewBpAddress, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);
        ImGui::SameLine();
        if (ImGui::Button("Add"))
        {
            debugger.AddBreakPoint(static_cast<uint16_t>(m_NewBpAddress));
        }
        ImGui::NewLine();

        ImGui::BeginChild("Breakpoints");
        for (uint16_t bp : debugger.GetBreakPoints())
        {
            _RenderBreakpoint(debugger, bp);
        }
        ImGui::EndChild();
    }

    void GuiDebugger::_RenderBreakpoint(CpuDebugger &debugger, uint16_t bp)
    {
        ImGui::Text("-- %s", Binary::ToHex(bp).c_str());
        ImGui::SameLine();
        ImGui::PushID(bp);
        if (ImGui::Button("Remove"))
        {
            debugger.RemoveBreakPoint(bp);
        }
        ImGui::PopID();
    }

} // namespace GBE
