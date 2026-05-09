#pragma once

#include <memory>
#include <string_view>
#include <vector>

union SDL_Event;

namespace GBE
{
    class Window;
    class Renderer;
    class Gameboy;
    class GuiLayer;

    class GuiManager
    {
    public:
        GuiManager(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer, std::shared_ptr<Gameboy> gameboy);
        ~GuiManager();

        void Render(float delta);
        bool ProcessEvent(const SDL_Event& event);

    private:
        std::shared_ptr<Window> m_Window = nullptr;
        std::shared_ptr<Renderer> m_Renderer = nullptr;
        std::shared_ptr<Gameboy> m_Gameboy = nullptr;

        std::shared_ptr<GuiLayer> m_RootLayer = nullptr;
        void _RenderFPSCounter(float delta);
    };
} // namespace GBE
