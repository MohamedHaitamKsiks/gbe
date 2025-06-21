#include "Cpu.h"

#include "InstructionResult.h"
#include "AluResult.h"
#include "Alu.h"

#include "memory/IMemory.h"
#include "util/Binary.h" 
#include "io/IORegister.h"

#include <iostream>
#include <exception>


namespace GBE
{
    Cpu::~Cpu()
    {
        
    }

    uint8_t Cpu::GetReg16Adr(Reg16 adr, IMemory &memory) const
    {
        return memory.Get(m_Regs.GetReg16(adr));
    }

    void Cpu::SetReg16Adr(Reg16 adr, uint8_t value, IMemory &memory)
    {
        memory.Set(
            m_Regs.GetReg16(adr), 
            value
        );
    }

    void Cpu::_HLIncDec(bool isInc)
    {
        AluResult aluResult{};

        if (isInc)
            Alu::Increment16(m_Regs.GetReg16(Reg16::HL), aluResult);
        else
            Alu::Decrement16(m_Regs.GetReg16(Reg16::HL), aluResult);

        m_Regs.SetReg16(Reg16::HL, aluResult.Result16);
    }

    uint8_t Cpu::GetOperandR8(OperandR8 r8, IMemory &memory, InstructionResult &result)
    {
        if (r8 == OperandR8::ADR_HL)
        {
            result.Cycles++;
            return GetReg16Adr(Reg16::HL, memory);
        }

        return m_Regs.GetReg8(static_cast<Reg8>(r8));
    }

    void Cpu::SetOperandR8(OperandR8 r8, uint8_t value, IMemory &memory, InstructionResult &result)
    {
        if (r8 == OperandR8::ADR_HL)
        {
            result.Cycles++;
            SetReg16Adr(Reg16::HL, value, memory);
            return; 
        }

        m_Regs.SetReg8(static_cast<Reg8>(r8), value);
    }

    uint16_t Cpu::GetOperandR16(OperandR16 r16, InstructionResult &result)
    {
        return m_Regs.GetReg16(static_cast<Reg16>(r16));
    }

    void Cpu::SetOperandR16(OperandR16 r16, uint16_t value, InstructionResult &result)
    {
        m_Regs.SetReg16(static_cast<Reg16>(r16), value);
    }

    uint16_t Cpu::GetOperandR16Stk(OperandR16Stk r16stk, InstructionResult &result)
    {
        if (r16stk == OperandR16Stk::AF)
            return m_Regs.GetReg16(Reg16::AF);
 

        return m_Regs.GetReg16(static_cast<Reg16>(r16stk));
    }

    void Cpu::SetOperandR16Stk(OperandR16Stk r16stk, uint16_t value, InstructionResult &result)
    {
        if (r16stk == OperandR16Stk::AF)
        {
            m_Regs.SetReg16(Reg16::AF, value);
            return; 
        }
        m_Regs.SetReg16(static_cast<Reg16>(r16stk), value);
    }

    uint16_t Cpu::GetOperandR16Mem(OperandR16Mem r16mem, IMemory& memory, InstructionResult &result)
    {
        result.Cycles++;
        uint8_t value = 0;

        switch (r16mem)
        {
        case OperandR16Mem::HLI:
            value = GetReg16Adr(Reg16::HL, memory);
            _HLIncDec(true);
            break;
        case OperandR16Mem::HLD:
            value = GetReg16Adr(Reg16::HL, memory);
            _HLIncDec(false);
            break;
        default:
            value = GetReg16Adr(static_cast<Reg16>(r16mem), memory);
        }
        return value;
    }

    void Cpu::SetOperandR16Mem(OperandR16Mem r16mem, uint8_t value, IMemory& memory, InstructionResult &result)
    {
        result.Cycles++;
        switch (r16mem)
        {
        case OperandR16Mem::HLI:
            SetReg16Adr(Reg16::HL, value, memory);
            _HLIncDec(true);
            break;
        case OperandR16Mem::HLD:
            SetReg16Adr(Reg16::HL, value, memory);
            _HLIncDec(false);
            break;
        default:
            SetReg16Adr(static_cast<Reg16>(r16mem), value, memory);
        }
    }

