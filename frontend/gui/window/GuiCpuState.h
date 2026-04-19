#pragma once

#include "GuiWindow.h"
#include <memory>
#include <vector>

namespace GBE
{
    class GuiCpuState: public GuiWindow
    {
    public:
        GuiCpuState(
            std::shared_ptr<Window> window, 
            std::shared_ptr<Renderer> renderer, 
            std::shared_ptr<Gameboy> gameboy
        );
        ~GuiCpuState() {}
    private:
        void _RenderWindow() override;
    };
} // namespace GBE
