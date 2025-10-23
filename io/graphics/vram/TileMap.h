#pragma once

#include <cstdint>
#include <array>

#include "memory/MemoryArea.h"

namespace GBE
{
    constexpr uint8_t TILE_MAP_SIZE = 32;
    constexpr uint16_t TILE_MAP_VRAM_SIZE = TILE_MAP_SIZE * TILE_MAP_SIZE;

    class TileMap: public MemoryArea
    {
    public:
        TileMap();
        ~TileMap() {}

        uint8_t GetTile(uint8_t x, uint8_t y) const;
    private:
        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;

        std::array<uint8_t, TILE_MAP_VRAM_SIZE> m_Data{};
    };
} // namespace GBE
