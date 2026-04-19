#include "Cpu.h"

#include "instruction/Instruction.h"
#include "instruction/InstructionDecoder.h"
#include "instruction/InstructionResult.h"

#include "alu/AluResult.h"
#include "alu/Alu.h"

#include "memory/Memory.h"

#include <iostream>
#include <exception>
#include <print>

namespace GBE
{
    void Cpu::Run(InstructionResult &result)
    {
        uint16_t pc = m_Regs.GetReg16(Reg16::PC);

        // debug
        if (m_Debugger.IsEnabled())
        {
            m_Debugger.Tick(pc);

            if (m_Debugger.IsBreaked())
                return;
        }

        // don't process intructions if halted
        if (m_IsHalted)
        {
            _HandleHalt(result);
            return;
        }

        // check if interruption is pending
        if (_HandleInterrupts(result))
            return;

        // handle instruction
        result.Cycles = 0;

        uint8_t opcode = GetImm8(result);
        const Instruction& instr = m_Decoder->Decode(opcode);
        _RunInstruction(instr, result);

        // handle queue TME
        _HandleIME();

        // if bug don't increment PC to run instruction twice
        _HandleHaltBug(pc);

    }

    void Cpu::_RunInstruction(const Instruction &instr, InstructionResult& result)
    {
        GBE_ASSERT(instr.GetType() != InstructionType::INVALID);

        if (instr.GetType() == InstructionType::PREFIX_INST)
        {
            _RunPrefixInstruction(result);
            return;
        }
        
        // run instruction method
        (this->*instr.GetMethod())(instr, result);
    }

    void Cpu::_RunPrefixInstruction(InstructionResult &result)
    {
        uint8_t opcode = GetImm8(result);
        const Instruction& instr = m_Decoder->DecodePrefix(opcode);
        (this->*instr.GetMethod())(instr, result);
    }



} // namespace GBE
