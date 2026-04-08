#include "Window.h"

#include <print>
#include <SDL3/SDL.h>

#include "util/Assert.h"

namespace GBE
{
    Window::Window(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        m_SDLWindow = SDL_CreateWindow("GBE", width, height, SDL_WINDOW_RESIZABLE);
        GBE_ASSERT(m_SDLWindow);
    }


    Window::~Window()
    {
        SDL_DestroyWindow(m_SDLWindow);
    }

    void Window::Update()
    {
        // update width / height
        int width, height;
        if (!SDL_GetWindowSize(m_SDLWindow, &width, &height))
            return; 

        m_Width = width;
        m_Height = height;
    }

    void Window::OpenFileDialog(const FileDialogCallback& callback)
    {
        if (m_IsFileDialogOpen)
            return;

        m_FileDialogCallback = callback;

        auto fileCallback = [](void *userdata, const char *const *fileList, int numFilters)
        {
            Window* pSelf = reinterpret_cast<Window *>(userdata);
            if (!pSelf)
                return; 

            size_t fileListIndex = 0;
            std::vector<std::string> files;
            while (auto* file = fileList[fileListIndex])
            {
                files.emplace_back(file);
                fileListIndex++;
            }

            (pSelf->m_FileDialogCallback)(files);
            pSelf->m_IsFileDialogOpen = false;
        };
        SDL_ShowOpenFileDialog(fileCallback, this, m_SDLWindow, nullptr, 0, nullptr, false);
        m_IsFileDialogOpen = true;
    }

    //}
} // namespace GBE
