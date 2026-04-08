#pragma once

#include <memory>

namespace GBE
{
    class Gameboy;
    class Renderer;
    class Window;
    class GuiLayer;
    class EventManager;

    // application containing the front end of the game boy emulator
    class Application
    {
    public:
        Application();
        ~Application();

        bool IsRunning() const;
        void Update(float delta);
    private:
        std::shared_ptr<Gameboy> m_GB = nullptr;
        std::shared_ptr<Renderer> m_Renderer = nullptr;
        std::shared_ptr<Window> m_Window = nullptr;
        std::shared_ptr<GuiLayer> m_GuiLayer = nullptr;
        std::shared_ptr<EventManager> m_EventManager = nullptr;

        float m_GBTickTimer = 0.0f;

    };
} // namespace GBE
