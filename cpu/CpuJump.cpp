#include "Cpu.h"

#include "memory/Memory.h"

#include "instruction/InstructionResult.h"

namespace GBE
{

    void Cpu::_Call(uint16_t adr16, Memory &memory, InstructionResult &result)
    {
        uint16_t de = m_Regs.GetReg16(Reg16::DE);

        // push pc to stack
        uint16_t pc = m_Regs.GetReg16(Reg16::PC);
        Push(pc, memory, result);

        // set pc to adr
        m_Regs.SetReg16(Reg16::PC, adr16);
    }

    void Cpu::CallImm16(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        //  operands
        auto [imm16] = instr.GetOperands<OperandImm16>();

        // fetch
        uint16_t imm16Value = GetImm16(memory, result);

        // execute
        _Call(imm16Value, memory, result);
    }

    void Cpu::CallCC_Imm16(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // operands
        auto [cc, imm16] = instr.GetOperands<OperandCond, OperandImm16>();

        // fetch
        uint16_t imm16Value = GetImm16(memory, result);

        // execute
        if (CheckOperandCond(cc))
            _Call(imm16Value, memory, result);
    }

    void Cpu::JumpHL(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // operands
        auto [hl] = instr.GetOperands<OperandR16>();
     
        // fetch
        uint16_t adr16 = m_Regs.GetReg16(Reg16::HL);

        // execute
        m_Regs.SetReg16(Reg16::PC, adr16);
    }

    void Cpu::JumpImm16(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        uint16_t adr16 = GetImm16(memory, result);

        // execute
        m_Regs.SetReg16(Reg16::PC, adr16);
        result.Cycles += 1;
    }

    void Cpu::JumpCC_Imm16(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto cc = instr.GetOperand<OperandCond>(0);
        uint16_t adr16 = GetImm16(memory, result);

        // execute
        if (CheckOperandCond(cc))
        {
            m_Regs.SetReg16(Reg16::PC, adr16);
            result.Cycles += 1;
        }
    }

    void Cpu::_JumpRelative(uint8_t offset8, InstructionResult &result)
    {
        int8_t signedOffset8 = static_cast<int8_t>(offset8);
        int16_t signedOffset16 = static_cast<int16_t>(signedOffset8);

        // offset pc
        uint16_t pc = m_Regs.GetReg16(Reg16::PC) + signedOffset16;
        m_Regs.SetReg16(Reg16::PC, pc);
        result.Cycles++;
    }

    void Cpu::JumpRelativeImm8(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t offset = GetImm8(memory, result);

        // execute
        _JumpRelative(offset, result);
    }

    void Cpu::JumpRelativeCC_Imm8(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto [cc, imm8] = instr.GetOperands<OperandCond, OperandImm8>();
        uint8_t offset = GetImm8(memory, result);

        // execute
        if (CheckOperandCond(cc))
            _JumpRelative(offset, result);
    }

    void Cpu::Return(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // pop top adress
        uint16_t topAdr16 = Pop(memory, result);

        // jump to adress
        m_Regs.SetReg16(Reg16::PC, topAdr16);
        result.Cycles++;
    }

    void Cpu::ReturnCC(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        auto cc = instr.GetOperand<OperandCond>(0);
        result.Cycles++;

        if (CheckOperandCond(cc))
            Return(instr, memory, result);
    }

    void Cpu::ReturnAndEnableInterrupts(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        EnableInterrupts(instr, memory, result);
        Return(instr, memory, result);
    }

    void Cpu::RstVec(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        auto [tgt3] = instr.GetOperands<OperandTgt3>();

        uint16_t adr16 = static_cast<uint16_t>(tgt3.Value) * 8;
        _Call(adr16, memory, result);
    }

} // namespace GBE