    bool Cpu::CheckOperandCond(OperandCond cc)
    {
        switch (cc)
        {
        case OperandCond::Z :
            return m_Regs.GetFlag(CpuFlag::Z);
        case OperandCond::NZ:
            return !m_Regs.GetFlag(CpuFlag::Z);
        case OperandCond::C:
            return m_Regs.GetFlag(CpuFlag::C);
        case OperandCond::NC:
            return !m_Regs.GetFlag(CpuFlag::C);
        }

        return false;
    }

    uint8_t Cpu::GetImm8(IMemory &memory, InstructionResult &result)
    {
        uint8_t imm8 = memory.Get(m_Regs.GetReg16(Reg16::PC)); 

        result.Cycles++;
        _AddPC(1);

        return imm8;
    }

    uint16_t Cpu::GetImm16(IMemory &memory, InstructionResult &result)
    {
        uint16_t imm16 = memory.Get16(m_Regs.GetReg16(Reg16::PC));

        _AddPC(2);
        result.Cycles += 2;
        
        return imm16;
    }

    void Cpu::SetImm8(uint8_t imm8, IMemory &memory)
    {
        memory.Set(m_Regs.GetReg16(Reg16::PC), imm8);
    }

    void Cpu::SetImm16(uint16_t imm16, IMemory &memory)
    {
        memory.Set16(m_Regs.GetReg16(Reg16::PC), imm16);
    }
    
    void Cpu::LoadR16_Imm16(OperandR16 r16, IMemory &memory, InstructionResult &result)
    {
        // fetch
        int16_t imm16 = GetImm16(memory, result);

        // execute
        SetOperandR16(r16, imm16, result);
    }
    
    void Cpu::LoadR16Mem_A(OperandR16Mem r16mem, IMemory &memory, InstructionResult &result)
    {
        // fetch
        int8_t value = GetOperandR8(OperandR8::A, memory, result);

        // execute
        SetOperandR16Mem(r16mem, value, memory, result);
    }

    void Cpu::LoadA_R16Mem(OperandR16Mem r16mem, IMemory &memory, InstructionResult &result)
    {
        // fetch
        int8_t value = GetOperandR16Mem(r16mem, memory, result);
    
        // execute
        SetOperandR8(OperandR8::A, value, memory, result);
    }

    void Cpu::LoadAdrImm16_SP(IMemory &memory, InstructionResult &result)
    {
        // fetch
        int16_t imm16 = GetImm16(memory, result);
        int16_t value = GetOperandR16(OperandR16::SP, result);

        // execute
        memory.Set16(imm16, value);
        result.Cycles += 2;
    }

    void Cpu::LoadR8_Imm8(OperandR8 r8, IMemory &memory, InstructionResult &result)
    {
        // fetch
        int8_t imm8 = GetImm8(memory, result);

        // execute
        SetOperandR8(r8, imm8, memory, result);

    }

    void Cpu::LoadR8_R8(OperandR8 src8, OperandR8 dest8, IMemory &memory, InstructionResult &result)
    {
        // fetch
        int8_t value = GetOperandR8(src8, memory, result);

        // execute
        SetOperandR8(dest8, value, memory, result);
    }

    void Cpu::LoadHighC_A(IMemory &memory, InstructionResult &result)
    {
        // fetch
        int8_t src = m_Regs.GetReg8(Reg8::A);
        int16_t destAdr = 0xff00 + m_Regs.GetReg8(Reg8::C);

        // execute
        memory.Set(
            destAdr,
            src
        );
        result.Cycles++;
    }

    void Cpu::LoadA_HighC(IMemory &memory, InstructionResult &result)
    {
        // fetch
        int16_t srcAdr = 0xff00 + m_Regs.GetReg8(Reg8::C);
        int8_t src = memory.Get(srcAdr);
        result.Cycles++;

        // execute
        m_Regs.SetReg8(Reg8::A, src);
    }

    void Cpu::LoadHighImm8_A(IMemory &memory, InstructionResult &result)
    {
        // fetch
        int8_t src = m_Regs.GetReg8(Reg8::A);
        int16_t destAdr = 0xff00 + GetImm8(memory, result);

        // execute
        memory.Set(
            destAdr,
            src
        );
        result.Cycles++;
    }

