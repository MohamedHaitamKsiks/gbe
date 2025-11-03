#include "PixelFIFO.h"

#include <cassert>

namespace GBE
{
    void PixelFIFO::PushBack(uint8_t pixel)
    {
        assert(m_CurrentSize < PIXEL_FIFO_SIZE);

        m_Pixels[m_PushIndex] = pixel;
        m_PushIndex = (m_PushIndex + 1) % PIXEL_FIFO_SIZE;

        m_CurrentSize++;
    }

    uint8_t PixelFIFO::PopFront()
    {
        // assert(CanPopFront());

        uint8_t pixel = m_Pixels[m_PopIndex];
        m_PopIndex = (m_PopIndex + 1) % PIXEL_FIFO_SIZE;

        m_CurrentSize--;    
        return pixel;
    }

    void PixelFIFO::Resize(uint32_t size, uint8_t pixel)
    {
        while (GetCurrentSize() < size)
        {
            PushBack(pixel);
        }
    }

    void PixelFIFO::Clear()
    {
        m_CurrentSize = 0;
        m_PopIndex = 0;
        m_PushIndex = 0;
    }

} // namespace GBE
