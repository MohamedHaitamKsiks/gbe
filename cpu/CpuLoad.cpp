#include "Cpu.h"

#include "util/Assert.h"
#include "memory/Memory.h"

#include "instruction/Instruction.h"
#include "instruction/InstructionResult.h"

namespace GBE
{

    void Cpu::LoadR16_Imm16(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [r16, imm16] = instr.GetOperands<OperandR16, OperandImm16>();

        // fetch
        uint16_t imm16Value = GetImm16(result);

        // execute
        SetOperandR16(r16, imm16Value, result);
    }

    void Cpu::LoadR16Mem_A(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [r16mem, a] = instr.GetOperands<OperandR16Mem, OperandR8>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint8_t value = GetOperandR8(a, result);

        // execute
        SetOperandR16Mem(r16mem, value, result);
    }

    void Cpu::LoadA_R16Mem(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [a, r16mem] = instr.GetOperands<OperandR8, OperandR16Mem>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint8_t value = GetOperandR16Mem(r16mem, result);

        // execute
        SetOperandR8(a, value, result);
    }

    void Cpu::LoadAdrImm16_SP(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [imm16, sp] = instr.GetOperands<OperandImm16, OperandR16>();
        GBE_ASSERT(sp == OperandR16::SP);

        // fetch
        int16_t imm16Value = GetImm16(result);
        int16_t value = GetOperandR16(sp, result);

        // execute
        m_Memory->Set16(imm16Value, value);
        result.Cycles += 2;
    }

    void Cpu::LoadR8_Imm8(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [r8, imm8] = instr.GetOperands<OperandR8, OperandImm8>();

        // fetch
        uint8_t imm8Value = GetImm8(result);

        // execute
        SetOperandR8(r8, imm8Value, result);
    }

    void Cpu::LoadR8_R8(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [dest8, src8] = instr.GetOperands<OperandR8, OperandR8>();

        // fetch
        uint8_t value = GetOperandR8(src8, result);

        // execute
        SetOperandR8(dest8, value, result);
    }

    void Cpu::LoadHighC_A(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [c, a] = instr.GetOperands<OperandR8, OperandR8>();
        GBE_ASSERT(a == OperandR8::A);
        GBE_ASSERT(c == OperandR8::C);

        // fetch
        uint8_t src = m_Regs.GetReg8(Reg8::A);
        uint16_t destAdr = 0xff00 + m_Regs.GetReg8(Reg8::C);

        // execute
        m_Memory->Set(
            destAdr,
            src
        );
        result.Cycles++;
    }

    void Cpu::LoadA_HighC(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [a, c] = instr.GetOperands<OperandR8, OperandR8>();
        GBE_ASSERT(a == OperandR8::A);
        GBE_ASSERT(c == OperandR8::C);

        // fetch
        uint16_t srcAdr = 0xff00 + m_Regs.GetReg8(Reg8::C);
        uint8_t src = m_Memory->Get(srcAdr);
        result.Cycles++;

        // execute
        m_Regs.SetReg8(Reg8::A, src);
    }

    void Cpu::LoadHighImm8_A(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [imm8, a] = instr.GetOperands<OperandImm8, OperandR8>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint8_t src = m_Regs.GetReg8(Reg8::A);
        uint16_t destAdr = 0xff00 + GetImm8(result);

        // execute
        m_Memory->Set(
            destAdr,
            src
        );
        result.Cycles++;
    }

    void Cpu::LoadA_HighImm8(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [a, imm8] = instr.GetOperands<OperandR8, OperandImm8>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint16_t srcAdr = 0xff00 + GetImm8(result);
        uint8_t src = m_Memory->Get(srcAdr);
        result.Cycles++;

        // execute
        m_Regs.SetReg8(Reg8::A, src);
    }

    void Cpu::LoadAdrImm16_A(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [imm16, a] = instr.GetOperands<OperandImm16, OperandR8>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint8_t src = m_Regs.GetReg8(Reg8::A);
        uint16_t destAdr = GetImm16(result);

        // execute
        m_Memory->Set(
            destAdr,
            src
        );
        result.Cycles++;
    }

    void Cpu::LoadA_AdrImm16(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [a, imm16] = instr.GetOperands<OperandR8, OperandImm16>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint16_t srcAdr = GetImm16(result);
        uint8_t src = m_Memory->Get(srcAdr);
        result.Cycles++;

        // execute
        m_Regs.SetReg8(Reg8::A, src);
    }

    void Cpu::LoadHL_SPImm8(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [hl, sp, imm8] = instr.GetOperands<OperandR16, OperandR16, OperandImm8>();
        GBE_ASSERT(sp == OperandR16::SP);
        GBE_ASSERT(hl == OperandR16::HL);

        _AddToDestSP_Imm8(Reg16::HL, result);
    }

    void Cpu::LoadSP_HL(const Instruction &instr, InstructionResult &result)
    {
        // operands
        auto [sp, hl] = instr.GetOperands<OperandR16, OperandR16>();
        GBE_ASSERT(sp == OperandR16::SP);
        GBE_ASSERT(hl == OperandR16::HL);

        // fetch
        uint16_t hlValue = m_Regs.GetReg16(Reg16::HL);

        // execute
        m_Regs.SetReg16(Reg16::SP, hlValue);

        // result
        result.Cycles++;
    }
    
} // namespace GBE
