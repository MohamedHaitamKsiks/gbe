#include "Window.h"
#include <cassert>
#include "frontend/gui/GuiLayer.h"


namespace GBE
{
    Window::Window(const std::shared_ptr<Ppu>& ppu, const std::shared_ptr<Joypad>& joypad, int32_t width, int32_t height)
    {
        m_Width = width;
        m_Height = height;

        SDL_Init(SDL_INIT_VIDEO);
        m_Window = SDL_CreateWindow("GBE", width, height, SDL_WINDOW_RESIZABLE);

        m_Renderer = std::make_unique<Renderer>(m_Window, ppu);
        m_GuiLayer = std::make_unique<GuiLayer>(m_Window, m_Renderer->GetRenderer());
        m_Joypad = joypad;
    }


    Window::~Window()
    {
        if (m_Window)
            SDL_DestroyWindow(m_Window);

        SDL_Quit();
    }

    bool Window::IsClosed() const
    {
        return m_IsClosed;
    }

    void Window::Update(float delta)
    {
        SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (m_GuiLayer)
            m_GuiLayer->ProcessEvent(event);

        if (event.type == SDL_EVENT_QUIT)
        {
            m_IsClosed = true;
            break;
        }

        if (event.type != SDL_EventType::SDL_EVENT_KEY_DOWN && event.type != SDL_EventType::SDL_EVENT_KEY_UP)
            continue;

        if (m_GuiLayer && m_GuiLayer->WantCaptureKeyboard())
            continue;

            JoypadEvent joypadEvent{};
            joypadEvent.Pressed = event.key.down;
            bool doQueueEvent = true;

            switch (event.key.key)
            {
            case SDLK_LEFT:
                joypadEvent.Button = JoypadButton::LEFT;
                break;
            case SDLK_RIGHT:
                joypadEvent.Button = JoypadButton::RIGHT;
                break;
            case SDLK_UP:
                joypadEvent.Button = JoypadButton::UP;
                break;
            case SDLK_DOWN:
                joypadEvent.Button = JoypadButton::DOWN;
                break;
            case SDLK_S:
                joypadEvent.Button = JoypadButton::A;
                break;
            case SDLK_D:
                joypadEvent.Button = JoypadButton::B;
                break;
            case SDLK_SPACE:
                joypadEvent.Button = JoypadButton::SELECT;
                break;
            case SDLK_RETURN:
                joypadEvent.Button = JoypadButton::START;
                break;
            default:
                doQueueEvent = false;
                break;
            }
            
            if (doQueueEvent)
                m_Joypad->QueueJoypadEvent(joypadEvent);
        }


        // update width / height
        SDL_GetWindowSize(m_Window, &m_Width, &m_Height);
        m_Renderer->DrawScene(delta, m_Width, m_Height);
        if (m_GuiLayer)
            m_GuiLayer->Render(delta, m_Renderer->GetRenderer());
        m_Renderer->Present();
    }

    void Window::SetOpenRomCallback(std::function<void(const std::string&)> cb)
    {
        if (m_GuiLayer)
            m_GuiLayer->SetOpenRomCallback(std::move(cb));
    }
} // namespace GBE
