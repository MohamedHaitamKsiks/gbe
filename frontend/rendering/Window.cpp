#include "Window.h"
#include <cassert>


namespace GBE
{
    Window::Window(std::shared_ptr<Ppu> ppu, int32_t width, int32_t height)
    {
        m_Width = width;
        m_Height = height;

        SDL_Init(SDL_INIT_VIDEO);
        m_Window = SDL_CreateWindow("GBE", width, height, SDL_WINDOW_RESIZABLE);

        m_Renderer = std::make_unique<Renderer>(m_Window, ppu);
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
            if (event.type == SDL_EVENT_QUIT)
            {
                m_IsClosed = true;
            }
        }


        // update width / height
        SDL_GetWindowSize(m_Window, &m_Width, &m_Height);
        m_Renderer->Render(delta, m_Width, m_Height);
    }
} // namespace GBE
