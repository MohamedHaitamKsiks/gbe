#pragma once

#include <array>
#include "GuiWindow.h"

namespace GBE
{
    class GuiMemoryDump: public GuiWindow
    {
    public:
        GuiMemoryDump(
            std::shared_ptr<Window> window, 
            std::shared_ptr<Renderer> renderer, 
            std::shared_ptr<Gameboy> gameboy
        );
        ~GuiMemoryDump() = default;

    private:
        std::array<uint16_t, 2> m_DumpRange = { 0 };
        int m_NumberOfCols = 0;
        void _RenderWindow() override;
    };
} // namespace GBE
