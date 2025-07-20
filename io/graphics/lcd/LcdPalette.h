#pragma once

#include <cstdint>
#include <array>

namespace GBE
{
    enum class LcdColor
    {
        WHITE = 0,
        LIGHT_GRAY = 1,
        DARK_GRAY = 2,
        BLACK = 3
    };

    class LcdPalette
    {
    public:
        ~LcdPalette() {}

        void Set(uint8_t byte);

        inline uint8_t Get() const
        {
            return m_Byte;
        }

        inline LcdColor GetColor(uint8_t colorIndex) const
        {
            return m_Colors.at(colorIndex);
        };
    private:
        uint8_t m_Byte = 0x0;
        std::array<LcdColor, 4> m_Colors{};
    };
} // namespace GBE
