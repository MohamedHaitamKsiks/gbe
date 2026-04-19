#include "Disassembler.h"

#include "Assembly.h"
#include "memory/Memory.h"
#include "cpu/instruction/InstructionDecoder.h"
#include <cstdint>

namespace GBE
{
    Disassembler::Disassembler(
        const std::shared_ptr<Memory>& memory, 
        const std::shared_ptr<InstructionDecoder>& decoder
    ):
        m_Memory(memory),
        m_Decoder(decoder)
    {
    }

    void Disassembler::Disassemble(uint16_t startAddress)
    {

    }

    void Disassembler::DisassembleInstruction(uint16_t address, Assembly &assembly)
    {
        // get opcode
        uint8_t opcode = m_Memory->Get(address);

        // decode instruction
        const Instruction &instr = m_Decoder->Decode(opcode);
        if (instr.GetType() == InstructionType::PREFIX_INST)
        {
            uint8_t prefixOpcode = m_Memory->Get(address + 1);
            const Instruction &prefixInstr = m_Decoder->DecodePrefix(prefixOpcode);

            _CreateAssembly(address, prefixInstr, assembly);
            return;
        }

        _CreateAssembly(address, instr, assembly);
    }

    void Disassembler::_CreateAssembly(uint16_t address, const Instruction &instr, Assembly &assembly)
    {
        assembly.SetAddress(address);

        InstructionType type = instr.GetType();
        assembly.SetOperation(type);
        if (type == InstructionType::NONE)
            return;

        _AddOperands(address, instr, assembly);

        // add jump next adress
        _AddJumpOrCallNextAddress(address, instr, assembly);
        _AddJumpRelativeNextAddress(address, instr, assembly);

        // don't add next instr address in this case
        if (_IsNonConditionalJumpOrReturn(instr))
            return;

        Assembly::NextAddress nextInsrtAddress{
            .Address    = address + instr.GetSize(),
            .IsJump     = false
        };
        assembly.AddNextAddress(nextInsrtAddress);
    }

    void Disassembler::_AddOperands(uint16_t address, const Instruction &instr, Assembly &assembly)
    {
        for (size_t i = 0; i < instr.GetOperandsCount(); i++)
        {
            Operand operand = instr.GetOperand(i);
            switch (operand.GetType())
            {
            case OperandType::IMM8:
            {
                uint8_t imm8 = m_Memory->Get(address + 1);
                assembly.AddImm8(imm8, operand.IsAddress());
                break;
            }
            case OperandType::IMM16:
            {
                uint16_t imm16 = m_Memory->Get16(address + 1);
                assembly.AddImm16(imm16, operand.IsAddress());
                break;
            }
            default:
                assembly.AddOperand(operand);
                break;
            }
        }
    }

    void Disassembler::_AddJumpOrCallNextAddress(uint16_t address, const Instruction &instr, Assembly &assembly)
    {
        InstructionType type = instr.GetType();

        if (type != InstructionType::JP && type != InstructionType::CALL)
            return;
    
        for (size_t i = 0; i < instr.GetOperandsCount(); i++)
        {
            if (instr.GetOperandType(i) == OperandType::IMM16)
            {
                uint16_t imm16 = m_Memory->Get16(address + 1);
                Assembly::NextAddress jumpAddress{
                    .Address = imm16,
                    .IsJump = true
                };
                assembly.AddNextAddress(jumpAddress);
                break;
            }
        }
    }

    void Disassembler::_AddJumpRelativeNextAddress(uint16_t address, const Instruction &instr, Assembly &assembly)
    {
        InstructionType type = instr.GetType();
        if (type != InstructionType::JR)
            return;

        for (size_t i = 0; i < instr.GetOperandsCount(); i++)
        {
            if (instr.GetOperandType(i) == OperandType::IMM8)
            {
                uint8_t offset8 = m_Memory->Get(address + 1);
                int8_t signedOffset8 = static_cast<int8_t>(offset8);
                int16_t signedOffset16 = static_cast<int16_t>(signedOffset8);

                uint16_t jumpAddress16 = address + signedOffset16;
                Assembly::NextAddress jumpAddress{
                    .Address = jumpAddress16,
                    .IsJump = true
                };
                assembly.AddNextAddress(jumpAddress);
                break;
            }
        }
    
    }

    bool Disassembler::_IsNonConditionalJumpOrReturn(const Instruction &instr)
    {
        InstructionType type = instr.GetType();

        if (type != InstructionType::JP     && 
            type != InstructionType::JR     && 
            type != InstructionType::RET    &&
            type != InstructionType::RETI
        )
            return false;

        for (size_t i = 0; i < instr.GetOperandsCount(); i++)
        {
            if (instr.GetOperandType(i) == OperandType::COND)
                return false;
        }

        return true;
    }

} // namespace GBE
