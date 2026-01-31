#pragma once

#include <SDL3/SDL.h>
#include <functional>
#include <string>
#include <string_view>

namespace GBE
{
    
    class GuiLayer
    {
    public:
        GuiLayer(SDL_Window* window, SDL_Renderer* renderer);
        ~GuiLayer();

        inline void SetOpenRomCallback(std::function<void(std::string_view)> cb)
        {
            m_OnOpenRom = std::move(cb);
        }

        void Render(float delta, SDL_Renderer* renderer);
        void ProcessEvent(const SDL_Event& event);
        bool WantCaptureKeyboard() const;

    private:
        SDL_Window*   m_Window   = nullptr;
        SDL_Renderer* m_Renderer = nullptr;

        std::function<void(std::string_view)> m_OnOpenRom;
        bool m_ShowOpenRom = false;
        char m_OpenRomPath[512] = "";
    };
} // namespace GBE
