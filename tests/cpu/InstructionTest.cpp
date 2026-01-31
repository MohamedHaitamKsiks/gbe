#include "GBETestSuite.h"

#include "cpu/instruction/Instruction.h"

GBE_TEST_SUITE(Instruction)
{
    TEST_CASE("AddOperand")
    {   
        // arrange
        GBE::Instruction instr{};
        std::vector<GBE::OperandType> expectedTypes = {
            GBE::OperandType::R8,
            GBE::OperandType::IMM16
        };

        // act
        instr.AddOperand(GBE::OperandR8::C, GBE::OperandImm16{});

        // assert
        CHECK_EQ(instr.GetOperandsCount(), 2);
        for (int i = 0; i < instr.GetOperandsCount(); i++)
        {
            CHECK_EQ(instr.GetOperandType(i), expectedTypes[i]);
        }
    }

    TEST_CASE("AddOperandAddress")
    {
        // arrange
        GBE::Instruction instr{};

        // act
        instr.AddOperand(GBE::OperandAddressOf(GBE::OperandR8::C));

        // assert
        CHECK_EQ(instr.GetOperandsCount(), 1);
        CHECK_EQ(instr.IsOperandAddress(0), true);
    }

    TEST_CASE("GetOperand")
    {
        // arrange
        GBE::Instruction instr{};
        instr.AddOperand(GBE::OperandR8::C, GBE::OperandBit3{5});

        // act
        auto r8 = instr.GetOperand<GBE::OperandR8>(0);
        auto bit3 = instr.GetOperand<GBE::OperandBit3>(1);

        // assert
        CHECK_EQ(r8, GBE::OperandR8::C);
        CHECK_EQ(bit3.Value, 5);
    }

    TEST_CASE("GetOperands")
    {
        // arrange
        GBE::Instruction instr{};
        instr.AddOperand(GBE::OperandR8::C, GBE::OperandBit3{8});

        // act
        auto [r8, bit3] = instr.GetOperands<GBE::OperandR8, GBE::OperandBit3>();

        // assert
        CHECK_EQ(r8, GBE::OperandR8::C);
        CHECK_EQ(bit3.Value, 8);
    }
}