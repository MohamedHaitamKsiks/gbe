#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <vector>
#include <memory>
#include <cstdint>
#include <functional>
#include <string>
#include "io/graphics/Ppu.h"

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

        void Render(float delta, int32_t width, int32_t height);
        void SetOpenRomCallback(std::function<void(const std::string&)> cb);

    private:
        std::shared_ptr<Ppu> m_Ppu = nullptr;

        SDL_Renderer* m_SDLRenderer = nullptr;
        SDL_Texture * m_SDLTexture = nullptr;

        std::array<ColorRGB32, LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT> m_Pixels;
        std::vector<ColorRGB32> m_ColorPalette; 

        // GUI: ROM loading
        std::function<void(const std::string&)> m_OnOpenRom;
        bool m_ShowOpenRom = false;
        char m_OpenRomPath[512] = "";

        void _UpdateTexture();
    };
} // namespace GBE
