#include "GBETestSuite.h"

#include "cpu/instruction/Instruction.h"
#include "cpu/instruction/InstructionDecoder.h"
#include "cpu/instruction/Operand.h"

#include <print>
#include <vector>
#include <stdexcept>

GBE_TEST_SUITE(InstructionDecoder)
{
    TEST_CASE("Decode: NOP (0x00)")
    {
        GBE::InstructionDecoder decoder{};
        const GBE::Instruction &instr = decoder.Decode(0x00);

        CHECK_EQ(instr.GetType(), GBE::InstructionType::NOP);
        CHECK_EQ(instr.GetOpcode(), 0x00);
    }

    TEST_CASE("Decode: LD r8,r8 (0x40)")
    {
        GBE::InstructionDecoder decoder{};
        const GBE::Instruction &instr = decoder.Decode(0x40);

        CHECK_EQ(instr.GetType(), GBE::InstructionType::LD);
        CHECK_EQ(instr.GetOperandsCount(), 2);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(0), GBE::OperandR8::B);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(1), GBE::OperandR8::B);
    }

    TEST_CASE("Decode: LD r16, imm16 (0x11)")
    {
        GBE::InstructionDecoder decoder{};
        const GBE::Instruction &instr = decoder.Decode(0x11);

        CHECK_EQ(instr.GetType(), GBE::InstructionType::LD);
        CHECK_EQ(instr.GetOperandsCount(), 2);
        CHECK_EQ(instr.GetOperand<GBE::OperandR16>(0), GBE::OperandR16::DE);
        CHECK_EQ(instr.GetOperandType(1), GBE::OperandType::IMM16);
    }

    TEST_CASE("Decode: JR imm8 (0x18)")
    {
        GBE::InstructionDecoder decoder{};
        const GBE::Instruction &instr = decoder.Decode(0x18);

        CHECK_EQ(instr.GetType(), GBE::InstructionType::JR);
    }

    TEST_CASE("DecodePrefix: RLC C (0x01)")
    {
        GBE::InstructionDecoder decoder{};
        const GBE::Instruction &instr = decoder.DecodePrefix(0x01);

        CHECK_EQ(instr.GetType(), GBE::InstructionType::RLC);
        CHECK_EQ(instr.GetOperandsCount(), 1);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(0), GBE::OperandR8::C);
    }

    TEST_CASE("DecodePrefix: BIT 3, D (0x5A)")
    {
        GBE::InstructionDecoder decoder{};
        const GBE::Instruction &instr = decoder.DecodePrefix(0x5A);

        CHECK_EQ(instr.GetType(), GBE::InstructionType::BIT);
        CHECK_EQ(instr.GetOperandsCount(), 2);
        CHECK_EQ(instr.GetOperand<GBE::OperandBit3>(0).Value, 3);
        CHECK_EQ(instr.GetOperand<GBE::OperandR8>(1), GBE::OperandR8::D);
    }

    TEST_CASE("Decode: cached result returned")
    {
        GBE::InstructionDecoder decoder{};
        const GBE::Instruction &first = decoder.Decode(0x00);
        const GBE::Instruction &second = decoder.Decode(0x00);
        CHECK_EQ(&first, &second);
    }

    TEST_CASE("All opcodes are decoded")
    {
        std::vector<uint8_t> invalidOpcodes = {0xD3, 0xDB, 0xDD, 0xE3, 0xE4, 0xEB, 0xEC, 0xED, 0xF4, 0xFC, 0xFD};
        
        GBE::InstructionDecoder decoder{};
        for (uint16_t opcode = 0; opcode <= 0xFF; opcode++)
        {
            if (std::find(invalidOpcodes.begin(), invalidOpcodes.end(), opcode) != invalidOpcodes.end())
                continue;
                
            const GBE::Instruction &instr = decoder.Decode(opcode);

            CHECK_NE(instr.GetType(), GBE::InstructionType::NONE);
            if (instr.GetType() != GBE::InstructionType::PREFIX_INST)
                CHECK_NE(instr.GetMethod(), nullptr);
            
        }
    }

    TEST_CASE("All prefix opcodes are decoded")
    {
        GBE::InstructionDecoder decoder{};
        for (uint16_t opcode = 0; opcode <= 0xFF; opcode++)
        {
            const GBE::Instruction &instr = decoder.DecodePrefix(opcode);
            CHECK_NE(instr.GetType(), GBE::InstructionType::NONE);
            CHECK_NE(instr.GetMethod(), nullptr);
        }
    }
}