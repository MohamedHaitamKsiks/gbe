#include "LcdPalettesMemory.h"

namespace GBE
{
    enum class LcdPaletteAddress
    {
        BGP = 0,
        OBP0 = 1,
        OBP1 = 2
    };

    void LcdPalettesMemory::_SetImp(uint16_t address, uint8_t value)
    {
        switch (static_cast<LcdPaletteAddress>(address))
        {
        case LcdPaletteAddress::BGP :
            m_BackgroundPalette.Set(value);
            break;
        case LcdPaletteAddress::OBP0 :
            m_ObjectPalettes[0].Set(value);
            break;
        case LcdPaletteAddress::OBP1:
            m_ObjectPalettes[1].Set(value);
            break;
        default:
            break;
        }
    }

    uint8_t LcdPalettesMemory::_GetImp(uint16_t address) const
    {
        switch (static_cast<LcdPaletteAddress>(address))
        {
        case LcdPaletteAddress::BGP:
            return m_BackgroundPalette.Get();
        case LcdPaletteAddress::OBP0:
            return m_ObjectPalettes[0].Get();
        case LcdPaletteAddress::OBP1:
            return m_ObjectPalettes[1].Get();
        default:
            break;
        }
        return 0xFF;
    }

} // namespace GBE

