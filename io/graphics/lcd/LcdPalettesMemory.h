#pragma once

#include <cstdint>
#include <array>

#include "LcdPalette.h"

#include "memory/MemoryMap.h"
#include "memory/MemoryArea.h"

namespace GBE
{

    constexpr MemoryMap MMAP_LCD_PALETTES = {0xFF47, 0xFF49};

    class LcdPalettesMemory: public MemoryArea
    {   
    public: 
        ~LcdPalettesMemory(){}

        inline const LcdPalette &GetBackgroundPalette() const
        {
            return m_BackgroundPalette;
        }

        inline const LcdPalette &GetObjectPalette(uint8_t paletteID) const
        {
            return m_ObjectPalettes.at(paletteID);
        }

    private:
        LcdPalette m_BackgroundPalette{};
        std::array<LcdPalette, 2> m_ObjectPalettes{};

        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;
    };
} // namespace GBE
