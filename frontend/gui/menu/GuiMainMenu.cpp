#include "GuiMainMenu.h"

#include "imgui.h"

#include "util/Assert.h"

#include "cartridge/Cartridge.h"
#include "gameboy/Gameboy.h"

#include "frontend/rendering/Window.h"
#include "frontend/gui/window/GuiDebugger.h"
#include "frontend/gui/window/GuiCpuState.h"
#include "frontend/gui/window/GuiDisassembler.h"

namespace GBE
{
    GuiMainMenu::GuiMainMenu(
        std::shared_ptr<Window> window, 
        std::shared_ptr<Renderer> renderer, 
        std::shared_ptr<Gameboy> gameboy
    ):
        GuiLayer(window, renderer, gameboy)
    {
        constexpr std::string_view cpuCategory = "CPU";

        _AddWindow<GuiDebugger>(cpuCategory);
        _AddWindow<GuiCpuState>(cpuCategory);
        _AddWindow<GuiDisassembler>(cpuCategory);
    }

    GuiMainMenu::~GuiMainMenu()
    {
    }

    void GuiMainMenu::_AddWindow(std::string_view category, std::shared_ptr<GuiWindow> window)
    {
        GBE_ASSERT(window);

        _GuiWindowInfo& windowInfo = m_Windows[std::string(category)].emplace_back();
        windowInfo._IsVisible = window->IsVisible();
        windowInfo._Window = window;
    }

    void GuiMainMenu::_LoadRom(std::string_view path)
    {
        m_Gameboy->Stop();

        auto cartridge = std::make_shared<Cartridge>();
        cartridge->Load(path);
        m_Gameboy->Start(cartridge);
    }

    void GuiMainMenu::_RenderImp()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                _RenderFile();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Windows"))
            {
                _RenderWindowsList();
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void GuiMainMenu::_RenderFile()
    {
        if (ImGui::MenuItem("Open ROM"))
        {
            m_Window->OpenFileDialog([this](const std::vector<std::string> &paths)
            {
                if (!paths.empty())
                    _LoadRom(paths[0]); 
            });
        }

        if (ImGui::MenuItem("Exit"))
        {
            m_Window->Close();
        }
    }

    void GuiMainMenu::_RenderWindowsList()
    {
        for (auto &[category, windowInfos] : m_Windows)
        {
            ImGui::Text(category.c_str());

            for (auto& windowInfo: windowInfos)
            {
                ImGui::Checkbox(windowInfo._Window->GetName().c_str(), &windowInfo._IsVisible);
                windowInfo._Window->SetVisible(windowInfo._IsVisible);
            }

            ImGui::Separator();
        }
    }

} // namespace GBE
