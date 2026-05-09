#pragma once

#include <map>
#include <memory>

#include "GuiWindow.h"

namespace GBE
{
    class Assembly;
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
        int m_MaxSectionStart = 0x0;
        int m_MaxSectionEnd = 0x7FFF;

        void _RenderWindow() override;
        void _Disassemble();
        void _RenderAssembly(const Assembly &assembly);
    };
} // namespace GBE
