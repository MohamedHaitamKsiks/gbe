#include "Vram.h"

#include "cpu/AluResult.h"
#include "cpu/Alu.h"

#include <iostream>

namespace GBE
{
    Vram::Vram()
    {
        // create vram with 
        m_Maps.resize(TILE_MAP_COUNT);
        m_Tiles.resize(TILE_COUNT);
        
        SetReadWriteFlags(true);

    }

    void Vram::_SetImp(uint16_t address, uint8_t value)
    {
        // 0x1800-0x1FFF
        if (address >= TILE_MAP_VRAM_ADDRESS)
        {
            uint16_t mapIndex = (address - TILE_MAP_VRAM_ADDRESS) / TILE_MAP_VRAM_SIZE;
            uint16_t mapLocalAddress = (address - TILE_MAP_VRAM_ADDRESS) % TILE_MAP_VRAM_SIZE;

            TileMap& map = m_Maps.at(mapIndex);
            map.Set(mapLocalAddress, value);

            return;
        }

        // $0000–17FF
        uint16_t tileIndex = address / TILE_VRAM_SIZE;
        uint16_t tileLocalAddress = address % TILE_VRAM_SIZE;

        TileData& tile = m_Tiles.at(tileIndex);
        tile.Set(tileLocalAddress, value);
    }

    uint8_t Vram::_GetImp(uint16_t address) const
    {
        // 0x1800-0x1FFF
        if (address >= TILE_MAP_VRAM_ADDRESS)
        {
            uint16_t mapIndex = (address - TILE_MAP_VRAM_ADDRESS) / TILE_MAP_VRAM_SIZE;
            uint16_t mapLocalAddress = (address - TILE_MAP_VRAM_ADDRESS) % TILE_MAP_VRAM_SIZE;

            const TileMap &map = m_Maps.at(mapIndex);

            return map.Get(mapLocalAddress);
        }

        // $0000–17FF
        uint16_t tileIndex = address / TILE_VRAM_SIZE;
        uint16_t tileLocalAddress = address % TILE_VRAM_SIZE;

        const TileData &tile = m_Tiles.at(tileIndex);
        return tile.Get(tileLocalAddress);
    }

    const TileData &Vram::GetTileBGWin(uint8_t tileID, bool objetAddressMode) const
    {
        if (objetAddressMode)
            return m_Tiles.at(tileID);

        int8_t signedTileID = tileID;
        return m_Tiles.at((2 * TILE_BLOCK_COUNT) + static_cast<int16_t>(signedTileID));
    }
} // namespace GBE
