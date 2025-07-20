#include "TileData.h"
#include "TileMap.h"

namespace GBE
{
    uint8_t TileMap::GetTile(uint8_t x, uint8_t y) const
    {
        return m_Data[x + y * TILE_MAP_SIZE];
    }
} // namespace GBE
