#include "Application.h"

#include <chrono>
#include <thread>

#define SEC_TO_MICRO 1000000.0f

namespace GBE
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        // load cartridge
        std::shared_ptr<Cartridge> catridge = std::make_shared<Cartridge>();
        catridge->Load("/home/ksiks_wa3r/Documents/workspace/gbe/assets/test_roms/dmg-acid2.gb");

        m_GB.Start(std::move(catridge));

        float delta = 0.0f;

        // get time now=
        m_Window = std::make_unique<Window>(m_GB.GetPpu(), 1280, 720);
        while (!m_Window->IsClosed())
        {
            const auto pastTime = std::chrono::high_resolution_clock::now();
            
            auto ppu = m_GB.GetPpu();

            // tick until we reach vblank
            m_GB.Tick();

            m_Window->Update(delta);


            // compute delta
            const auto currentTime = std::chrono::high_resolution_clock::now();
            delta = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - pastTime).count() / SEC_TO_MICRO;
        
            // sleep 
            float wait = 16.74 - (delta * 1000.0f);
            if (wait < 0.0f)
                wait = 0.01f;
            SDL_Delay(wait);

        }
    }

} // namespace GBE
