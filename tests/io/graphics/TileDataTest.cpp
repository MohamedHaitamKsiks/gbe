#include "GBETestSuite.h"

#include "io/graphics/vram/TileData.h"

GBE_TEST_SUITE(TileDataTest)
{
    TEST_CASE("Test tile image")
    {
        // arrange
        GBE::TileData tile{};
        // tile data
        std::vector<uint8_t> tileData = { 0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x5E, 0x7E, 0x0A, 0x7C, 0x56, 0x38, 0x7C };
        // expected image
        std::vector<uint8_t> tileImage = {
            0, 2, 3, 3, 3, 3, 2, 0,
            0, 3, 0, 0, 0, 0, 3, 0,
            0, 3, 0, 0, 0, 0, 3, 0,
            0, 3, 0, 0, 0, 0, 3, 0,
            0, 3, 1, 3, 3, 3, 3, 0,
            0, 1, 1, 1, 3, 1, 3, 0,
            0, 3, 1, 3, 1, 3, 2, 0,
            0, 2, 3, 3, 3, 2, 0, 0
        };

        // act
        for (uint16_t i = 0; i < static_cast<uint16_t>(tileData.size()); i++)
            tile.Set(i, tileData[i]);

        // assert
        for (uint8_t y = 0; y < GBE::TILE_SIZE; y++)
        {
            for (uint8_t x = 0; x < GBE::TILE_SIZE; x++)
            {
                CHECK_EQ(
                    tileImage[y * GBE::TILE_SIZE + x], 
                    tile.GetPixel(x, y)
                );
            }
        }
    }
}