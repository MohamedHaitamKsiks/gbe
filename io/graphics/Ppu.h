#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <memory>
#include <coroutine>

#include "lcd/LcdScreen.h"
#include "lcd/LcdControl.h"
#include "lcd/LcdPalettesMemory.h"
#include "vram/Vram.h"
#include "oam/ObjectAttributesMemory.h"

#include "PpuTask.h"
#include "PpuTaskManager.h"
#include "PixelFIFO.h"

#include "io/interrupts/InterruptManager.h"
#include "util/Signal.h"

namespace GBE
{
    enum class PpuMode
    {
        OAM_SCAN = 2, 
        DRAW_PIXELS = 3,
        H_BLANK = 0,
        V_BLANK = 1
    };

    constexpr uint32_t DOT_TO_M_CYCLE = 4;  
    constexpr uint32_t V_BLANK_DOTS = 4560;
    constexpr uint32_t RENDER_LINE_DOTS = 456;
    constexpr uint32_t OAM_SCAN_DOTS = 80;
    constexpr uint32_t FRAME_DOTS = 70224;
    constexpr uint32_t FETCH_BG_DOTS = 6;

    // pixel processing unit
    class Ppu
    {
    public:
        using VBlankSignal = Signal<const LcdScreen&>;

        Ppu(std::shared_ptr<InterruptManager> interruptManager);
        ~Ppu();

        // init 
        void Init();

        // tick ppu n dots
        void Tick(uint32_t dots);

        // get vram
        inline std::shared_ptr<Vram> GetVram()
        {
            return m_Vram;
        }

        // get oam
        inline std::shared_ptr<ObjectAttributesMemory> GetOam()
        {
            return m_Oam;
        }

        // get lcd control register
        inline std::shared_ptr<LcdControl> GetLcdControl()
        {
            return m_LcdControl;
        }

        // get lcd palettes
        inline std::shared_ptr<LcdPalettesMemory> GetLcdPalettes()
        {
            return m_Palettes;
        }

        // get screen
        inline const LcdScreen& GetLcdScreen() const
        {
            return m_Screen;
        }

        inline uint32_t GetDotsCounter() const
        {
            return m_DotsCounter;
        }

        inline VBlankSignal& GetVBlankSignal() 
        {
            return m_VBlankSignal;
        }

        inline PpuMode GetPpuMode() const
        {
            return m_PpuMode;
        }

    private:
        VBlankSignal m_VBlankSignal{};

        // dot counter
        uint32_t m_FrameCounter = 0;
        uint32_t m_DotsCounter = 0;
        uint32_t m_LineDotsCounter = 0;
        uint32_t m_WaitDots = 0;
        uint32_t m_HBlankWaitDots = 0;


        // 8 KiB Video Ram
        std::shared_ptr<Vram> m_Vram{nullptr};

        // Object Attribute Memory
        std::shared_ptr<ObjectAttributesMemory> m_Oam{nullptr};

        // pallettes
        std::shared_ptr<LcdPalettesMemory> m_Palettes{nullptr};

        // LCD Control
        std::shared_ptr<LcdControl> m_LcdControl{nullptr};
        std::shared_ptr<InterruptManager> m_InterruptManager;

        // ppu mode 
        PpuMode m_PpuMode;
        PpuMode m_QueuePpuMode;

        // output screen
        LcdScreen m_Screen{};
        int32_t m_LcdX = 0;
        int32_t m_LcdY = 0;
        bool m_IsRendering = true;
        
        PixelFIFO m_ObjectFIFO{};
        PixelFIFO m_BackgroundFIFO{};

        // fetcher
        uint16_t m_TileIndex = 0;
        uint8_t m_TileY = 0;
        bool m_FetchWindow = false;

        void _Render();
        void _OAMScan();
        void _DrawingPixels();
        void _HorizontalBlank();
        void _VerticalBlank();
        void _DrawPixel();

        void _Fetch();

    };
} // namespace GBE
