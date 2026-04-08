#include "GuiLayer.h"

#include <magic_enum.hpp>
#include <SDL3/SDL.h>
#include <print>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "frontend/rendering/Window.h"
#include "frontend/rendering/Renderer.h"

#include "gameboy/Gameboy.h"
#include "cartridge/Cartridge.h"

namespace GBE
{
    GuiLayer::GuiLayer(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer, std::shared_ptr<Gameboy> gameboy): 
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
    }

    GuiLayer::~GuiLayer()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiLayer::Render(float delta)
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        _RenderMainMenuBar();
        _RenderFPSCounter(delta);
        _RenderCpuState();

        ImGui::Render();

        SDL_Renderer *sdlRenderer = m_Renderer->GetSDLRenderer();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdlRenderer);
    }

    bool GuiLayer::ProcessEvent(const SDL_Event &event)
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        return ImGui::GetIO().WantCaptureKeyboard;
    }

    void GuiLayer::_LoadRom(std::string_view path)
    {
        m_Gameboy->Stop();

        auto cartridge = std::make_shared<Cartridge>();
        cartridge->Load(path);
        m_Gameboy->Start(cartridge);
    }

    void GuiLayer::_RenderMainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open ROM", "Ctrl+O"))
                {
                    m_Window->OpenFileDialog([this](const std::vector<std::string> &paths)
                                             {
                        if (!paths.empty())
                            _LoadRom(paths[0]); });
                }

                if (ImGui::MenuItem("Exit", nullptr, false, false))
                {
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void GuiLayer::_RenderFPSCounter(float delta)
    {
        ImGui::Begin("GBE");
        float fpsImgui = (delta > 0.0f) ? (1.0f / delta) : 0.0f;
        ImGui::Text("FPS: %.1f", fpsImgui);
        ImGui::End();
    }

    void GuiLayer::_RenderCpuState()
    {
        Cpu& cpu = m_Gameboy->GetCpu();

        bool ime = cpu.GetIME();
        bool isHalted = cpu.IsHalted();
        CpuRegistersSet& regs = cpu.GetRegisters();

        ImGui::Begin("CPU State");

        ImGui::Text(regs.ToString().c_str());

        ImGui::Text("\nIME: %s\n", ime ? "1" : "0");
        ImGui::Text("Halted: %s\n", isHalted ? "true" : "false");

        ImGui::End();
    }

} // namespace GBE
