#pragma once

#include <memory>

namespace GBE
{
    class Joypad;
    class GuiLayer;
    class Window;

    class EventManager
    {
    public:
        EventManager(std::shared_ptr<Window> window, std::shared_ptr<Joypad> joypad, std::shared_ptr<GuiLayer> guiLayer);
        ~EventManager() = default;

        void ProcessEvents();
    private:
        std::shared_ptr<Window> m_Window = nullptr;
        std::shared_ptr<Joypad> m_Joypad = nullptr;
        std::shared_ptr<GuiLayer> m_GuiLayer = nullptr;
    };
} // namespace GBE
