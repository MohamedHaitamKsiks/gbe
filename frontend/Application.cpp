#include "Application.h"

#include <SDL3/SDL.h>
#include <string>
#include <print>

#include "gameboy/Gameboy.h"

#include "rendering/Window.h"
#include "rendering/Renderer.h"
#include "gui/GuiLayer.h"
#include "event/EventManager.h"


namespace GBE
{
    Application::Application()
    {
        SDL_Init(SDL_INIT_VIDEO);

        m_GB = std::make_shared<Gameboy>();
        m_Window = std::make_shared<Window>(1280, 720);
        m_Renderer = std::make_shared<Renderer>(m_Window, m_GB->GetPpu());
        m_GuiLayer = std::make_shared<GuiLayer>(m_Window, m_Renderer, m_GB);
        m_EventManager = std::make_shared<EventManager>(m_Window, m_GB->GetJoypad(), m_GuiLayer);
    }

    Application::~Application()
    {
        std::atexit([]() {
            std::println("Quitting SDL...");
            SDL_Quit();
        });
    }

    bool Application::IsRunning() const
    {
        return !m_Window->IsClosed();
    }

    void Application::Update(float delta)
    {
        // ticks for one frame every 16.74 ms
        constexpr float FRAME_TIME = 16.74f * 0.001f; // seconds
        m_GBTickTimer += delta;
        if (m_GBTickTimer >= FRAME_TIME)
        {
            m_GB->Tick();
            m_GBTickTimer -= FRAME_TIME;
        }

        // update window and render 
        m_EventManager->ProcessEvents();
        m_Window->Update();

        m_Renderer->BeginFrame();
        m_GuiLayer->Render(delta);
        m_Renderer->EndFrame();
    }

} // namespace GBE
