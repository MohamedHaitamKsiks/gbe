#pragma once

#include <map>
#include <memory>

#include "GuiWindow.h"

namespace GBE
{
    class GuiDisassembler : public GuiWindow
    {
    public:
        GuiDisassembler(
            std::shared_ptr<Window> window, 
            std::shared_ptr<Renderer> renderer, 
            std::shared_ptr<Gameboy> gameboy
        );
        ~GuiDisassembler();
    private:
        int m_StartPC = 0x100;

        void _RenderWindow() override;
    };
} // namespace GBE
