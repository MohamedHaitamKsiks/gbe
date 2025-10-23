#pragma once

#include <cstdint>
#include <SDL3/SDL.h>
#include "Renderer.h"

namespace GBE
{
    class Ppu;
    
    class Window
    {
    public:
        Window(std::shared_ptr<Ppu> ppu, int32_t width, int32_t height);
        ~Window();

        bool IsClosed() const;

        void Update(float delta);

    private:
        int32_t m_Width = -1;
        int32_t m_Height = -1;

        SDL_Window *m_Window = nullptr;
        bool m_IsClosed = false;

        std::unique_ptr<Renderer> m_Renderer = nullptr;
    };
} // namespace GBE
