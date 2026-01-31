#include "Disassembler.h"

#include "Assembly.h"
#include "memory/Memory.h"
#include "cpu/instruction/InstructionDecoder.h"
#include <cstdint>

namespace GBE
{
    
    Assembly Disassembler::Disassemble(uint16_t instAddr, const Memory &memory, const InstructionDecoder &decoder)
    {
        // get opcode
        uint8_t opcode = memory.Get(instAddr);

        // decode instruction
        const Instruction& instr = decoder.Decode(opcode);
        if (instr.GetType() == InstructionType::PREFIX_INST)
        {
            Assembly assembly = Disassemble(instAddr + 1, memory, decoder);
            return assembly;
        }

        Assembly assembly{};
        assembly.SetAddress(instAddr);
        assembly.SetOpcode(opcode);
        assembly.SetOperation(instr.GetType());

        for (size_t i = 0; i < instr.GetOperandsCount(); i++)
        {
            Operand operand = instr.GetOperand(i);
            switch (operand.GetType())
            {
            case OperandType::IMM8:
            {
                uint8_t imm8 = memory.Get(instAddr + 1);
                assembly.AddImm8(imm8, operand.IsAddress());
                break;
            }
            case OperandType::IMM16:
            {
                uint16_t imm16 = memory.Get16(instAddr + 1);
                assembly.AddImm16(imm16, operand.IsAddress());
                break;
            }
            default:
                assembly.AddOperand(operand);
                break;
            }
        }

        return assembly;
    }
    
} // namespace GBE
