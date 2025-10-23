#include "TileData.h"
#include "TileMap.h"

namespace GBE
{
    TileMap::TileMap()
    {
        SetReadWriteFlags(true);
    }

    uint8_t TileMap::GetTile(uint8_t x, uint8_t y) const
    {
        return m_Data[x + y * TILE_MAP_SIZE];
    }
    
    void TileMap::_SetImp(uint16_t address, uint8_t value)
    {
        m_Data[address] = value;
    }
    
    uint8_t TileMap::_GetImp(uint16_t address) const
    {
        return m_Data.at(address);
    }
} // namespace GBE
