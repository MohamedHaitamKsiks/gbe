#pragma once

#include <memory>
#include <unordered_map>

#include "frontend/gui/window/GuiWindow.h"
#include "frontend/gui/GuiLayer.h"

namespace GBE
{
    class GuiMainMenu : public GuiLayer
    {
    public:
        GuiMainMenu(
            std::shared_ptr<Window> window,
            std::shared_ptr<Renderer> renderer,
            std::shared_ptr<Gameboy> gameboy
        );
        ~GuiMainMenu();
        
    private:
        struct _GuiWindowInfo
        {
            std::shared_ptr<GuiWindow> _Window = nullptr;
            bool _IsVisible = false;
        };

        std::unordered_map<
            std::string,
            std::vector<_GuiWindowInfo>
        > m_Windows{};

        template <typename T>
        void _AddWindow(std::string_view category)
        {
            auto window = AddLayer<T>();
            _AddWindow(category, window);
        }
        void _AddWindow(std::string_view category, std::shared_ptr<GuiWindow> window);
        
        void _LoadRom(std::string_view path);

        void _RenderImp();
        void _RenderFile();
        void _RenderWindowsList();
    };
} // namespace GBE
