#include "GBETestSuite.h"

#include "cpu/CpuRegister.h"

GBE_TEST_SUITE(CpuRegister)
{
    TEST_CASE("SetLow")
    {
        // arrange
        GBE::CpuRegister reg{};

        // act
        reg.SetHigh(0xCD);

        // assert
        CHECK_EQ(reg.GetHigh(), 0xCD);
    };

    TEST_CASE("SetHigh")
    {
        // arrange
        GBE::CpuRegister reg{};

        // act
        reg.SetLow(0xCD);

        // assert
        CHECK_EQ(reg.GetLow(), 0xCD);
    };

    TEST_CASE("Set16")
    {
        // arrange 
        GBE::CpuRegister reg{};

        // act
        reg.Set16(0xABCD);

        // assert
        CHECK_EQ(reg.GetLow(), 0xCD);
        CHECK_EQ(reg.GetHigh(), 0xAB);
    };

    TEST_CASE("Get16")
    {
        // arrange
        GBE::CpuRegister reg{};
        reg.SetLow(0xCD);
        reg.SetHigh(0xAB);
        
        // act
        uint16_t regValue16 = reg.Get16();

        // assert
        CHECK_EQ(regValue16, 0xABCD);
    };
}
