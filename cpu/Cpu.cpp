#include "Cpu.h"

#include "instruction/InstructionResult.h"

#include "alu/AluResult.h"
#include "alu/Alu.h"

#include "memory/Memory.h"
#include "util/Binary.h" 
#include "io/IORegister.h"

#include <magic_enum.hpp>
#include <iostream>
#include <exception>


namespace GBE
{
    Cpu::~Cpu()
    {   
    }

    void Cpu::Init()
    {
        m_Regs.SetReg8(Reg8::A, 0x01);
        m_Regs.SetFlags(CpuFlag::C | CpuFlag::Z | CpuFlag::H | CpuFlag::N, 0x0);
        
        m_Regs.SetReg8(Reg8::B, 0xFF);
        m_Regs.SetReg8(Reg8::C, 0x13);
        
        m_Regs.SetReg8(Reg8::D, 0x00);
        m_Regs.SetReg8(Reg8::E, 0xC1);
        
        m_Regs.SetReg8(Reg8::H, 0x84);
        m_Regs.SetReg8(Reg8::L, 0x03);

        m_Regs.SetReg16(Reg16::PC, 0x0100);
        m_Regs.SetReg16(Reg16::SP, 0xFFFE);
    }

    uint8_t Cpu::GetReg16Adr(Reg16 adr, Memory &memory) const
    {
        return memory.Get(m_Regs.GetReg16(adr));
    }

    void Cpu::SetReg16Adr(Reg16 adr, uint8_t value, Memory &memory)
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

    uint8_t Cpu::GetOperandR8(OperandR8 r8, Memory &memory, InstructionResult &result)
    {
        if (r8 == OperandR8::ADR_HL)
        {
            result.Cycles++;
            return GetReg16Adr(Reg16::HL, memory);
        }

        return m_Regs.GetReg8(static_cast<Reg8>(r8));
    }

    void Cpu::SetOperandR8(OperandR8 r8, uint8_t value, Memory &memory, InstructionResult &result)
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

    uint16_t Cpu::GetOperandR16Mem(OperandR16Mem r16mem, Memory& memory, InstructionResult &result)
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

    void Cpu::SetOperandR16Mem(OperandR16Mem r16mem, uint8_t value, Memory& memory, InstructionResult &result)
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

    uint8_t Cpu::GetImm8(Memory &memory, InstructionResult &result)
    {
        uint8_t imm8 = memory.Get(m_Regs.GetReg16(Reg16::PC)); 

        result.Cycles++;
        _AddPC(1);

        return imm8;
    }

    uint16_t Cpu::GetImm16(Memory &memory, InstructionResult &result)
    {
        uint16_t imm16 = memory.Get16(m_Regs.GetReg16(Reg16::PC));

        _AddPC(2);
        result.Cycles += 2;
        
        return imm16;
    }

    void Cpu::SetImm8(uint8_t imm8, Memory &memory)
    {
        memory.Set(m_Regs.GetReg16(Reg16::PC), imm8);
    }

    void Cpu::SetImm16(uint16_t imm16, Memory &memory)
    {
        memory.Set16(m_Regs.GetReg16(Reg16::PC), imm16);
    }

    void Cpu::ComplementCarryFlag(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // reset N and H Flags
        m_Regs.SetFlag(CpuFlag::N, false);
        m_Regs.SetFlag(CpuFlag::H, false);

        // Invert carry flag
        bool carryFlag = (m_Regs.GetFlags() & CpuFlag::C); 
        m_Regs.SetFlag(CpuFlag::C, !carryFlag);
    }

    void Cpu::SetCarryFlag(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        // reset N and H Flags
        m_Regs.SetFlag(CpuFlag::N, false);
        m_Regs.SetFlag(CpuFlag::H, false);

        // set C
        m_Regs.SetFlag(CpuFlag::C, true);
    }

    void Cpu::DisableInterrupts(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        m_IME = false;
        m_QueueIME = 0;
    }

    void Cpu::EnableInterrupts(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
        if (m_QueueIME == 0 && !m_IME)
            m_QueueIME = 2;
    }

    void Cpu::DecimalAdjustAccumulator(const Instruction &instr, Memory &memory, InstructionResult &result)
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
            if (m_Regs.GetFlag(CpuFlag::H) || (a & 0x0f) > 0x9)
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

    void Cpu::Stop(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
    }

    void Cpu::Halt(const Instruction &instr, Memory &memory, InstructionResult &result)
    {
    }

    void Cpu::_HandleIME()
    {
        if (m_IME || m_QueueIME <= 0)
            return;

        m_QueueIME--;
        if (m_QueueIME == 0)
            m_IME = true;
    }

    bool Cpu::_HandleInterrupts(Memory &memory, InstructionResult &result)
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
            {
                // std::cout << "Exception: " << magic_enum::enum_name(flag) << "\n";
                return true;
            }
        }

        return false;
    }

    bool Cpu::_HandleInterruptFlag(InterruptFlag flag, Memory &memory, InstructionResult &result)
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

        return true;
    }
} // namespace GBE
