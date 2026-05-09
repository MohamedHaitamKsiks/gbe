#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <memory>
#include "Renderer.h"
#include "frontend/gui/GuiManager.h"

class SDL_Window;

namespace GBE
{
    class Window
    {
    public:
        using FileDialogCallback = std::function<void(const std::vector<std::string>&)>;

        Window(uint32_t width, uint32_t height);
        ~Window();

        inline bool IsClosed() const
        {
            return m_IsClosed;
        }

        inline void Close()
        {
            m_IsClosed = true;
        }

        void Update();

        inline uint32_t GetWidth() const 
        { 
            return m_Width; 
        }

        inline uint32_t GetHeight() const 
        { 
            return m_Height; 
        }

        inline SDL_Window* GetSDLWindow() const 
        { 
            return m_SDLWindow; 
        }

        void OpenFileDialog(const FileDialogCallback& callback);

    private:
        SDL_Window *m_SDLWindow = nullptr;
        
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        bool m_IsClosed = false;

        FileDialogCallback m_FileDialogCallback;
        bool m_IsFileDialogOpen = false;

    };
} // namespace GBE
