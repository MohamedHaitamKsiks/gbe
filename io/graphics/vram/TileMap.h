#pragma once

#include <cstdint>
#include <array>

namespace GBE
{
    constexpr uint8_t TILE_MAP_SIZE = 32;
    constexpr uint16_t TILE_MAP_VRAM_SIZE = TILE_MAP_SIZE * TILE_MAP_SIZE;

    class TileMap
    {
    public:
        ~TileMap() {}

        inline void Set(uint16_t address, uint8_t value) 
        {
            m_Data[address] = value;
        }

        inline uint8_t Get(uint16_t address) const
        {
            return m_Data.at(address);
        }

        uint8_t GetTile(uint8_t x, uint8_t y) const;

    private:
        std::array<uint8_t, TILE_MAP_VRAM_SIZE> m_Data{};
    };
} // namespace GBE
