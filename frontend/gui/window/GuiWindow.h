#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "frontend/gui/GuiLayer.h"

namespace GBE
{
    class Gameboy;
    class Renderer;
    class Window;

    class GuiWindow: public GuiLayer
    {
    public:
        GuiWindow(
            std::shared_ptr<Window> window, 
            std::shared_ptr<Renderer> renderer, 
            std::shared_ptr<Gameboy> gameboy
        );
        virtual ~GuiWindow();

        inline const std::string& GetName() const
        {
            return m_Name;
        }

        inline void SetName(std::string_view name)
        {
            m_Name = name;
        }

    protected:
        std::string m_Name = "GBE";
        virtual void _RenderWindow() = 0; 
        
    private:
        void _RenderImp();
    };
} // namespace GBE
