#include "LcdPalette.h"

namespace GBE
{
    void LcdPalette::Set(uint8_t byte)
    {
        m_Byte = byte;
        for (uint8_t i = 0; i < static_cast<uint8_t>(m_Colors.size()); i++)
        {
            uint8_t colorIndex = (byte >> (2 * i)) & 3;
            m_Colors[i] = static_cast<LcdColor>(colorIndex);
        }
    }
} // namespace GBE
