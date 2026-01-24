#include "Cpu.h"

#include "instruction/InstructionResult.h"

#include "alu/AluResult.h"
#include "alu/Alu.h"

#include "memory/Memory.h"

#include <iostream>
#include <exception>

namespace GBE
{
    void Cpu::Run(Memory &memory, InstructionResult &result)
    {
        // check if interruption is pending
        if (_HandleInterrupts(memory, result))
            return;

        // handle instruction
        result.Cycles = 0;

        uint8_t opcode = GetImm8(memory, result);
        const Instruction& instr = m_Decoder.Decode(opcode);
        _RunInstruction(instr, memory, result);

        // handle queue TME
        _HandleIME();

    }

    void Cpu::_RunInstruction(const Instruction &instr, Memory& memory, InstructionResult& result)
    {
        if (instr.GetType() == InstructionType::PREFIX_INST)
        {
            _RunPrefixInstruction(memory, result);
            return;
        }

        // run instruction method
        (this->*instr.GetMethod())(instr, memory, result);
    }

    void Cpu::_RunPrefixInstruction(Memory &memory, InstructionResult &result)
    {
        uint8_t opcode = GetImm8(memory, result);
        const Instruction& instr = m_Decoder.DecodePrefix(opcode);

        _RunInstruction(instr, memory, result);
    }

} // namespace GBE
