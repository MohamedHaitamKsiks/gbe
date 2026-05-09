#include "Cpu.h"


#include "instruction/Instruction.h"
#include "instruction/InstructionResult.h"

#include "memory/Memory.h"

namespace GBE
{

    void Cpu::Push(uint16_t value, InstructionResult &result)
    {
        // decrement stack pointer
        uint16_t stack = m_Regs.GetReg16(Reg16::SP);
        stack -= 2;
        m_Regs.SetReg16(Reg16::SP, stack);

        // load
        m_Memory->Set16(stack, value);

        // add cycles
        result.Cycles += 3;
    }

    uint16_t Cpu::Pop(InstructionResult &result)
    {
        // get top
        uint16_t stack = m_Regs.GetReg16(Reg16::SP);
        uint16_t top = m_Memory->Get16(stack);

        // increment stack
        stack += 2;
        m_Regs.SetReg16(Reg16::SP, stack);

        // add cycles
        result.Cycles += 2;

        // returns the popped value
        return top;
    }

    uint16_t Cpu::Top()
    {
        uint16_t stack = m_Regs.GetReg16(Reg16::SP);
        return m_Memory->Get16(stack);
    }

    void Cpu::PushR16Stk(const Instruction &instr, InstructionResult &result)
    {
        // fetch
        auto [r16stk] = instr.GetOperands<OperandR16Stk>();
        uint16_t value = GetOperandR16Stk(r16stk, result);

        // execute
        Push(value, result);
    }

    void Cpu::PopR16Stk(const Instruction &instr, InstructionResult &result)
    {
        // execute
        auto [r16stk] = instr.GetOperands<OperandR16Stk>();
        uint16_t top = Pop(result);

        // result
        SetOperandR16Stk(r16stk, top, result);
    }
} // namespace GBE
