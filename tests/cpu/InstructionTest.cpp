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
        CHECK_EQ(instr.OperandsCount, 2);
        for (int i = 0; i < instr.OperandsCount; i++)
        {
            CHECK_EQ(instr.Operands[i].GetType(), expectedTypes[i]);
        }
    }

    TEST_CASE("AddOperandAddress")
    {
        // arrange
        GBE::Instruction instr{};

        // act
        instr.AddOperand(GBE::OperandAddressOf(GBE::OperandR8::C));

        // assert
        CHECK_EQ(instr.OperandsCount, 1);
        CHECK_EQ(instr.Operands[0].IsAddress(), true);
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
}