    void Cpu::LoadA_HighImm8(IMemory &memory, InstructionResult &result)
    {
        // fetch
        int16_t srcAdr = 0xff00 + GetImm8(memory, result);
        int8_t src = memory.Get(srcAdr);
        result.Cycles++;

        // execute
        m_Regs.SetReg8(Reg8::A, src);
    }

    void Cpu::LoadAdrImm16_A(IMemory &memory, InstructionResult &result)
    {
        // fetch
        int8_t src = m_Regs.GetReg8(Reg8::A);
        int16_t destAdr = GetImm16(memory, result);

        // execute
        memory.Set(
            destAdr,
            src
        );
        result.Cycles++;
    }

    void Cpu::LoadA_AdrImm16(IMemory &memory, InstructionResult &result)
    {
        // fetch
        int16_t srcAdr = GetImm16(memory, result);
        int8_t src = memory.Get(srcAdr);
        result.Cycles++;

        // execute
        m_Regs.SetReg8(Reg8::A, src);
    }

    void Cpu::LoadHL_SPImm8(IMemory &memory, InstructionResult &result)
    {
        _AddToDestSP_Imm8(Reg16::HL, memory, result);
    }

    void Cpu::LoadSP_HL(InstructionResult &result)
    {
        // fetch
        uint16_t hl = m_Regs.GetReg16(Reg16::HL);

        // execute
        m_Regs.SetReg16(Reg16::SP, hl);

        // result
        result.Cycles++;
    }

    void Cpu::ExecAluOpR16(Alu::OperationDest16 op, OperandR16 r16, InstructionResult &result)
    {
        // fetch
        uint16_t value = GetOperandR16(r16, result);

        // execute
        AluResult aluResult{};
        op(value, aluResult);
        result.Cycles++;

        // save result
        SetOperandR16(r16, aluResult.Result16, result);
    }

