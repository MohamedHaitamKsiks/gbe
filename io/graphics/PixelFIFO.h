#pragma once

#include <cstdint>
#include <array>

#include "vram/TileData.h"

namespace GBE
{
    constexpr uint8_t PIXEL_FIFO_SIZE = TILE_SIZE * 2;

    class PixelFIFO
    {
    public:
        ~PixelFIFO() {}

        void PushBack(uint8_t pixel);
        
        uint8_t PopFront();

        inline uint32_t GetCurrentSize() const
        {
            return m_CurrentSize;
        }

        inline bool CanPopFront() const
        {
            return m_CurrentSize > TILE_SIZE;
        }

        inline bool CanPush() const
        {
            return m_CurrentSize <= TILE_SIZE;
        }

        inline void Set(uint8_t index, uint8_t pixel)
        {
            m_Pixels[(m_PopIndex + index) % PIXEL_FIFO_SIZE] = pixel;
        }

        uint8_t Get(uint8_t index) const
        {
            return m_Pixels[(m_PopIndex + index) % PIXEL_FIFO_SIZE];
        }

        void Resize(uint32_t size, uint8_t pixel);

        void Clear();

    private:
        std::array<uint8_t, PIXEL_FIFO_SIZE> m_Pixels{};
        uint32_t m_PopIndex = 0;
        uint32_t m_PushIndex = 0;
        uint32_t m_CurrentSize = 0;

    };
} // namespace GBE
