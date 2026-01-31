#pragma once

#include <cstdint>
#include <SDL3/SDL.h>
#include <functional>
#include <string>
#include <memory>
#include "Renderer.h"
#include "frontend/gui/GuiLayer.h"

#include "io/joypad/Joypad.h"

namespace GBE
{
    class Ppu;
    
    class Window
    {
    public:
        Window(const std::shared_ptr<Ppu>& ppu, const std::shared_ptr<Joypad>& joypad, int32_t width, int32_t height);
        ~Window();

        bool IsClosed() const;

        void Update(float delta);
        inline void SetOpenRomCallback(std::function<void(std::string_view)> cb)
        {
            m_GuiLayer->SetOpenRomCallback(std::move(cb));
        }

    private:
        int32_t m_Width = -1;
        int32_t m_Height = -1;

        SDL_Window *m_Window = nullptr;
        bool m_IsClosed = false;

        std::unique_ptr<Renderer> m_Renderer = nullptr;
        std::shared_ptr<GuiLayer> m_GuiLayer = nullptr;
        std::shared_ptr<Joypad> m_Joypad = nullptr;

        void _ProcessEvents();
    };
} // namespace GBE