    void Cpu::AddHL_R16(OperandR16 r16, InstructionResult &result)
    {
        // fetch
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

    void Cpu::ExecAluOpR8(Alu::OperationDest8 op, OperandR8 r8, IMemory &memory, InstructionResult &result)
    {
        // fetch
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


    void Cpu::ExecAluOpA_R8(Alu::OperationDestSrc8 op, OperandR8 r8, IMemory& memory, InstructionResult &result, bool addCarry)
    {
        // fetch 
        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        _ExecAluOpA(op, value, addCarry);
    }

    void Cpu::ExecAluOpA_Imm8(Alu::OperationDestSrc8 op, IMemory &memory, InstructionResult &result, bool addCarry)
    {
        // fetch
        uint8_t value = GetImm8(memory, result);

        // execute
        _ExecAluOpA(op, value, addCarry);
    }

    void Cpu::AddSP_Imm8(IMemory &memory, InstructionResult &result)
    {
        _AddToDestSP_Imm8(Reg16::SP, memory, result);
    }

    void Cpu::ComplementA(InstructionResult &result)
    {
        // fetch
        uint8_t a = m_Regs.GetReg8(Reg8::A);

        // execute
        m_Regs.SetReg8(Reg8::A, ~a);

        // result
        m_Regs.SetFlag(CpuFlag::H, true);
        m_Regs.SetFlag(CpuFlag::N, true);
    }

    void Cpu::TestBitR8(uint8_t bit, OperandR8 r8, IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        AluResult aluResult{};
        Alu::TestBit(bit, value, aluResult);
        
        // result
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::SetBitR8(uint8_t bit, OperandR8 r8, IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        AluResult aluResult{};
        Alu::SetBit(bit, value, aluResult);

        // result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::ResetBitR8(uint8_t bit, OperandR8 r8, IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t value = GetOperandR8(r8, memory, result);

        // execute
        AluResult aluResult{};
        Alu::ResetBit(bit, value, aluResult);

        // result
        SetOperandR8(r8, aluResult.Result8, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::RotateR8(Alu::OperationRotateSrc op, ShiftDirection direction, OperandR8 r8, IMemory &memory, InstructionResult &result, bool checkZero)
    {
        // fetch
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
        SetOperandR8(r8, value, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::ShiftR8(ShiftDirection direction, OperandR8 r8, IMemory &memory, InstructionResult &result, bool isLogical)
    {
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
        SetOperandR8(r8, value, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }
    
    void Cpu::SwapR8(OperandR8 r8, IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t value = GetOperandR8(r8, memory, result);

        // run alu swap op
        AluResult aluResult{};
        Alu::Swap(
            value,
            aluResult
        );

        // save result
        SetOperandR8(r8, value, memory, result);
        m_Regs.SetFlags(aluResult.AffectedFlags, aluResult.Flags);
    }

    void Cpu::Push(uint16_t value, IMemory &memory, InstructionResult &result)
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

    uint16_t Cpu::Pop(IMemory &memory, InstructionResult &result)
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

    uint16_t Cpu::Top(IMemory &memory)
    {
        uint16_t stack = m_Regs.GetReg16(Reg16::SP);
        return memory.Get16(stack);
    }

    void Cpu::PushR16Stk(OperandR16Stk r16stk, IMemory &memory, InstructionResult &result)
    {
        // fetch 
        uint16_t value = GetOperandR16Stk(r16stk, result);

        // execute
        Push(value, memory, result);

        // result
        SetOperandR16Stk(r16stk, value, result);
    }
    
    void Cpu::PopR16Stk(OperandR16Stk r16stk, IMemory &memory, InstructionResult &result)
    {
        // execute
        uint16_t top = Pop(memory, result);

        // result
        SetOperandR16Stk(r16stk, top, result);
    }

    void Cpu::_Call(uint16_t adr16, IMemory &memory, InstructionResult &result)
    {
        // push pc to stack
        uint16_t pc = m_Regs.GetReg16(Reg16::PC);
        Push(pc, memory, result);

        // set pc to adr
        m_Regs.SetReg16(Reg16::PC, adr16);
    }

    void Cpu::CallImm16(IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint16_t imm16 = GetImm16(memory, result);

        // execute
        _Call(imm16, memory, result);
    }

    void Cpu::CallCC_Imm16(OperandCond cc, IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint16_t imm16 = GetImm16(memory, result);

        // execute
        if (CheckOperandCond(cc))
            _Call(imm16, memory, result);
    }


    void Cpu::JumpHL(InstructionResult &result)
    {
        // fetch
        uint16_t adr16 = m_Regs.GetReg16(Reg16::HL);

        // execute
        m_Regs.SetReg16(Reg16::PC, adr16);
    }

    void Cpu::JumpImm16(IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint16_t adr16 = GetImm16(memory, result);

        // execute
        m_Regs.SetReg16(Reg16::PC, adr16);
        result.Cycles += 1;
    }
    
    void Cpu::JumpCC_Imm16(OperandCond cc, IMemory& memory, InstructionResult& result)
    {
        // fetch
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
        signedOffset16--;

        // offset pc
        uint16_t pc = m_Regs.GetReg16(Reg16::PC) + signedOffset16;
        m_Regs.SetReg16(Reg16::PC, pc);
        result.Cycles++;
    }

    void Cpu::_AddToDestSP_Imm8(Reg16 dest, IMemory &memory, InstructionResult &result)
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


    void Cpu::JumpRelativeImm8(IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t offset = GetImm8(memory, result);

        // execute
        _JumpRelative(offset, result);
    }
    
    void Cpu::JumpRelativeCC_Imm8(OperandCond cc, IMemory &memory, InstructionResult &result)
    {
        // fetch
        uint8_t offset = GetImm8(memory, result);

        // execute
        if (CheckOperandCond(cc))
            _JumpRelative(offset, result);
    }

    void Cpu::Return(IMemory &memory, InstructionResult &result)
    {
        // pop top adress
        uint16_t topAdr16 = Pop(memory, result);

        // jump to adress
        m_Regs.SetReg16(Reg16::PC, topAdr16);
        result.Cycles++;
    }
    
    void Cpu::ReturnCC(OperandCond cc, IMemory &memory, InstructionResult &result)
    {
        result.Cycles++;

        if (CheckOperandCond(cc))
            Return(memory, result);
    }
    
    void Cpu::ReturnAndEnableInterrupts(IMemory &memory, InstructionResult &result)
    {
        // TODO: Add enbale interrupts
        EnableInterrupts(result);
        Return(memory, result);
    }
    
    void Cpu::RstVec(uint8_t tgt3, IMemory &memory, InstructionResult &result)
    {
        uint16_t adr16 = static_cast<uint16_t>(tgt3) * 8;

        _Call(adr16, memory, result);
    }
    
    void Cpu::ComplementCarryFlag(InstructionResult &result)
    {
        // reset N and H Flags
        m_Regs.SetFlag(CpuFlag::N, false);
        m_Regs.SetFlag(CpuFlag::H, false);

        // Invert carry flag
        bool carryFlag = (m_Regs.GetFlags() & CpuFlag::C); 
        m_Regs.SetFlag(CpuFlag::C, !carryFlag);
    }

    void Cpu::SetCarryFlag(InstructionResult &result)
    {
        // reset N and H Flags
        m_Regs.SetFlag(CpuFlag::N, false);
        m_Regs.SetFlag(CpuFlag::H, false);

        // set C
        m_Regs.SetFlag(CpuFlag::C, true);
    }

    void Cpu::DisableInterrupts(InstructionResult &result)
    {
        m_IME = false;
        m_QueueIME = 0;
    }

    void Cpu::EnableInterrupts(InstructionResult & result)
    {
        if (m_QueueIME == 0 && !m_IME)
            m_QueueIME = 2;
    }

    void Cpu::DecimalAdjustAccumulator(InstructionResult &result)
    {
        // fetch
        uint8_t a = m_Regs.GetReg8(Reg8::A);

        // execute
        uint8_t adjust = 0;

        if (m_Regs.GetFlag(CpuFlag::N))
        {
            if (m_Regs.GetFlag(CpuFlag::H))
                adjust += 0x6;

            if (m_Regs.GetFlag(CpuFlag::C))
                adjust += 0x60;

            a -= adjust;
        }
        else
        {
            if (m_Regs.GetFlag(CpuFlag::H) || (a & 0xf) > 0x9)
                adjust += 0x6;

            if (m_Regs.GetFlag(CpuFlag::C) || a > 0x99)
            {
                adjust += 0x60;
                m_Regs.SetFlag(CpuFlag::C, true);
            }

            a += adjust;
        }

        // result
        m_Regs.SetReg8(Reg8::A, a);
        m_Regs.SetFlag(CpuFlag::Z, a == 0);
        m_Regs.SetFlag(CpuFlag::H, false);
    }

    void Cpu::_HandleIME()
    {
        
        if (m_IME || m_QueueIME <= 0)
            return;

        m_QueueIME--;
        if (m_QueueIME == 0)
            m_IME = true;
    }

    bool Cpu::_HandleInterrupts(IMemory &memory, InstructionResult &result)
    {
        if (!m_IME)
            return false;

        // list of interrupt flags ordered by priority
        std::vector<InterruptFlag> flags = {
            InterruptFlag::V_BLANK,
            InterruptFlag::LCD,
            InterruptFlag::TIMER,
            InterruptFlag::SERIAL,
            InterruptFlag::JOYPAD
        };

        for (auto flag: flags)
        {
            if (_HandleInterruptFlag(flag, memory, result))
                return true;
        }

        return false;
    }

    bool Cpu::_HandleInterruptFlag(InterruptFlag flag, IMemory &memory, InstructionResult &result)
    {
        // cast flag to uint8_t
        uint8_t flagBit = static_cast<uint8_t>(flag);

        // get interrupt flags
        uint8_t interruptEnable = memory.Get(static_cast<uint16_t>(IORegister::IE));
        uint8_t interruptFlag = memory.Get(static_cast<uint16_t>(IORegister::IF));

        // check interrupt enable
        if (!Binary::TestBit(interruptEnable, flagBit))
            return false;

        // check interrupt flag
        if (!Binary::TestBit(interruptFlag, flagBit))
            return false;

        // get handler address
        uint16_t handler = 0x40 + 0x8 * static_cast<uint16_t>(flagBit);

        // handle interrupt
        m_IME = false; // disable IME flag
        interruptFlag = Binary::ResetBit(interruptFlag, flagBit); // reset interrupt flag
        memory.Set(static_cast<uint16_t>(IORegister::IF), interruptFlag);
        _Call(handler, memory, result); // call interrupt

        // result
        result.Cycles = 5; // hard code 5 cycles it's easier that way
        result.Asm.SetOperation("interrupt");
        result.Asm.AddImm16(handler, true);

        return true;
    }
} // namespace GBE
