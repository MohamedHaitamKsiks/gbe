#pragma once

#include <memory>
#include <string_view>

union SDL_Event;

namespace GBE
{
    class Window;
    class Renderer;
    class Gameboy;

    class GuiLayer
    {
    public:
        GuiLayer(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer, std::shared_ptr<Gameboy> gameboy);
        ~GuiLayer();

        void Render(float delta);
        bool ProcessEvent(const SDL_Event& event);

    private:
        std::shared_ptr<Window> m_Window = nullptr;
        std::shared_ptr<Renderer> m_Renderer = nullptr;
        std::shared_ptr<Gameboy> m_Gameboy = nullptr;

        void _LoadRom(std::string_view path);
        void _RenderMainMenuBar();
        void _RenderFPSCounter(float delta);
        void _RenderCpuState();
    };
} // namespace GBE
