#include "GuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

namespace GBE
{
    GuiLayer::GuiLayer(SDL_Window* window, SDL_Renderer* renderer)
        : m_Window(window), m_Renderer(renderer)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
        ImGui_ImplSDLRenderer3_Init(m_Renderer);
    }

    GuiLayer::~GuiLayer()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiLayer::ProcessEvent(const SDL_Event& event)
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
    }

    bool GuiLayer::WantCaptureKeyboard() const
    {
        return ImGui::GetIO().WantCaptureKeyboard;
    }

    void GuiLayer::Render(float delta, SDL_Renderer* renderer)
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open ROM..."))
                {
                    auto file_callback = [](void* userdata, const char* const* fileList, int numFilters)
                    {
                        GuiLayer* self = reinterpret_cast<GuiLayer*>(userdata);
                        if (self && self->m_OnOpenRom && fileList && fileList[0])
                        {
                            self->m_OnOpenRom(std::string(fileList[0]));
                        }
                    };
                    SDL_ShowOpenFileDialog(file_callback, this, m_Window, nullptr, 0, nullptr, false);
                }

                if (ImGui::MenuItem("Exit", nullptr, false, false))
                {

                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ImGui::Begin("GBE");
        float fps_imgui = (delta > 0.0f) ? (1.0f / delta) : 0.0f;
        ImGui::Text("FPS: %.1f", fps_imgui);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
} // namespace GBE
