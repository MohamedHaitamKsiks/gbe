#pragma once

#include <map>
#include <memory>
#include <array>
#include <cstdint>

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
        uint16_t m_StartPC = 0x100;
        std::array<uint16_t, 2> m_MaxSection = {};

        void _RenderWindow() override;
        void _Disassemble();
        void _RenderAssembly(const Assembly &assembly);
    };
} // namespace GBE
