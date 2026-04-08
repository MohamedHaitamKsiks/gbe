#include "Renderer.h"

#include "Window.h"
#include "io/graphics/Ppu.h"
#include "util/Assert.h"

#include <SDL3/SDL.h>
#include <cassert>

namespace GBE
{
    Renderer::Renderer(std::shared_ptr<Window> window, std::shared_ptr<Ppu> ppu): 
        m_Window(window),
        m_Ppu(ppu)
    {
        GBE_ASSERT(m_Window);
        GBE_ASSERT(m_Ppu);

        // create sdl renderer
        SDL_Window* sdlWindow = window->GetSDLWindow();
        m_SDLRenderer = SDL_CreateRenderer(sdlWindow, nullptr);
        GBE_ASSERT(m_SDLRenderer);

        // init color palette
        m_ColorPalette.push_back({224, 248, 208});
        m_ColorPalette.push_back({136, 192, 112});
        m_ColorPalette.push_back({52, 104, 86});
        m_ColorPalette.push_back({8, 24, 32});

        assert(m_ColorPalette.size() == LCD_COLORS_NUMBER);

        // create texture
        m_SDLTexture = SDL_CreateTexture(
            m_SDLRenderer,
            SDL_PIXELFORMAT_RGB24,
            SDL_TEXTUREACCESS_STREAMING,
            LCD_SCREEN_WIDTH,
            LCD_SCREEN_HEIGHT
        );
        SDL_SetTextureScaleMode(m_SDLTexture, SDL_SCALEMODE_NEAREST);
    }

    Renderer::~Renderer()
    {
        SDL_DestroyTexture(m_SDLTexture);
        SDL_DestroyRenderer(m_SDLRenderer);
    }

    void Renderer::BeginFrame()
    {
        // clear
        SDL_SetRenderDrawColor(m_SDLRenderer, 0, 0, 0, 255);
        SDL_RenderClear(m_SDLRenderer);

        // keep aspect ratio and draw texture
        _UpdateTexture();
        float aspectRatio = static_cast<float>(LCD_SCREEN_WIDTH) / static_cast<float>(LCD_SCREEN_HEIGHT);

        SDL_FRect rect = {
            0.0f,
            0.0f,
            0.0f,
            0.0f
        };

        uint32_t width = m_Window->GetWidth();
        float fWidth = static_cast<float>(width);

        uint32_t height = m_Window->GetHeight();
        float fHeight = static_cast<float>(height);
        if (fWidth > fHeight)
        {
            rect.h = fHeight;
            rect.w = fHeight * aspectRatio;
            rect.x = (fWidth - rect.w) * 0.5f;
        }
        else
        {
            rect.w = fWidth;
            rect.h = fWidth / aspectRatio;
            rect.y = (fHeight - rect.h) * 0.5f;
        }

        // draw
        SDL_RenderTexture(m_SDLRenderer, m_SDLTexture, nullptr, &rect);
    }

    void Renderer::EndFrame()
    {
        SDL_RenderPresent(m_SDLRenderer);
    }

    void Renderer::_UpdateTexture()
    {
        // update texture
        const auto &lcdScreen = m_Ppu->GetLcdScreen();
        for (int y = 0; y < LCD_SCREEN_HEIGHT; y++)
        {
            for (int x = 0; x < LCD_SCREEN_WIDTH; x++)
            {
                uint8_t pixel = lcdScreen.GetPixel(x, y);
                ColorRGB32 color = m_ColorPalette[pixel % m_ColorPalette.size()];
                
                int index = (x + y * LCD_SCREEN_WIDTH) * 3;
                m_Pixels[index + 0] = color.Red;
                m_Pixels[index + 1] = color.Green;
                m_Pixels[index + 2] = color.Blue;
            }
        }

        SDL_UpdateTexture(m_SDLTexture, nullptr, m_Pixels.data(), LCD_SCREEN_WIDTH * 3);
    }
} // namespace GBE
