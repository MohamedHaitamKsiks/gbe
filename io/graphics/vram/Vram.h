#pragma once

#include <cstdint>
#include <vector>

#include "TileMap.h"
#include "TileData.h"

#include "memory/MemoryMap.h"

namespace GBE
{
    constexpr uint16_t TILE_BLOCK_COUNT = 128;
    constexpr uint16_t TILE_COUNT = TILE_BLOCK_COUNT * 3;
    constexpr uint16_t TILE_MAP_COUNT = 2;
    
    // local address in vram of tile maps
    constexpr uint16_t TILE_MAP_VRAM_ADDRESS = 0x1800;

    // VRAM Memory map 
    static constexpr MemoryMap MMAP_VRAM(0x8000, 0x9FFF);

    // video ram storing tile set and tile map
    // can read and write from like normal ram as long as it is accessible
    // 384 on 3 blocks with 128 tile 
    class Vram
    {
    public:
        Vram();
        ~Vram() {}

        // set byte at vrame 
        void Set(uint16_t address, uint8_t value);

        // get byte from vram
        uint8_t Get(uint16_t address) const;

        // set accessible flag
        inline void SetAccessible(bool isAccessible)
        {
            m_isAccessible = isAccessible;
        }

        // is accessible
        inline bool IsAccessible() const
        {
            return m_isAccessible;
        }

        // Get tile map
        inline const TileMap& GetTileMap(uint8_t tileMapID) const  
        {
            return m_Maps.at(tileMapID);
        }
        
        // get object (always from block 0 to 1)
        inline const TileData& GetObject(uint8_t objectID) const
        {
            return m_Tiles.at(objectID);
        }

        // get tile id for background / window
        // if object address mode is set to true => start from block 0
        // else start from block 1
        const TileData& GetTileBGWin(uint8_t tileID, bool objetAddressMode) const;
    private:
        // 8 KiB Video Ram
        bool m_isAccessible = false;
        std::vector<TileData> m_Tiles{};
        std::vector<TileMap> m_Maps{};
        
    };
} // namespace GBE
