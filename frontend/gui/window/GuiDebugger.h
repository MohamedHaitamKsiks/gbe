#pragma once

#include "GuiWindow.h"  
#include <memory>

namespace GBE
{
    class Gameboy;
    class CpuDebugger;
    class GuiDebugger: public GuiWindow
    {
    public:
        GuiDebugger(
            std::shared_ptr<Window> window, 
            std::shared_ptr<Renderer> renderer, 
            std::shared_ptr<Gameboy> gameboy
        );
        ~GuiDebugger();

    private:
        int m_NewBpAddress = 0;
    
        void _RenderWindow() override;
        void _RenderDebuggerActions(CpuDebugger &debugger);
        void _RenderListBreakpoints(CpuDebugger &debugger);
        void _RenderBreakpoint(CpuDebugger &debugger, uint16_t bp);
    };
} // namespace GBE
