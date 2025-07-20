#include "GBETestSuite.h"

#include "io/graphics/lcd/LcdPalette.h"

GBE_TEST_SUITE(LcdPalette)
{
    GBE::LcdPalette palette{};

    TEST_CASE("GetColor")
    {
        // arrange
        palette.Set(0x39); // 00 11 10 01
        std::array<GBE::LcdColor, 4> expectedColors = {
            GBE::LcdColor::LIGHT_GRAY,
            GBE::LcdColor::DARK_GRAY,
            GBE::LcdColor::BLACK,
            GBE::LcdColor::WHITE
        };

        // act & assery
        for (uint8_t i = 0; i < 4; i++)
        {   
            // act
            GBE::LcdColor color = palette.GetColor(i);
            
            // assert
            CHECK_EQ(
                color,
                expectedColors.at(i)
            );
        }
    }
}