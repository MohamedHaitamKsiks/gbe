#pragma once

#include <memory>
#include <vector>

namespace GBE
{
    class Gameboy;
    class Window;
    class Renderer;

    class GuiLayer
    {
    public:
        GuiLayer(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer, std::shared_ptr<Gameboy> gameboy);
        virtual ~GuiLayer();
        void Render();

        inline bool IsVisible() const
        {
            return m_IsVisible;
        }

        inline void SetVisible(bool visible)
        {
            m_IsVisible = visible;
        }

        template <typename T>
        std::shared_ptr<T> AddLayer()
        {
            auto layer = std::make_shared<T>(m_Window, m_Renderer, m_Gameboy);
            m_Layers.push_back(layer);
            return layer;
        }

    protected:
        virtual void _RenderImp() {};

        std::shared_ptr<Gameboy> m_Gameboy = nullptr;
        std::shared_ptr<Renderer> m_Renderer = nullptr;
        std::shared_ptr<Window> m_Window = nullptr;

        std::vector<std::shared_ptr<GuiLayer>> m_Layers{};

        bool m_IsVisible = true;
    };
} // namespace GBE
