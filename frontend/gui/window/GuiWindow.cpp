#include "GuiWindow.h"

#include "imgui.h"

namespace GBE
{
    GuiWindow::GuiWindow(
        std::shared_ptr<Window> window, 
        std::shared_ptr<Renderer> renderer, 
        std::shared_ptr<Gameboy> gameboy
    ):
        GuiLayer(window, renderer, gameboy)
    {
        SetVisible(false);
    }

    GuiWindow::~GuiWindow()
    {
    }

    void GuiWindow::_RenderImp()
    {
        if (ImGui::Begin(m_Name.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing))
        {
            _RenderWindow();
        }
        ImGui::End();
    }

} // namespace GBE
