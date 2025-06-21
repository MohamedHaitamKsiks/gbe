#pragma once

#include <cstdint>
#include <vector>

#include "TileData.h"

namespace GBE
{
    class Vram
    {
    public:
        Vram();
        ~Vram() {}

        void Set(uint16_t address, uint8_t value);
        uint8_t Get(uint16_t address) const;

        

    private:
        // 8 KiB Video Ram
        bool m_isAccessible = false;
        std::vector<TileData> m_Tiles{};
        std::vector<uint8_t> m_TileMaps{};
        
    };
} // namespace GBE
