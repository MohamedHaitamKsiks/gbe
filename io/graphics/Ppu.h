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

        int32_t m_LcdX = 0;
        int32_t m_LcdY = 0;

        // 8 KiB Video Ram
        std::shared_ptr<Vram> m_Vram{nullptr};

        // Object Attribute Memory
        std::shared_ptr<ObjectAttributesMemory> m_Oam{nullptr};

        // pallettes
        std::shared_ptr<LcdPalettesMemory> m_Palettes{nullptr};

        // LCD Control
        std::shared_ptr<LcdControl> m_LcdControl{nullptr};
        
        std::shared_ptr<InterruptManager> m_InterruptManager;
        PpuMode m_PpuMode;

        // output screen
        LcdScreen m_Screen{};

        // tick ppu one time 
        // async function recalled each dot
        PpuTaskManager m_TaskManager{};
        bool m_IsRendering = true;
        
        PpuTask _Render();
        PpuTask _RenderLine();
         
        PpuTask _OAMScan();

        PpuTask _DrawingPixels();
        PpuTask _HorizontalBlank(uint32_t dots);
        PpuTask _VerticalBlank();

        
    };
} // namespace GBE
