#include "TileData.h"

#include <cassert>

#include "util/Binary.h"

namespace GBE
{
    uint8_t TileData::GetPixel(uint8_t x, uint8_t y) const
    {
        // get line data
        uint16_t lineAddress = static_cast<uint16_t>(y) * 2;
        uint8_t hight = m_Data[lineAddress + 1];
        uint8_t low = m_Data[lineAddress];

        // get x bit index
        uint8_t xBit = TILE_SIZE - x - 1;

        // get color value
        uint8_t colorHight = Binary::TestBit(hight, xBit) ? 2 : 0;
        uint8_t colorLow = Binary::TestBit(low, xBit) ? 1 : 0;
        uint8_t color = colorHight + colorLow;

        return color;
    }
} // namespace GBE
