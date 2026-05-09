#include "GuiManager.h"

#include <magic_enum.hpp>
#include <SDL3/SDL.h>
#include <print>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "frontend/rendering/Window.h"
#include "frontend/rendering/Renderer.h"

#include "GuiLayer.h"
#include "menu/GuiMainMenu.h"

#include "gameboy/Gameboy.h"
#include "cartridge/Cartridge.h"
#include "util/Assert.h"

namespace GBE
{
    GuiManager::GuiManager(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer, std::shared_ptr<Gameboy> gameboy): 
        m_Window(window), 
        m_Renderer(renderer),
        m_Gameboy(gameboy)
    {
        GBE_ASSERT(m_Window);
        GBE_ASSERT(m_Renderer);
        GBE_ASSERT(m_Gameboy);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        SDL_Window* sdlWindow = window->GetSDLWindow();
        SDL_Renderer* sdlRenderer = renderer->GetSDLRenderer();

        ImGui_ImplSDL3_InitForSDLRenderer(sdlWindow, sdlRenderer);
        ImGui_ImplSDLRenderer3_Init(sdlRenderer);

        m_RootLayer = std::make_shared<GuiLayer>(m_Window, m_Renderer, m_Gameboy);
        auto mainMenu = m_RootLayer->AddLayer<GuiMainMenu>();
    }

    GuiManager::~GuiManager()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiManager::Render(float delta)
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        m_RootLayer->Render();

        ImGui::Render();

        SDL_Renderer *sdlRenderer = m_Renderer->GetSDLRenderer();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdlRenderer);
    }

    bool GuiManager::ProcessEvent(const SDL_Event &event)
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        return ImGui::GetIO().WantCaptureKeyboard;
    }

    void GuiManager::_RenderFPSCounter(float delta)
    {
        ImGui::Begin("GBE");
        float fpsImgui = (delta > 0.0f) ? (1.0f / delta) : 0.0f;
        ImGui::Text("FPS: %.1f", fpsImgui);
        ImGui::End();
    }

} // namespace GBE
