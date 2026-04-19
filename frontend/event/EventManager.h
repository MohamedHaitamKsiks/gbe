#pragma once

#include <memory>

namespace GBE
{
    class Gameboy;
    class GuiManager;
    class Window;

    class EventManager
    {
    public:
        EventManager(std::shared_ptr<Window> window, std::shared_ptr<Gameboy> gameboy, std::shared_ptr<GuiManager> GuiManager);
        ~EventManager() = default;

        void ProcessEvents();
    private:
        std::shared_ptr<Window> m_Window = nullptr;
        std::shared_ptr<Gameboy> m_Gameboy = nullptr;
        std::shared_ptr<GuiManager> m_GuiManager = nullptr;
    };
} // namespace GBE
