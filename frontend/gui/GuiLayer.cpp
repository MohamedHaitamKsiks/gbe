#include "GuiLayer.h"

#include "util/Assert.h"

namespace GBE
{
    GuiLayer::GuiLayer(
        std::shared_ptr<Window> window, 
        std::shared_ptr<Renderer> renderer,
        std::shared_ptr<Gameboy> gameboy
    ):
        m_Window(window),
        m_Renderer(renderer),
        m_Gameboy(gameboy)
    {
        GBE_ASSERT(m_Window);
        GBE_ASSERT(m_Renderer);
        GBE_ASSERT(m_Gameboy);
    }

    GuiLayer::~GuiLayer()
    {
    }

    void GuiLayer::Render()
    {
        if (!m_IsVisible)
            return;

        _RenderImp();

        for (auto& layer: m_Layers)
            layer->Render();
    }

} // namespace GBE
