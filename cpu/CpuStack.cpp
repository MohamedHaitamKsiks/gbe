#include "Cpu.h"

#include "memory/Memory.h"
#include "instruction/InstructionResult.h"

namespace GBE
{

    void Cpu::Push(uint16_t value, Memory &memory, InstructionResult &result)
    {
        // decrement stack pointer
        uint16_t stack = m_Regs.GetReg16(Reg16::SP);
        stack -= 2;
        m_Regs.SetReg16(Reg16::SP, stack);

        // load
        memory.Set16(stack, value);

        // add cycles
        result.Cycles += 3;
    }

    uint16_t Cpu::Pop(Memory &memory, InstructionResult &result)
    {
        // get top
        uint16_t stack = m_Regs.GetReg16(Reg16::SP);
        uint16_t top = memory.Get16(stack);

        // increment stack
        stack += 2;
        m_Regs.SetReg16(Reg16::SP, stack);

        // add cycles
        result.Cycles += 2;

        // returns the popped value
        return top;
    }

    uint16_t Cpu::Top(Memory &memory)
    {
        uint16_t stack = m_Regs.GetReg16(Reg16::SP);
        return memory.Get16(stack);
    }

    void Cpu::PushR16Stk(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto [r16stk] = instr.GetOperands<OperandR16Stk>();
        uint16_t value = GetOperandR16Stk(r16stk, result);

        // execute
        Push(value, memory, result);
    }

    void Cpu::PopR16Stk(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // execute
        auto [r16stk] = instr.GetOperands<OperandR16Stk>();
        uint16_t top = Pop(memory, result);

        // result
        SetOperandR16Stk(r16stk, top, result);
    }
} // namespace GBE
