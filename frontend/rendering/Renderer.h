#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <vector>
#include <memory>
#include <cstdint>
#include "frontend/gui/GuiManager.h"
#include "io/graphics/lcd/LcdScreen.h"

namespace GBE
{
    class Window;
    class Gameboy;
    
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
        Renderer(std::shared_ptr<Window> window, std::shared_ptr<Gameboy> ppu);
        ~Renderer();

        void BeginFrame();
        void EndFrame();
        
        inline SDL_Renderer* GetSDLRenderer() const 
        { 
            return m_SDLRenderer; 
        }

    private:
        std::shared_ptr<Window> m_Window = nullptr;
        std::shared_ptr<Gameboy> m_GB = nullptr;

        SDL_Renderer* m_SDLRenderer = nullptr;
        SDL_Texture * m_SDLTexture = nullptr;

        std::array<uint8_t, LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * 3> m_Pixels;
        std::vector<ColorRGB32> m_ColorPalette; 

        void _UpdateTexture();
    };
} // namespace GBE
