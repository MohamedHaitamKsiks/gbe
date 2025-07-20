#pragma once

#include <cstdint>
#include <array>
#include <vector>

namespace GBE
{
    constexpr uint8_t TILE_SIZE = 8;
    constexpr uint8_t TILE_VRAM_SIZE = TILE_SIZE * 2;

    // 8x8 tile data
    class TileData
    {
    public:
        ~TileData() {}

        inline void Set(uint16_t address, uint8_t value) 
        {
            m_Data[address] = value;
        }

        inline uint8_t Get(uint16_t address) const
        {
            return m_Data.at(address);
        }

        uint8_t GetPixel(uint8_t x, uint8_t y) const;

    private:
        std::array<uint8_t, TILE_VRAM_SIZE> m_Data{};
    };
} // namespace GBE
