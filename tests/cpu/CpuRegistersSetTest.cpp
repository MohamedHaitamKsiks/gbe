#include "GBETestSuite.h"

#include <iostream>
#include "memory/IMemory.h"
#include "cpu/CpuRegistersSet.h"


GBE_TEST_SUITE(CpuRegisterSet)
{
    TEST_CASE("Reg8")
    {
        // arrange
        GBE::CpuRegistersSet regs{};

        // act
        regs.SetReg8(GBE::Reg8::A, 0x12);
        uint8_t a = regs.GetReg8(GBE::Reg8::A);
    
        // assert
        CHECK_EQ(a, static_cast<uint8_t>(0x12));
    }

    TEST_CASE("Reg16")
    {
        // arrange
        GBE::CpuRegistersSet regs{};

        // act
        regs.SetReg16(GBE::Reg16::BC, 0xABCD);
        uint8_t bc = regs.GetReg16(GBE::Reg16::BC);

        // assert
        CHECK_EQ(bc, static_cast<uint8_t>(0xABCD));
    }

    TEST_CASE("SetFlag")
    {
        // arrange
        GBE::CpuRegistersSet regs{};

        // act
        regs.SetFlag(GBE::CpuFlag::H, true);
        regs.SetFlag(GBE::CpuFlag::N, true);
        regs.SetFlag(GBE::CpuFlag::Z, true);
        regs.SetFlag(GBE::CpuFlag::H, false);

        uint8_t flags = regs.GetFlags();

        // assert
        CHECK_EQ(flags, static_cast<uint8_t>(GBE::CpuFlag::N) | static_cast<uint8_t>(GBE::CpuFlag::Z));
    }

    TEST_CASE("SetFlags")
    {
        // arrange
        GBE::CpuRegistersSet regs{};

        // act
        regs.SetFlag(GBE::CpuFlag::N, true);
        regs.SetFlags(GBE::CpuFlag::H | GBE::CpuFlag::N, GBE::CpuFlag::H);
        uint8_t flags = regs.GetFlags();

        // assert
        CHECK_EQ(flags, static_cast<uint8_t>(GBE::CpuFlag::H));
    }
}