#include "GBETestSuite.h"

#include "cpu/instruction/Instruction.h"
#include "cpu/instruction/InstructionDecoder.h"
#include "cpu/instruction/Operand.h"

#include <stdexcept>


GBE_TEST_SUITE(InstructionDecoder)
{
    TEST_CASE("Decode: NOP (0x00)")
    {
        GBE::InstructionDecoder decoder;
        const GBE::Instruction &instr = decoder.Decode(0x00);

        CHECK_EQ(instr.Type, GBE::InstructionType::NOP);
        CHECK_EQ(instr.Opcode, 0x00);
    }

    TEST_CASE("Decode: LD r8,r8 (0x40)")
    {
        GBE::InstructionDecoder decoder;
        const GBE::Instruction &instr = decoder.Decode(0x40);
        
        CHECK_EQ(instr.Type, GBE::InstructionType::LD);
        CHECK_EQ(instr.OperandsCount, 2);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(0), GBE::OperandR8::B);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(1), GBE::OperandR8::B);
    }

    TEST_CASE("Decode: LD r16, imm16 (0x11)")
    {
        GBE::InstructionDecoder decoder;
        const GBE::Instruction &instr = decoder.Decode(0x11);

        CHECK_EQ(instr.Type, GBE::InstructionType::LD);
        CHECK_EQ(instr.OperandsCount, 2);
        CHECK_EQ(instr.GetOperand<GBE::OperandR16>(0), GBE::OperandR16::DE);
        CHECK_EQ(instr.Operands.at(1).GetType(), GBE::OperandType::IMM16);
    }

    TEST_CASE("Decode: JR imm8 (0x18)")
    {
        GBE::InstructionDecoder decoder;
        const GBE::Instruction &instr = decoder.Decode(0x18);

        CHECK_EQ(instr.Type, GBE::InstructionType::JR);
    }

    TEST_CASE("DecodePrefix: RLC C (0x01)")
    {
        GBE::InstructionDecoder decoder;
        const GBE::Instruction &instr = decoder.DecodePrefix(0x01);

        CHECK_EQ(instr.Type, GBE::InstructionType::RLC);
        CHECK_EQ(instr.OperandsCount, 1);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(0), GBE::OperandR8::C);
    }

    TEST_CASE("DecodePrefix: BIT 3, D (0x5A)")
    {
        GBE::InstructionDecoder decoder;
        const GBE::Instruction &instr = decoder.DecodePrefix(0x5A);

        CHECK_EQ(instr.Type, GBE::InstructionType::BIT);
        CHECK_EQ(instr.OperandsCount, 2);
        CHECK_EQ(instr.GetOperand<GBE::OperandBit3>(0).Value, 3);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(1), GBE::OperandR8::D);
    }

    TEST_CASE("Decode: cached result returned")
    {
        GBE::InstructionDecoder decoder;
        const GBE::Instruction &first = decoder.Decode(0x00);
        const GBE::Instruction &second = decoder.Decode(0x00);
        CHECK_EQ(&first, &second);
    }

}