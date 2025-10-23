#include "Ppu.h"

#include <iostream>

namespace GBE
{
    Ppu::Ppu(std::shared_ptr<InterruptManager> interruptManager)
    {
        m_Vram = std::make_shared<Vram>();
        m_Oam = std::make_shared<ObjectAttributesMemory>();
        m_Palettes = std::make_shared<LcdPalettesMemory>();
        m_LcdControl = std::make_shared<LcdControl>();
        m_InterruptManager = interruptManager;

    }

    Ppu::~Ppu()
    {
        m_IsRendering = false;
    }

    void Ppu::Init()
    {
        m_PpuMode = PpuMode::DRAW_PIXELS;
        m_TaskManager.Start([this]() -> PpuTask{
            return _Render();
        });
    }

    void Ppu::Tick(uint32_t dots)
    {
        for (uint32_t i = 0; i < dots; i++)
        {
            m_TaskManager.ProcessDot();
            /*m_LcdY++;
            if (m_LcdY > 70224 - 4560)
            {
                m_PpuMode = PpuMode::V_BLANK;
            }

            if (m_LcdY > 70224)
            {
                m_LcdY -= 70224;
                m_PpuMode = PpuMode::DRAW_PIXELS;
            }*/


            m_LcdControl->UpdatePpuMode(m_PpuMode);
            m_LcdControl->UpdateLcdYCoordinate(m_LcdY);
        }
    }



    PpuTask Ppu::_Render()
    {
        while (m_IsRendering)
        {
            m_LcdY = 0;

            for (uint32_t i = 0; i < LCD_SCREEN_HEIGHT; i++)
            {
                co_yield _RenderLine();
                m_LcdY++;
                m_LcdControl->UpdateLcdYCoordinate(m_LcdY);
            }
            co_yield _VerticalBlank();

            m_FrameCounter++;
        }

        co_return;
    }

    PpuTask Ppu::_RenderLine()
    {
        uint32_t dots = 0;

        // OAM
        auto scanTask = _OAMScan();
        co_yield scanTask;
        dots += scanTask.promise().Dots;

        // DRAW PIXELS
        auto drawTask = _DrawingPixels();
        co_yield drawTask;
        dots += drawTask.promise().Dots;

        //H-BLANK
        uint32_t hBlankDots = RENDER_LINE_DOTS - dots;
        co_yield _HorizontalBlank(hBlankDots);
        
        co_return;
    }

    PpuTask Ppu::_OAMScan()
    {
        m_PpuMode = PpuMode::OAM_SCAN;

        /*
        m_Vram->SetReadWriteFlags(true);
        m_Palettes->SetReadWriteFlags(true);
        m_Oam->SetReadWriteFlags(false);
        */
        co_yield PpuTask::Wait(OAM_SCAN_DOTS);

        co_return;
    }

    PpuTask Ppu::_DrawingPixels()
    {
        m_PpuMode = PpuMode::DRAW_PIXELS;

        /*
        m_Vram->SetReadWriteFlags(false);
        m_Palettes->SetReadWriteFlags(false);
        m_Oam->SetReadWriteFlags(false);
        */

        const auto bgMapID = m_LcdControl->GetBackgroundTileMapID();
        const auto& bgMap = m_Vram->GetTileMap(0);

        m_LcdX = 0;
        for (uint32_t i = 0; i < LCD_SCREEN_WIDTH; i++)
        {
            co_yield nullptr;

            uint32_t tileX = m_LcdX / TILE_SIZE;
            uint32_t tileY = (m_LcdY / TILE_SIZE) + 4;
            uint8_t tileID = bgMap.GetTile(tileX, tileY);
            const auto &tile = m_Vram->GetTileBGWin(tileID, true) ;// (tileX + tileY * (LCD_SCREEN_WIDTH / TILE_SIZE)) % TILE_MAP_VRAM_SIZE, true);

            uint8_t color = tile.GetPixel(m_LcdX % TILE_SIZE, m_LcdY % TILE_SIZE);
            m_Screen.SetPixel(m_LcdX, m_LcdY, color % 4);
            m_LcdX++;
        }

        co_return;
    }

    PpuTask Ppu::_HorizontalBlank(uint32_t dots)
    {
        m_PpuMode = PpuMode::H_BLANK;

        m_Vram->SetReadWriteFlags(true);
        m_Palettes->SetReadWriteFlags(true);
        m_Oam->SetReadWriteFlags(true);

        co_yield PpuTask::Wait(dots);
        co_return;
    }

    PpuTask Ppu::_VerticalBlank()
    {
        m_PpuMode = PpuMode::V_BLANK;
        // m_InterruptManager->QueueInterrupt(InterruptFlag::LCD);
        m_InterruptManager->QueueInterrupt(InterruptFlag::V_BLANK);
        // m_VBlankSignal.Emit(m_Screen);

        m_Vram->SetReadWriteFlags(true);
        m_Palettes->SetReadWriteFlags(true);
        m_Oam->SetReadWriteFlags(true);

        co_yield PpuTask::Wait(V_BLANK_DOTS);
        co_return;
    }

} // namespace GBE
