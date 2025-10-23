#pragma once

#include "frontend/rendering/Window.h"

#include "gameboy/Gameboy.h"

namespace GBE
{
    // application containing the front end of the game boy emulator
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
    private:
        Gameboy m_GB{};
        std::shared_ptr<Window> m_Window = nullptr;
    };
} // namespace GBE
