#include "Ppu.h"

#include <iostream>
#include <magic_enum.hpp>

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
        m_DotsCounter = 0;
        m_WaitDots = 0;
        m_QueuePpuMode = PpuMode::OAM_SCAN;

        m_BackgroundFIFO.Clear();
    }

    void Ppu::Tick(uint32_t dots)
    {
        if (!m_LcdControl->GetControlFlag(LcdControlFlag::LCD_PPU_ENABLE))
            return;
        for (uint32_t i = 0; i < dots; i++)
        {
            if (m_WaitDots > 0)
                m_WaitDots--;

            if (m_WaitDots > 0)
                continue;
            
            _Render();
            m_LcdControl->UpdatePpuMode(m_PpuMode);
            m_LcdControl->UpdateLcdYCoordinate(m_LcdY);
        }
    }



    void Ppu::_Render()
    {
        m_PpuMode = m_QueuePpuMode;
        switch (m_PpuMode)
        {
        case PpuMode::OAM_SCAN:
            _OAMScan();
            break;
        case PpuMode::DRAW_PIXELS:
            _DrawingPixels();
            break;
        case PpuMode::H_BLANK:
            _HorizontalBlank();
            break;
        case PpuMode::V_BLANK:
            _VerticalBlank();
            break;
        default:
            break;
        }
    }

    void Ppu::_OAMScan()
    {
        m_LcdX = 0;
        m_TileIndex = ((m_LcdY + 32) / TILE_SIZE) * TILE_MAP_SIZE;
        m_BackgroundFIFO.Clear();

        m_Vram->SetReadWriteFlags(true);
        m_Palettes->SetReadWriteFlags(true);
        m_Oam->SetReadWriteFlags(false);
        
        m_WaitDots = OAM_SCAN_DOTS;
        m_QueuePpuMode = PpuMode::DRAW_PIXELS;
    }

    void Ppu::_DrawingPixels()
    {
        m_Vram->SetReadWriteFlags(false);
        m_Palettes->SetReadWriteFlags(false);
        m_Oam->SetReadWriteFlags(false);
        
        
        // _DrawPixel();
        while (m_BackgroundFIFO.CanPush())
            _Fetch();

        if (m_BackgroundFIFO.CanPopFront())
            _DrawPixel();


        if (m_LcdX >= LCD_SCREEN_WIDTH)
        {
            m_QueuePpuMode = PpuMode::H_BLANK;
            m_HBlankWaitDots = 204; // fuck ?
        }
    }

    void Ppu::_HorizontalBlank()
    {
        m_Vram->SetReadWriteFlags(true);
        m_Palettes->SetReadWriteFlags(true);
        m_Oam->SetReadWriteFlags(true);

        m_WaitDots = m_HBlankWaitDots;
        m_QueuePpuMode = PpuMode::OAM_SCAN;

        m_LcdY++;
        m_TileY = (m_TileY + 1) % TILE_SIZE;
        if (m_LcdY >= LCD_SCREEN_HEIGHT)
        {
            m_QueuePpuMode = PpuMode::V_BLANK;
        }
    }
    
    void Ppu::_VerticalBlank()
    {
        m_InterruptManager->QueueInterrupt(InterruptFlag::V_BLANK);

        m_Vram->SetReadWriteFlags(true);
        m_Palettes->SetReadWriteFlags(true);
        m_Oam->SetReadWriteFlags(true);

        m_WaitDots = V_BLANK_DOTS;
        m_QueuePpuMode = PpuMode::OAM_SCAN;
        m_LcdY = 0;
    }

    void Ppu::_DrawPixel()
    {
        uint8_t color = m_BackgroundFIFO.PopFront();
        m_Screen.SetPixel(m_LcdX, m_LcdY, color);
        m_LcdX++;
        m_WaitDots = 1;
    }

    void Ppu::_Fetch()
    {
        const auto bgMapID = m_LcdControl->GetBackgroundTileMapID();
        const auto &bgMap = m_Vram->GetTileMap(bgMapID);
        uint8_t tileID = bgMap.Get(m_TileIndex);

        const auto &tile = m_Vram->GetTileBGWin(tileID, true);

        uint8_t y = m_TileY;
        for (uint8_t x = 0; x < TILE_SIZE; x++)
        {
            uint8_t color = tile.GetPixel(x, y);
            m_BackgroundFIFO.PushBack(color);
        }

        m_TileIndex++;
    }

} // namespace GBE
