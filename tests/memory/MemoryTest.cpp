#include "GBETestSuite.h"

#include "memory/Memory.h"

namespace GBETest
{
    class TestMemoryArea: public GBE::MemoryArea
    {
    public:
        TestMemoryArea(size_t size, uint8_t defaultValue)
        {
            m_Data.resize(size, defaultValue);
            SetReadWriteFlags(true);
        };
        ~TestMemoryArea() = default;

    private:
        std::vector<uint8_t> m_Data{};

        uint8_t _GetImp(uint16_t address) const override
        {
            return m_Data.at(address);
        }

        void _SetImp(uint16_t address, uint8_t value) override
        {
            m_Data[address] = value;
        }
    };
} // namespace GBETest


GBE_TEST_SUITE(MemoryTest)
{
    GBE::Memory memory;
    TEST_CASE("Setup")
    {
        memory.MapMemoryArea({
                {0, 9},
            },
            std::make_shared<GBETest::TestMemoryArea>(10, 0)
        );

        memory.MapMemoryArea({
                {10, 14},
                {20, 25},
            },
            std::make_shared<GBETest::TestMemoryArea>(10, 1)
        );

        memory.MapMemoryArea({
                {15, 19}
            },
            std::make_shared<GBETest::TestMemoryArea>(5, 2)
        );
    }

    TEST_CASE("Get")
    {
        // assert
        CHECK_EQ(memory.Get(9), 0);
        CHECK_EQ(memory.Get(21), 1);
        CHECK_EQ(memory.Get(15), 2);
    }

    TEST_CASE("Set")
    {
        // arrange
        uint8_t valueToSet = 0xFF;
        uint16_t addressToSet = 16;

        // act
        memory.Set(addressToSet, valueToSet);

        // assert
        CHECK_EQ(memory.Get(addressToSet), valueToSet);
    }
}