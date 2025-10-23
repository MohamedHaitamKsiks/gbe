#pragma once

#include <cstdint>
#include <array>

namespace GBE
{
    constexpr uint8_t LCD_SCREEN_WIDTH = 160;
    constexpr uint8_t LCD_SCREEN_HEIGHT = 144;

    // container of lcd screen data;
    class LcdScreen
    {
    public:
        using Pixels = std::array<uint8_t, LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT>;

        ~LcdScreen() {}

        inline void SetPixel(uint8_t x, uint8_t y, uint8_t value)
        {
            m_Pixels[y * LCD_SCREEN_WIDTH + x] = value;
        }

        inline const Pixels& GetPixels() const
        {
            return m_Pixels;
        }

        inline uint8_t GetPixel(uint8_t x, uint8_t y) const
        {
            return m_Pixels[x + y * LCD_SCREEN_WIDTH];
        }
    private:
        Pixels m_Pixels{};
    };
} // namespace GBE
