#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <vector>
#include <memory>
#include <cstdint>

#include "io/graphics/Ppu.h"
#include "frontend/gui/GuiLayer.h"
#include "io/graphics/lcd/LcdScreen.h"

namespace GBE
{
    struct ColorRGB32
    {
        uint8_t Red = 0x0;
        uint8_t Green = 0x0;
        uint8_t Blue = 0x0;
    };


    // draw ppu screen to the current window
    class Renderer
    {
    public:
        Renderer(SDL_Window* sdlWindow, std::shared_ptr<Ppu> ppu);
        ~Renderer();

        inline void SetGuiLayer(std::shared_ptr<GuiLayer> guiLayer)
        {
            m_GuiLayer = guiLayer;
        }

        void Render(float delta, int32_t width, int32_t height);
        SDL_Renderer* GetRenderer() const { return m_SDLRenderer; }

    private:
        std::shared_ptr<Ppu> m_Ppu = nullptr;
        std::shared_ptr<GuiLayer> m_GuiLayer = nullptr;

        SDL_Window*  m_SDLWindow  = nullptr;
        SDL_Renderer* m_SDLRenderer = nullptr;
        SDL_Texture * m_SDLTexture = nullptr;

        std::array<uint8_t, LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * 3> m_Pixels;
        std::vector<ColorRGB32> m_ColorPalette; 

        void _UpdateTexture();
    };
} // namespace GBE
