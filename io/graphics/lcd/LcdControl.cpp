#include "LcdControl.h"
#include "io/graphics/Ppu.h"

#include "util/Binary.h"

namespace GBE
{
    enum class LcdAddress
    {
        // LCD
        LCDC = 0x0, // Control
        STAT = 0x1, // Status
        SCY = 0x2,  // Viewport Y
        SCX = 0x3,  // Viewport X
        LY = 0x4,   // Y coordinate
        LYC = 0x5,  // LY compare
        DMA = 0x6,  // OAM DMA

        // Palettes (DMG)
        BGP = 0x7,  // BG palette
        OBP0 = 0x8, // OBJ palette 0
        OBP1 = 0x9, // OBJ palette 1

        // Window
        WY = 0xA, // Y position
        WX = 0xB, // X position + 7
    };

    constexpr uint8_t LYC_WRITABLE_MASK = 0b01111000; 

    void LcdControl::Set(uint16_t address, uint8_t value)
    {
        switch (static_cast<LcdAddress>(address))
        {
        case LcdAddress::LCDC:
            m_Control = value;
            break;
        case LcdAddress::STAT:
            m_Status = value;
            break;
        case LcdAddress::SCY:
            m_ViewportY = value;
            break;
        case LcdAddress::SCX:
            m_ViewportX = value;
            break;
        // READ ONLY //
        case LcdAddress::LY:
            break;
        // BITS 0 - 2 READ ONLY //
        case LcdAddress::LYC:
            m_LcdYCompare = (value & LYC_WRITABLE_MASK) | m_LcdYCompare; 
            break;
        case LcdAddress::DMA:
            m_DMA = value;
            break;
        case LcdAddress::BGP:
            m_BackgroundPalette.Set(value);
            break;
        case LcdAddress::OBP0:
            m_ObjectPalettes[0].Set(value);
            break;
        case LcdAddress::OBP1:
            m_ObjectPalettes[1].Set(value);
            break;
        case LcdAddress::WY:
            m_WindowY = value;
            break;
        case LcdAddress::WX:
            m_WindowX = value;
            break;
        default:
            break;
        }
    }
    
    uint8_t LcdControl::Get(uint16_t address) const
    {
        switch (static_cast<LcdAddress>(address))
        {
        case LcdAddress::LCDC:
            return m_Control;
        case LcdAddress::STAT:
            return m_Status;
        case LcdAddress::SCY:
            return m_ViewportY;
        case LcdAddress::SCX:
            return m_ViewportX;
        case LcdAddress::LY:
            return m_LcdYCoordinate;
        case LcdAddress::LYC:
            return m_LcdYCompare;
        case LcdAddress::DMA:
            return m_DMA;
        case LcdAddress::BGP:
            return m_BackgroundPalette.Get();
        case LcdAddress::OBP0:
            return m_ObjectPalettes[0].Get();
        case LcdAddress::OBP1:
            return m_ObjectPalettes[1].Get();
        case LcdAddress::WY:
            return m_WindowY;
        case LcdAddress::WX:
            return m_WindowX;
        default:
            return 0xFF;
        }
    }

    bool LcdControl::GetControlFlag(LcdControlFlag flag) const
    {
        return Binary::TestBit(m_Control, static_cast<uint8_t>(flag));
    }

    uint8_t LcdControl::GetBackgroundTileMapID() const
    {
        return GetControlFlag(LcdControlFlag::BG_TILE_MAP) ? 1 : 0;
    }

    uint8_t LcdControl::GetWindowTileMapID() const
    {
        return GetControlFlag(LcdControlFlag::WINDOW_TILE_MAP) ? 1 : 0;
    }
    
    uint16_t LcdControl::GetDMATransferAddress() const
    {
        return static_cast<uint16_t>(m_DMA) << 8;
    }

    bool LcdControl::GetStatusFlag(LcdStatusFlag flag) const
    {
        return Binary::TestBit(m_Status, static_cast<uint8_t>(flag));
    }

    void LcdControl::UpdateLcdYCoordinate(uint8_t y)
    {
        m_LcdYCoordinate = y;

        // LYC == LY
        if (m_LcdYCoordinate == m_LcdYCompare)
            Binary::SetBit(m_Status, static_cast<uint8_t>(LcdStatusFlag::LYC_EQ_LY));
        else
            Binary::ResetBit(m_Status, static_cast<uint8_t>(LcdStatusFlag::LYC_EQ_LY));
    }

    void LcdControl::UpdatePpuMode(PpuMode mode)
    {
        uint8_t ppuModeInt2 = 0;
        constexpr uint8_t PPU_MODE_MASK = 0b11111100;

        if (GetControlFlag(LcdControlFlag::LCD_PPU_ENABLE))
            ppuModeInt2 = static_cast<uint8_t>(mode); // to bites

        m_Status = (m_Status & PPU_MODE_MASK) | ppuModeInt2;
    }
} // namespace GBE
