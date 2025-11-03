#pragma once

#include <cstdint>
#include <array>
#include <memory>

#include "LcdPalette.h"

#include "memory/MemoryArea.h"
#include "memory/MemoryMap.h"

namespace GBE
{
    constexpr std::array<MemoryMap, 2> MMAP_LCD_CONTROL = {MemoryMap{0xFF40, 0xFF46}, MemoryMap{0xFF4A, 0xFF4B}};
    constexpr uint16_t LCD_CONTROL_SIZE = MMAP_LCD_CONTROL[0].GetSize() + MMAP_LCD_CONTROL[1].GetSize();
    constexpr uint32_t DMA_TRANSFER_DOTS = 640;

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

    class Memory;
    class ObjectAttributesMemory;

    class LcdControl : public MemoryArea
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

        inline uint8_t GetObjectSize() const
        {
            return GetControlFlag(LcdControlFlag::OBJ_SIZE)? 2: 1;
        }

        bool GetStatusFlag(LcdStatusFlag flag) const;

        // update LY. shoud be called each tick 
        bool UpdateLcdYCoordinate(uint8_t y);
        
        // update ppu mode. soule be called each tick
        bool UpdatePpuMode(PpuMode mode);


        // do the transfer of DMA from memory to oam memory
        void Tick(const Memory& memory, const std::shared_ptr<ObjectAttributesMemory>& oam, uint32_t dots);

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
        bool m_StartDMATransfer = true;
        uint32_t m_DMADots = 0; 

        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;
    };
} // namespace GBE

