#include "GBETestSuite.h"

#include "cpu/CpuFlags.h"
#include "cpu/Alu.h"
#include "cpu/AluResult.h"


GBE_TEST_SUITE(ALU)
{
    GBE::AluResult result{};

    TEST_CASE("Increment16")
    {
        // arrange
        uint16_t input = 16;

        // act
        GBE::Alu::Increment16(input, result);

        // assert
        CHECK_EQ(result.Result16, static_cast<uint16_t>(17));
    }

    TEST_CASE("Increment16 overflows")
    {
        // arrange
        uint16_t input = -1;

        // act
        GBE::Alu::Increment16(input, result);

        // assert
        CHECK_EQ(result.Result16, static_cast<uint16_t>(0));
    }

    TEST_CASE("Decrement16")
    {
        // arrange
        uint16_t input = 16;

        // act
        GBE::Alu::Decrement16(input, result);

        // assert
        CHECK_EQ(result.Result16, static_cast<uint16_t>(15));
    }

    TEST_CASE("Decrement16 underflows")
    {
        // arrange
        uint16_t input = 0;

        // act
        GBE::Alu::Decrement16(input, result);

        // assert
        CHECK_EQ(result.Result16, static_cast<uint16_t>(-1));
    }


    TEST_CASE("Add8")
    {
        // arrange
        uint8_t a = 1;
        uint8_t b = 1;

        // act
        GBE::Alu::Add8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(2));
        CHECK(!(result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
        CHECK(!(result.Flags & GBE::CpuFlag::Z));
    }


    TEST_CASE("Add8 Flag Z")
    {
        // arrange
        int8_t a = 12;
        int8_t b = -12;

        // act
        GBE::Alu::Add8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<int8_t>(0));
        CHECK((result.Flags & GBE::CpuFlag::Z));

    }


    TEST_CASE("Add8 Flag H")
    {
        // arrange
        uint8_t a = 12;
        uint8_t b = 15;

        // act
        GBE::Alu::Add8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(27));
        CHECK((result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
        CHECK(!(result.Flags & GBE::CpuFlag::Z));
    }

    TEST_CASE("Add8 Flag C")
    {
        // arrange
        uint8_t a = 224;
        uint8_t b = 96;

        // act
        GBE::Alu::Add8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(64));
        CHECK((result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("Add16")
    {
        // arrange
        uint16_t a = 300;
        uint16_t b = 400;

        // act
        GBE::Alu::Add16(a, b, result);

        // assert
        CHECK_EQ(result.Result16, static_cast<uint16_t>(700));
        CHECK(!(result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("Add16 Flag H")
    {
        // arrange
        uint16_t a = 3000;
        uint16_t b = 3000;

        // act
        GBE::Alu::Add16(a, b, result);

        // assert
        CHECK_EQ(result.Result16, static_cast<uint16_t>(6000));
        CHECK((result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("Add8 Flag C")
    {
        // arrange
        uint16_t a = 57344;
        uint16_t b = 40960;

        // act
        GBE::Alu::Add16(a, b, result);

        // assert
        CHECK_EQ(result.Result16, static_cast<uint16_t>(32768));
        CHECK(!(result.Flags & GBE::CpuFlag::H));
        CHECK((result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("Offset16")
    {
        // arrange
        uint16_t a = 5;
        int8_t e = 1;

        // act
        GBE::Alu::Offset16(a, e, result);

        // assert
        CHECK_EQ(result.Result16, 6);
        CHECK(!(result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("Offset16 Negative")
    {
        // arrange
        uint16_t a = 300;
        int8_t e = -50;

        // act
        GBE::Alu::Offset16(a, e, result);

        // assert
        CHECK_EQ(result.Result16, 250);
        CHECK((result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("Increment8")
    {
        // arrange
        uint8_t a = 28;

        // act
        GBE::Alu::Increment8(a, result);

        // assert
        CHECK_EQ(result.Result8, 29);
        CHECK(!(result.Flags & GBE::CpuFlag::H));
    }

    TEST_CASE("Increment8 Flag H")
    {
        // arrange
        uint8_t a = 31;

        // act
        GBE::Alu::Increment8(a, result);

        // assert
        CHECK_EQ(result.Result8, 32);
        CHECK((result.Flags & GBE::CpuFlag::H));
    }

    TEST_CASE("Increment8 Overflows")
    {
        // arrange
        uint8_t a = 0xff;

        // act
        GBE::Alu::Increment8(a, result);

        // assert
        CHECK_EQ(result.Result8, 0);
    }

    TEST_CASE("Decrement8")
    {
        // arrange
        uint8_t a = 28;

        // act
        GBE::Alu::Decrement8(a, result);

        // assert
        CHECK_EQ(result.Result8, 27);
        CHECK(!(result.Flags & GBE::CpuFlag::H));
    }

    TEST_CASE("Decrement8 Flag H")
    {
        // arrange
        uint8_t a = 32;

        // act
        GBE::Alu::Decrement8(a, result);

        // assert
        CHECK_EQ(result.Result8, 31);
        CHECK((result.Flags & GBE::CpuFlag::H));
    }

    TEST_CASE("Decrement8 Underflows")
    {
        // arrange
        uint8_t a = 0;

        // act
        GBE::Alu::Decrement8(a, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(-1));
    }

    TEST_CASE("Sub8")
    {
        // arrange
        uint8_t a = 5;
        uint8_t b = 1;

        // act
        GBE::Alu::Sub8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(4));
        CHECK(!(result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
        CHECK(!(result.Flags & GBE::CpuFlag::Z));
    }


    TEST_CASE("Sub8 Flag Z")
    {
        // arrange
        int8_t a = 12;
        int8_t b = 12;

        // act
        GBE::Alu::Sub8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<int8_t>(0));
        CHECK((result.Flags & GBE::CpuFlag::Z));
    }


    TEST_CASE("Sub8 Flag H")
    {
        // arrange
        uint8_t a = 44;
        uint8_t b = 15;

        // act
        GBE::Alu::Sub8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(29));
        CHECK((result.Flags & GBE::CpuFlag::H));
        CHECK(!(result.Flags & GBE::CpuFlag::C));
        CHECK(!(result.Flags & GBE::CpuFlag::Z));
    }

    TEST_CASE("Sub8 Flag C")
    {
        // arrange
        uint8_t a = 50;
        uint8_t b = 100;

        // act
        GBE::Alu::Sub8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(-50));
        CHECK((result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("Cmp Flag C")
    {
        // arrange
        uint8_t a = 50;
        uint8_t b = 100;

        // act
        GBE::Alu::Cmp8(a, b, result);

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(a));
        CHECK((result.Flags & GBE::CpuFlag::C));
    }

    TEST_CASE("And8")
    {
        // arrange
        uint8_t a = 50;  // 0011 0010
        uint8_t b = 17;  // 0001 0001

        // act
        GBE::Alu::And8(a, b, result); // result 0001 0000

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(16));
    }

    TEST_CASE("Or8")
    {
        // arrange
        uint8_t a = 50; // 0011 0010
        uint8_t b = 17; // 0001 0001

        // act
        GBE::Alu::Or8(a, b, result); // result 0011 0011

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(51));
    }

    TEST_CASE("Xor8")
    {
        // arrange
        uint8_t a = 50; // 0011 0010
        uint8_t b = 17; // 0001 0001

        // act
        GBE::Alu::Xor8(a, b, result); // result 0010 0011

        // assert
        CHECK_EQ(result.Result8, static_cast<uint8_t>(35));
    }

    TEST_CASE("Rotate left")
    {
        // ararnge
        uint8_t carry = 1;
        uint8_t a = 178; // 1011 0010

        // act
        GBE::Alu::Rotate(a, carry, GBE::ShiftDirection::LEFT, result); // result 0110 0101 and carry shoud be 1

        // assert
        CHECK_EQ(
            result.Result8, 
            static_cast<uint8_t>(101)
        );

        CHECK_NE(
            result.Flags & GBE::CpuFlag::C, 
            0
        );
    }

    TEST_CASE("Rotate right")
    {
        // ararnge
        uint8_t carry = 1;
        uint8_t a = 51; // 0011 0011

        // act
        GBE::Alu::Rotate(a, carry, GBE::ShiftDirection::RIGHT, result); // result 0110 0101 and carry shoud be 1

        // assert
        CHECK_EQ(
            result.Result8,
            static_cast<uint8_t>(153)
        );

        CHECK_NE(
            result.Flags & GBE::CpuFlag::C, 
            0
        );
    }

    TEST_CASE("Rotate with carry right")
    {
        // ararnge
        uint8_t carry = 0;
        uint8_t a = 51; // 0011 0011

        // act
        GBE::Alu::RotateCarry(a, carry, GBE::ShiftDirection::RIGHT, result); // result 1001 1001 and carry shoud be 1

        // assert
        CHECK_EQ(
            result.Result8,
            static_cast<uint8_t>(153)
        );

        CHECK_NE(
            result.Flags & GBE::CpuFlag::C, 
            0
        );
    }

    TEST_CASE("Shift Arith left")
    {
        // ararnge
        uint8_t a = 51; // 0011 0011

        // act
        GBE::Alu::Shift(a, GBE::ShiftDirection::LEFT, result); // result 0110 0110 and carry shoud be 1

        // assert
        CHECK_EQ(
            result.Result8,
            static_cast<uint8_t>(102)
        );

        CHECK_EQ(
            result.Flags & GBE::CpuFlag::C, 
            0
        );
    }

    TEST_CASE("Shift Arith right")
    {
        // ararnge
        uint8_t a = 179; // 1011 0011

        // act
        GBE::Alu::Shift(a, GBE::ShiftDirection::RIGHT, result); // result 1101 1001 and carry shoud be 1

        // assert
        CHECK_EQ(
            result.Result8,
            static_cast<uint8_t>(217)
        );

        CHECK_NE(
            result.Flags & GBE::CpuFlag::C, 
            0
        );
    }

    TEST_CASE("Shift Logical right")
    {
        // ararnge
        uint8_t a = 179; // 1011 0011

        // act
        GBE::Alu::Shift(a, GBE::ShiftDirection::RIGHT, result, true); // result 0101 1001 and carry shoud be 1

        // assert
        CHECK_EQ(
            result.Result8,
            static_cast<uint8_t>(89)
        );

        CHECK_NE(
            result.Flags & GBE::CpuFlag::C, 
            0
        );
    }

    TEST_CASE("Swap test")
    {
        // ararnge
        uint8_t a = 0xAB; // 1011 0011

        // act
        GBE::Alu::Swap(a, result); // result 0101 1001 and carry shoud be 1

        // assert
        CHECK_EQ(
            result.Result8,
            static_cast<uint8_t>(0xBA)
        );
    }

}

