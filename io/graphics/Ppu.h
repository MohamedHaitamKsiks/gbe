#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "LcdScreen.h"
#include "LcdPalette.h"
#include "Vram.h"

namespace GBE
{
    enum class PpuMode
    {
        OAM_SCAN = 2, 
        DRAW_PIXELS = 3,
        H_BLANK = 0,
        V_BLANK = 1
    };

    struct PpuPalettes
    {
        LcdColor BGP{};
        std::array<LcdColor, 2> OBP{}; 
    };

    class Ppu
    {
    public:
        ~Ppu() {}

        // tick ppu n dots
        void Tick(uint32_t dots);

        // get palettes
        inline PpuPalettes& GetPalettes() 
        {
            return m_Palettes;
        }

        // get screen 
        inline const LcdScreen& GetLcdScreen() const
        {
            return m_Screen;
        }
    private:
        // dot counter
        uint32_t m_DotsCounter = 0;
        uint32_t m_PixelIndex = 0;

        // 8 KiB Video Ram
        Vram m_Vram{};

        // Object Attribute Memory

        // color pallets
        PpuPalettes m_Palettes{};

        // output screen
        LcdScreen m_Screen{};
    };
} // namespace GBE
