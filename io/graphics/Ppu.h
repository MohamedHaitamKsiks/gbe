#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <memory>
#include <coroutine>

#include "lcd/LcdScreen.h"
#include "PixelFIFO.h"

#include "util/Class.h"

namespace GBE
{
    class InterruptManager;
    class Memory;
    class Vram;
    class ObjectAttributesMemory;
    class LcdControl;
    class LcdPalettesMemory;

    enum class PpuMode
    {
        OAM_SCAN = 2, 
        DRAW_PIXELS = 3,
        H_BLANK = 0,
        V_BLANK = 1
    };

    constexpr uint32_t DOT_TO_M_CYCLE = 4;  
    constexpr uint32_t V_BLANK_LINE_COUNT = 10;
    constexpr uint32_t RENDER_LINE_DOTS = 456;
    constexpr uint32_t LCD_SCREEN_FULL_HEIGHT = LCD_SCREEN_HEIGHT + V_BLANK_LINE_COUNT;
    constexpr uint32_t OAM_SCAN_DOTS = 80;
    constexpr uint32_t FRAME_DOTS = 70224;
    constexpr uint32_t FETCH_BG_DOTS = 6;

    // pixel processing unit
    class Ppu
    {
    public:
        GBE_CLASS_NO_COPY_NO_MOVE(Ppu)

        Ppu(
            const std::shared_ptr<Vram>& vram, 
            const std::shared_ptr<ObjectAttributesMemory>& oam,
            const std::shared_ptr<LcdPalettesMemory>& palettes,
            const std::shared_ptr<LcdControl>& lcdControl,
            const std::shared_ptr<InterruptManager>& interruptManager
        );
        ~Ppu();

        // init 
        void Init();

        // tick ppu n dots
        void Tick(uint32_t dots);

        // get screen
        inline const LcdScreen& GetLcdScreen() const
        {
            return m_Screen;
        }

        inline uint32_t GetDotsCounter() const
        {
            return m_DotsCounter;
        }

        inline PpuMode GetPpuMode() const
        {
            return m_PpuMode;
        }

    private:
        // dot counter
        uint32_t m_FrameCounter = 0;
        uint32_t m_DotsCounter = 0;
        uint32_t m_LineDotsCounter = 0;
        uint32_t m_WaitDots = 0;
        uint32_t m_HBlankWaitDots = 0;

        // memory 
        std::shared_ptr<Vram> m_Vram = nullptr;
        std::shared_ptr<ObjectAttributesMemory> m_Oam = nullptr;
        std::shared_ptr<LcdPalettesMemory> m_Palettes = nullptr;
        std::shared_ptr<LcdControl> m_LcdControl = nullptr;
        std::shared_ptr<InterruptManager> m_InterruptManager = nullptr;

        // ppu mode 
        PpuMode m_PpuMode;
        PpuMode m_QueuePpuMode;

        // output screen
        LcdScreen m_Screen{};
        uint32_t m_LcdX = 0;
        uint32_t m_LcdY = 0;
        bool m_IsRendering = true;

        PixelFIFO m_ObjectFIFO{};
        PixelFIFO m_BackgroundFIFO{};

        // tile
        uint8_t m_TileY = 0;
        uint8_t m_TileX = 0;

        uint8_t m_TileOffsetY = 0;
        uint8_t m_TileOffsetX = 0;
        
        // window
        uint8_t m_WindowInternalY = 0;
        bool m_IsOnWindow = false;

        // queue interrupt
        bool m_VBlankInterrupt = false;

        // objects
        std::vector<uint8_t> m_LineObjects{};

        void _Render();
        void _OAMScan();
        void _DrawingPixels();
        void _HorizontalBlank();
        void _VerticalBlank();
        void _DrawPixel();

        void _FetchBackgroundFIFO();
        void _FetchObjectsFIFO();
    };
} // namespace GBE
