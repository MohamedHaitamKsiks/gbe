#include "Cpu.h"

#include "memory/Memory.h"

#include "instruction/InstructionResult.h"

#include "alu/AluResult.h"
#include "alu/Alu.h"

namespace GBE
{
    void Cpu::_AddToDestSP_Imm8(Reg16 dest, Memory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t imm8 = GetImm8(memory, result);

        uint16_t a = m_Regs.GetReg16(Reg16::SP);
        int8_t e = static_cast<int8_t>(imm8);

        // execute
        AluResult aluResult{};
        Alu::Offset16(a, e, aluResult);
        m_Regs.SetReg16(dest, aluResult.Result16);

        // result
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
        result.Cycles++;
    }

    void Cpu::AddSP_Imm8(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        _AddToDestSP_Imm8(Reg16::SP, memory, result);
    }

    void Cpu::AddHL_R16(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto r16 = instr.GetOperand<OperandR16>(1);

        uint16_t a = GetOperandR16(OperandR16::HL, result);
        uint16_t b = GetOperandR16(r16, result);

        // execute
        AluResult aluResult{};
        Alu::Add16(a, b, aluResult);
        result.Cycles++;

        // save result
        SetOperandR16(OperandR16::HL, aluResult.Result16, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::ComplementA(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t a = m_Regs.GetReg8(Reg8::A);

        // execute
        m_Regs.SetReg8(Reg8::A, ~a);

        // result
        m_Regs.SetFlag(CpuFlag::H, true);
        m_Regs.SetFlag(CpuFlag::N, true);
    }

    void Cpu::ExecAluOpR16(Alu::OperationDest16 op, const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto r16 = instr.GetOperand<OperandR16>(0);
        uint16_t value = GetOperandR16(r16, result);

        // execute
        AluResult aluResult{};
        op(value, aluResult);
        result.Cycles++;

        // save result
        SetOperandR16(r16, aluResult.Result16, result);
    }

    void Cpu::ExecAluOpR8(Alu::OperationDest8 op, const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto r8 = instr.GetOperand<OperandR8>(0);
        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        AluResult aluResult{};
        op(value, aluResult);

        // result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::_ExecAluOpA(Alu::OperationDestSrc8 op, uint8_t v8, bool addCarry)
    {
        // fetch
        uint8_t a = m_Regs.GetReg8(Reg8::A);

        // execute
        AluResult aluResult{};
        op(a, v8, aluResult, (addCarry) ? _GetCarry() : 0);

        // result
        m_Regs.SetReg8(Reg8::A, aluResult.Result8);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::ExecAluOpA_R8(Alu::OperationDestSrc8 op, bool addCarry, const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // operands
        auto [a, r8] = instr.GetOperands<OperandR8, OperandR8>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        _ExecAluOpA(op, value, addCarry);
    }

    void Cpu::ExecAluOpA_Imm8(Alu::OperationDestSrc8 op, bool addCarry, const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // operands
        auto [a, imm8] = instr.GetOperands<OperandR8, OperandImm8>();
        GBE_ASSERT(a == OperandR8::A);

        // fetch
        uint8_t value = GetImm8(memory, result);

        // execute
        _ExecAluOpA(op, value, addCarry);
    }

    void Cpu::RotateR8(Alu::OperationRotateSrc op, ShiftDirection direction,  const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        OperandR8 r8 = OperandR8::A;
        bool checkZero = false;

        if (instr.GetOperandsCount() > 0)
        {
            r8 = instr.GetOperand<OperandR8>(0);
            checkZero = true;
        }

        uint8_t value = GetOperandR8(r8, memory, result);
        uint8_t carry = _GetCarry();

        // run alu rotation op
        AluResult aluResult{};
        op(
            value,
            carry,
            direction,
            aluResult,
            checkZero
        );

        // save result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::ShiftR8(ShiftDirection direction, bool isLogical, const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // operands
        auto [r8] = instr.GetOperands<OperandR8>();

        // fetch
        uint8_t value = GetOperandR8(r8, memory, result);
        uint8_t carry = _GetCarry();

        // run alu rotation op
        AluResult aluResult{};
        Alu::Shift(
            value,
            direction,
            aluResult,
            isLogical
        );

        // save result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::SwapR8(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // operands
        auto [r8] = instr.GetOperands<OperandR8>();

        // fetch
        uint8_t value = GetOperandR8(r8, memory, result);

        // run alu swap op
        AluResult aluResult{};
        Alu::Swap(
            value,
            aluResult
        );

        // save result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::TestBitR8(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto [bit, r8] = instr.GetOperands<OperandBit3, OperandR8>();

        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        AluResult aluResult{};
        Alu::TestBit(bit.Value, aluResult.Result8, aluResult);

        // result
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::SetBitR8(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto [bit, r8] = instr.GetOperands<OperandBit3, OperandR8>();

        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        AluResult aluResult{};
        Alu::SetBit(bit.Value, value, aluResult);

        // result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::ResetBitR8(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // fetch
        auto [bit, r8] = instr.GetOperands<OperandBit3, OperandR8>();

        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        AluResult aluResult{};
        Alu::ResetBit(bit.Value, value, aluResult);

        // result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }
} // namespace GBE
