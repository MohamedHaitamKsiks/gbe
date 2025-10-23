#pragma once

#include <cstdint>
#include <array>

#include "LcdPalette.h"

#include "memory/MemoryArea.h"
#include "memory/MemoryMap.h"

namespace GBE
{
    constexpr MemoryMap MMAP_LCD_CONTROL(0xFF40, 0xFF4B);
    constexpr uint16_t LCD_CONTROL_SIZE = MMAP_LCD_CONTROL.GetSize();

    enum class LcdControlFlag
    {
        BG_WINDOW_ENABLE = 0,
        OBJ_ENABLE = 1,
        OBJ_SIZE = 2,
        BG_TILE_MAP = 3,
        BG_WINDOW_TILES = 4,
        WINDOW_ENABLE = 5,
        WINDOW_TILE_MAP = 6,
        LCD_PPU_ENABLE = 7
    };

    enum class LcdStatusFlag
    {
        LYC = 6,
        MODE_2 = 5,
        MODE_1 = 4,
        MODE_0 = 3,
        LYC_EQ_LY = 2
    };

    enum class PpuMode;

    class LcdControl: public MemoryArea
    {
    public:
        LcdControl();
        ~LcdControl() {}

        bool GetControlFlag(LcdControlFlag flag) const;

        uint8_t GetBackgroundTileMapID() const;

        uint8_t GetWindowTileMapID() const;

        inline uint8_t GetViewportX() const
        {
            return m_ViewportX;
        }

        inline uint8_t GetViewportY() const
        {
            return m_ViewportY;
        }

        inline uint8_t GetWindowX() const
        {
            return m_WindowX;
        }

        inline uint8_t GetWindowY() const
        {
            return m_WindowY;
        }

        uint16_t GetDMATransferAddress() const;

        inline uint8_t GetLcdYCoordinate() const 
        {
            return m_LcdYCoordinate;
        }

        inline uint8_t GetLcdYCompare() const
        {
            return m_LcdYCompare;
        }

        bool GetStatusFlag(LcdStatusFlag flag) const;

        // update LY. shoud be called each tick 
        void UpdateLcdYCoordinate(uint8_t y);
        
        // update ppu mode. soule be called each tick
        void UpdatePpuMode(PpuMode mode);

    private:
        uint8_t m_Control = 0;
        uint8_t m_Status = 0;

        uint8_t m_ViewportX = 0;
        uint8_t m_ViewportY = 0;

        uint8_t m_WindowX = 0;
        uint8_t m_WindowY = 0;

        uint8_t m_LcdYCoordinate = 0;
        uint8_t m_LcdYCompare = 0;

        uint8_t m_DMA = 0;

        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;
    };
} // namespace GBE

