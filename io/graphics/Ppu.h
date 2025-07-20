#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <memory>
#include <coroutine>

#include "lcd/LcdScreen.h"
#include "lcd/LcdControl.h"
#include "vram/Vram.h"
#include "oam/ObjectAttributesMemory.h"

#include "PpuTask.h"
#include "PpuTaskManager.h"

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

    // pixel processing unit
    class Ppu
    {
    public:
        Ppu();
        ~Ppu();

        // init 
        void Init();

        // tick ppu n dots
        void Tick(uint32_t dots);

        // get vram
        inline Vram& GetVram()
        {
            return m_Vram;
        }

        // get oam
        inline ObjectAttributesMemory& GetOam()
        {
            return m_Oam;
        }

        // get lcd control register
        inline LcdControl& GetLcdControl()
        {
            return m_LcdControl;
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
    private:
        // dot counter
        uint32_t m_DotsCounter = 0;
        uint32_t m_PixelIndex = 0;

        // 8 KiB Video Ram
        Vram m_Vram{};

        // Object Attribute Memory
        ObjectAttributesMemory m_Oam{};

        // LCD Control
        LcdControl m_LcdControl{};
        PpuMode m_PpuMode;

        // output screen
        LcdScreen m_Screen{};

        // tick ppu one time 
        // async function recalled each dot
        PpuTaskManager m_TaskManager;
        bool m_IsRendering = true;
        
        PpuTask _Render();
        PpuTask _OAMScan();
        PpuTask _DrawingPixels();
        PpuTask _HorizontalBlank(uint32_t dots);
        PpuTask _VerticalBlank();
    };
} // namespace GBE
