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
            m_LineDotsCounter++;
            if (m_WaitDots > 0)
                m_WaitDots--;

            if (m_WaitDots > 0)
                continue;
            
            _Render();
            
            // update lcd control and manage interrupts  
            bool intPpuMode = m_LcdControl->UpdatePpuMode(m_PpuMode);
            bool intLYC = m_LcdControl->UpdateLcdYCoordinate(m_LcdY);

            if (intLYC || intPpuMode)
            {
                m_InterruptManager->QueueInterrupt(InterruptFlag::LCD);
            }
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
        m_IsOnWindow = false;

        // viewport
        uint32_t positionX = m_LcdX + m_LcdControl->GetViewportX();
        uint32_t positionY = m_LcdY + m_LcdControl->GetViewportY();
        
        m_TileX = positionX / TILE_SIZE;
        m_TileY = positionY / TILE_SIZE;

        m_TileOffsetY = positionY % TILE_SIZE;
        m_TileOffsetX = positionX % TILE_SIZE;

        m_BackgroundFIFO.Clear();

        // rw flags
        m_Palettes->SetReadWriteFlags(true);
        m_Oam->SetReadWriteFlags(false);
        m_Vram->SetReadWriteFlags(true);
        
        // fetch objects
        m_LineObjects.clear();
        for (uint8_t objectID = 0; objectID < OBJECT_COUNT; objectID++)
        {
            const auto& obj = m_Oam->GetObject(objectID);
            uint8_t objY = obj.GetYPosition();
            uint8_t objSize = TILE_SIZE * m_LcdControl->GetObjectSize();
            const uint8_t maxObjSize = 2 * TILE_SIZE;

            if (m_LcdY + maxObjSize >= objY && m_LcdY + maxObjSize < objY + objSize)
            {
                m_LineObjects.push_back(objectID);
            }
        }

        // queue next mode
        m_WaitDots = OAM_SCAN_DOTS;
        m_QueuePpuMode = PpuMode::DRAW_PIXELS;
        m_LineDotsCounter = 0;
    }

    void Ppu::_DrawingPixels()
    {
        m_Vram->SetReadWriteFlags(false);
        m_Palettes->SetReadWriteFlags(false);
        m_Oam->SetReadWriteFlags(false);
        
        // check window
        if (!m_IsOnWindow 
            && m_LcdControl->GetControlFlag(LcdControlFlag::WINDOW_ENABLE)
            && m_LcdX + 7 >= m_LcdControl->GetWindowX()
            && m_LcdY >= m_LcdControl->GetWindowY()
        )
        {
            m_TileOffsetY = m_WindowInternalY % TILE_SIZE;

            m_TileX = 0;
            m_TileY = m_WindowInternalY / TILE_SIZE;

            m_BackgroundFIFO.Clear();
            m_IsOnWindow = true;
        }
        

        while (m_BackgroundFIFO.CanPush())
            _FetchBackgroundFIFO();

        _FetchObjectsFIFO();

        if (m_BackgroundFIFO.CanPopFront())
            _DrawPixel();

        if (m_LcdX >= LCD_SCREEN_WIDTH)
        {
            m_QueuePpuMode = PpuMode::H_BLANK;
            m_HBlankWaitDots = RENDER_LINE_DOTS - m_LineDotsCounter;
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
        if (m_IsOnWindow)
            m_WindowInternalY++;

        if (m_LcdY >= LCD_SCREEN_HEIGHT)
        {
            m_QueuePpuMode = PpuMode::V_BLANK;
            m_VBlankInterrupt = true;
            m_LineDotsCounter = 0;
        }
    }
    
    void Ppu::_VerticalBlank()
    {
        if (m_VBlankInterrupt)
        {
            m_InterruptManager->QueueInterrupt(InterruptFlag::V_BLANK);
            m_WindowInternalY = 0;

            m_Vram->SetReadWriteFlags(true);
            m_Palettes->SetReadWriteFlags(true);
            m_Oam->SetReadWriteFlags(true);
        }
        else
        {
            m_LcdY++;
        }

        if (m_LcdY >= LCD_SCREEN_FULL_HEIGHT)
        {
            m_QueuePpuMode = PpuMode::OAM_SCAN;
            m_LcdY = 0;
        }
        else
        {
            m_WaitDots = RENDER_LINE_DOTS;
            m_QueuePpuMode = PpuMode::V_BLANK;
            m_VBlankInterrupt = false;
        }
    }

    void Ppu::_DrawPixel()
    {
        uint8_t color = 0;
        uint8_t backgroundColor = 0;

        // bg window 
        if (m_LcdControl->GetControlFlag(LcdControlFlag::BG_WINDOW_ENABLE))
        {
            while(m_TileOffsetX > 0)
            {
                m_BackgroundFIFO.PopFront();
                m_TileOffsetX--;
            }
    
            backgroundColor = m_BackgroundFIFO.PopFront();
        }

        // object
        uint8_t objectColor = 0;
        if (m_LcdControl->GetControlFlag(LcdControlFlag::OBJ_ENABLE))
        {
            if (m_ObjectFIFO.GetCurrentSize() > 0)
                objectColor = m_ObjectFIFO.PopFront();
        }

        // color mixing
        color = backgroundColor;
        if (objectColor)
            color = objectColor;

        m_Screen.SetPixel(m_LcdX, m_LcdY, color);
        m_LcdX++;
        m_WaitDots = 1;
    }

    void Ppu::_FetchBackgroundFIFO()
    {
        // fetch background/window
        uint8_t bgMapID = m_LcdControl->GetBackgroundTileMapID();
        if (m_IsOnWindow)
        {
            bgMapID = m_LcdControl->GetWindowTileMapID();
        }

        const TileMap &bgMap = m_Vram->GetTileMap(bgMapID);

        uint32_t tileIndex = (m_TileX % TILE_MAP_SIZE + m_TileY * TILE_MAP_SIZE) % TILE_MAP_VRAM_SIZE;
        uint8_t tileID = bgMap.Get(tileIndex);

        bool addressMode = m_LcdControl->GetControlFlag(LcdControlFlag::BG_WINDOW_TILES);
        const auto &tile = m_Vram->GetTileBGWin(tileID, addressMode);

        uint8_t y = m_TileOffsetY;
        for (uint8_t x = 0; x < TILE_SIZE; x++)
        {
            uint8_t colorIndex = tile.GetPixel(x, y);

            // apply color pallette
            const auto &bgPalette = m_Palettes->GetBackgroundPalette();
            uint8_t color = static_cast<uint8_t>(bgPalette.GetColor(colorIndex));

            m_BackgroundFIFO.PushBack(color);
        }

        m_TileX++;
    }

    void Ppu::_FetchObjectsFIFO()
    {
        if (!m_LcdControl->GetControlFlag(LcdControlFlag::OBJ_ENABLE))
        {
            return;
        }

        for (auto objectID: m_LineObjects)
        {
            // check obj pos
            const auto& obj = m_Oam->GetObject(objectID);
            if (obj.GetXPosition() != m_LcdX + TILE_SIZE)
                continue;

            // get obj y info
            const uint8_t objSize = m_LcdControl->GetObjectSize() * TILE_SIZE;

            uint8_t y = (m_LcdY + (2 * TILE_SIZE) - obj.GetYPosition());
            if (obj.GetYFlip())
                y = objSize - 1 - y;

            // get obj til id
            uint8_t objTileID = obj.GetTileIndex();
            if (objSize > TILE_SIZE)
            {
                uint8_t mask = 1;
                objTileID = (objTileID & ~mask);
            }

            if (y >= TILE_SIZE)
                objTileID++;

            //
            const auto& tile = m_Vram->GetObject(objTileID);
            uint8_t paletteID = obj.GetDMGPalette();

            // copy obj to object fifo
            m_ObjectFIFO.Resize(TILE_SIZE, 0);

            for (uint8_t x = 0; x < TILE_SIZE; x++)
            {
                if (m_ObjectFIFO.Get(x) != 0)
                    continue;
                
                uint8_t localX = x;
                if (obj.GetXFlip())
                    localX = TILE_SIZE - localX - 1;
                
                uint8_t localY = y % TILE_SIZE;
                // apply color pallette
                uint8_t colorIndex = tile.GetPixel(localX, localY);

                const auto &objPalette = m_Palettes->GetObjectPalette(paletteID);
                uint8_t color = static_cast<uint8_t>(objPalette.GetColor(colorIndex));

                m_ObjectFIFO.Set(x, color);
            }

            return;
        }
    }
} // namespace GBE
