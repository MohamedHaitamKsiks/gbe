#include "Ppu.h"

#include <iostream>

namespace GBE
{
    Ppu::Ppu()
    {
    }

    Ppu::~Ppu()
    {
        m_IsRendering = false;
    }

    void Ppu::Init()
    {
        m_TaskManager.Start([this]() -> PpuTask{
            return _Render();
        });
    }

    void Ppu::Tick(uint32_t dots)
    {
        m_LcdControl.UpdateLcdYCoordinate(0);

        for (uint32_t i = 0; i < dots; i++)
        {
            m_TaskManager.ProcessDot();
            m_LcdControl.UpdatePpuMode(m_PpuMode);
        }
    }



    PpuTask Ppu::_Render()
    {
        while (m_IsRendering)
        {
            auto scanTask = _OAMScan();
            co_yield scanTask;
        }

        co_return;
    }

    PpuTask Ppu::_OAMScan()
    {
        co_yield PpuTask::Wait(10);
        co_return;
    }

    PpuTask Ppu::_DrawingPixels()
    {
        m_PpuMode = PpuMode::DRAW_PIXELS;
        m_Vram.SetAccessible(false);
        co_return;
    }

    PpuTask Ppu::_HorizontalBlank(uint32_t dots)
    {
        m_PpuMode = PpuMode::H_BLANK;
        co_yield PpuTask::Wait(dots);
        co_return;
    }

    PpuTask Ppu::_VerticalBlank()
    {
        m_PpuMode = PpuMode::V_BLANK;
        m_Vram.SetAccessible(true);

        co_yield PpuTask::Wait(V_BLANK_DOTS);
        co_return;
    }

} // namespace GBE
