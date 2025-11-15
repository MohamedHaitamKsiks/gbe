#include "Application.h"

#include <chrono>
#include <thread>
#include <string>


namespace GBE
{
    constexpr float SEC_TO_MICRO = 1000000.0f;
    
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
        catridge->LoadFromAssets("assets/test_roms/cpu_instrs.gb");

        m_GB.Start(std::move(catridge));

        float delta = 0.0f;

        // get time now=
        m_Window = std::make_unique<Window>(m_GB.GetPpu(), m_GB.GetJoypad(), 1280, 720);

        m_Window->SetOpenRomCallback([this](const std::string &romPath) {
            auto cartridge = std::make_shared<Cartridge>();
            cartridge->Load(romPath);
            m_GB.Stop();
            m_GB.Start(std::move(cartridge));
        });

        while (!m_Window->IsClosed())
        {
            const auto pastTime = std::chrono::high_resolution_clock::now();
            
            auto ppu = m_GB.GetPpu();

            // ticks for one frame
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
