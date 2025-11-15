#include "Renderer.h"
#include <cassert>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

namespace GBE
{
    Renderer::Renderer(SDL_Window *sdlWindow, std::shared_ptr<Ppu> ppu)
    {
        m_Ppu = ppu;

        // create sdl renderer
        m_SDLRenderer = SDL_CreateRenderer(sdlWindow, nullptr);

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

        // Initialize Dear ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        // SDL3 + SDL_Renderer3 backends
        ImGui_ImplSDL3_InitForSDLRenderer(sdlWindow, m_SDLRenderer);
        ImGui_ImplSDLRenderer3_Init(m_SDLRenderer);
    }

    Renderer::~Renderer()
    {
        // Shutdown Dear ImGui
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyTexture(m_SDLTexture);
        SDL_DestroyRenderer(m_SDLRenderer);
    }

    void Renderer::Render(float delta, int32_t width, int32_t height)
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

        float fWidth = static_cast<float>(width);
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

        // ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Basic UI
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Open ROM...", nullptr, false, false); // placeholder
                ImGui::MenuItem("Exit", nullptr, false, false);        // handled via SDL event
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                static bool show_demo = false;
                ImGui::MenuItem("ImGui Demo", nullptr, &show_demo);
                if (show_demo) ImGui::ShowDemoWindow(&show_demo);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Begin("GBE");
        float fps_imgui = (delta > 0.0f) ? (1.0f / delta) : 0.0f;
        ImGui::Text("FPS: %.1f", fps_imgui);
        ImGui::End();

        // Render ImGui over the SDL scene
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());

        // show fps
        SDL_SetRenderDrawColor(m_SDLRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        float fps = 0.0f;
        if (delta > 0.0f)
            fps = 1.0f / delta;

        SDL_RenderDebugTextFormat(m_SDLRenderer, 16.0f, 16.0f, "%f FPS", fps);

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
                m_Pixels[x + y * LCD_SCREEN_WIDTH] = color;
            }
        }

        SDL_UpdateTexture(m_SDLTexture, nullptr, m_Pixels.data(), LCD_SCREEN_WIDTH * sizeof(ColorRGB32));
    }
} // namespace GBE
