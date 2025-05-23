#pragma once

#include "CpuRegistersSet.h"
#include "Alu.h"
#include "CpuOperand.h"

namespace GBE
{
    class IMemory;
    class InstructionResult;

    // cpu of the game boy
    // executes uintruction set
    class Cpu
    {
    public:
        ~Cpu();

        // run current instruction
        uint16_t Run(IMemory &memory, InstructionResult& result);


        // get registers
        inline CpuRegistersSet& GetRegisters() 
        {
            return m_Regs;
        }

        /*
            Get/Set Instruction operands
        */

        // get value at register address
        uint8_t GetReg16Adr(Reg16 adr, IMemory& memory) const;

        // set value at register adress
        void SetReg16Adr(Reg16 adr, uint8_t value, IMemory& memory);

        /*
            Get/ Set operand
        */
        uint8_t GetOperandR8(OperandR8 r8, IMemory& memory, InstructionResult& result);
        void SetOperandR8(OperandR8 r8, uint8_t value, IMemory &memory, InstructionResult &result);

        uint16_t GetOperandR16(OperandR16 r16, InstructionResult &result);
        void SetOperandR16(OperandR16 r16, uint16_t value, InstructionResult &result);

        uint16_t GetOperandR16Stk(OperandR16Stk r16stk, InstructionResult &result);
        void SetOperandR16Stk(OperandR16Stk r16stk, uint16_t value, InstructionResult &result);

        uint16_t GetOperandR16Mem(OperandR16Mem r16mem, IMemory &memory, InstructionResult &result);
        void SetOperandR16Mem(OperandR16Mem r16mem, uint8_t value, IMemory &memory, InstructionResult &result);

        // check operand is true relative to flags
        bool CheckOperandCond(OperandCond cc);

        /*
            Get/Set Immediate data 
        */

        // get immediate 8bits
        uint8_t GetImm8(IMemory& memory, InstructionResult& result);

        // get immediate 16bits
        uint16_t GetImm16(IMemory &memory, InstructionResult &result);

        // set immediate 8bits
        void SetImm8(uint8_t imm8, IMemory &memory);

        // set immediate 16bits
        void SetImm16(uint16_t imm16, IMemory &memory);

        /*
            Load Operations
        */

        // ld r16, imm16
        void LoadR16_Imm16(OperandR16 r16, IMemory &memory, InstructionResult &result);

        // ld [r16mem], a
        void LoadR16Mem_A(OperandR16Mem r16mem, IMemory& memory, InstructionResult& result);

        // ld a, [r16mem]
        void LoadA_R16Mem(OperandR16Mem r16mem, IMemory& memory, InstructionResult& result);

        // ld [imm16], sp
        void LoadAdrImm16_SP(IMemory& memory, InstructionResult& result);

        // ld r8, imm8
        void LoadR8_Imm8(OperandR8 r8, IMemory& memory, InstructionResult& result);

        // ld r8, r8
        void LoadR8_R8(OperandR8 src8, OperandR8 dest8, IMemory& memory, InstructionResult& result);

        // ld [$ff00 + c], a
        void LoadHighC_A(IMemory &memory, InstructionResult &result);

        // ld a, [$ff00 + c]
        void LoadA_HighC(IMemory &memory, InstructionResult &result);

        // ld [$ff00 + imm8], a
        void LoadHighImm8_A(IMemory &memory, InstructionResult &result);

        // ld a, [$ff00 + imm8]
        void LoadA_HighImm8(IMemory &memory, InstructionResult &result);

        // ld [imm16], a
        void LoadAdrImm16_A(IMemory& memory, InstructionResult &result);

        // ld a, [imm16]
        void LoadA_AdrImm16(IMemory &memory, InstructionResult &result);

        /* 
            ALU Operations
        */ 

        // op r16
        void ExecAluOpR16(Alu::OperationDest16 op, OperandR16 r16, InstructionResult &result);

        // add hl, r16
        void AddHL_R16(OperandR16 r16, InstructionResult &result);

        // op r8
        void ExecAluOpR8(Alu::OperationDest8 op, OperandR8 r8, IMemory& memory, InstructionResult &result);

        // op A, r8
        void ExecAluOpA_R8(Alu::OperationDestSrc8 op, OperandR8 r8, IMemory& memory, InstructionResult &result, bool addCarry = false);

        // op A, imm8
        void ExecAluOpA_R8(Alu::OperationDestSrc8 op, IMemory &memory, InstructionResult &result, bool addCarry = false);

        // add SP, Imm8
        void AddSP_Imm8(IMemory &memory, InstructionResult &result);

        // cpl A
        void ComplementA(InstructionResult& result);

        /*
            Bit manimulation
        */

        // bit r3, r8
        void TestBitR8(uint8_t bit, OperandR8 r8, IMemory& memory, InstructionResult &result);

        // set r3, r8
        void SetBitR8(uint8_t bit, OperandR8 r8, IMemory &memory, InstructionResult &result);

        // res r3, r8
        void ResetBitR8(uint8_t bit, OperandR8 r8, IMemory &memory, InstructionResult &result);

        /*
            Shift operations 
        */

        // rl/rr/rlc/rrc R8
        void RotateR8(Alu::OperationRotateSrc op, ShiftDirection direction, OperandR8 r8, IMemory& memory, InstructionResult& result, bool checkZero = true);

        // sla/sra/srl R8
        void ShiftR8(ShiftDirection direction, OperandR8 r8, IMemory &memory, InstructionResult &result, bool isLogical = false);

        // swap r8
        void SwapR8(OperandR8 r8, IMemory &memory, InstructionResult &result);

        /*
            Stack manipulation
        */

        // push value to stack
        void Push(uint16_t value, IMemory &memory, InstructionResult &result);

        // pop value from stack
        uint16_t Pop(IMemory &memory, InstructionResult &result);

        // get top value of the stack
        uint16_t Top(IMemory& memory);

        // push r16stk
        void PushR16Stk(OperandR16Stk r16stk, IMemory& memory, InstructionResult& result);

        // pop r16stk
        void PopR16Stk(OperandR16Stk r16stk, IMemory &memory, InstructionResult &result);

        /*
            Jumps and subroutine instructions
        */
        // call imm16
        void CallImm16(IMemory& memory, InstructionResult& result);

        // call cc, imm16
        void CallCC_Imm16(OperandCond cc, IMemory &memory, InstructionResult &result);

        // jp HL
        void JumpHL(InstructionResult& result);

        // jp imm16
        void JumpImm16(IMemory& memory, InstructionResult& result);

        // jp cc, imm16
        void JumpCC_Imm16(OperandCond cc, IMemory& memory, InstructionResult& result);

        // jr imm16
        void JumpRelativeImm16(IMemory& memory, InstructionResult& result);

        // jr cc, imm16
        void JumpRelativeCC_Imm16(OperandCond cc, IMemory &memory, InstructionResult &result);

        // ret 
        void Return(IMemory& memory, InstructionResult& result);

        // ret cc
        void ReturnCC(OperandCond cc, IMemory &memory, InstructionResult &result);

        // reti
        void ReturnAndEnableInterrupts(IMemory& memory, InstructionResult& result);
    
        // rst vec
        void RstVec(uint8_t tgt3, IMemory& memory, InstructionResult& result);
    
        // ccf
        void ComplementCarryFlag(InstructionResult &result);

        // scf
        void SetCarryFlag(InstructionResult &result);

        // di
        void DisableInterrupts(IMemory& memory, InstructionResult& result);

        // ei
        void EnableInterrupts(IMemory &memory, InstructionResult &result);
        
    private: 
        CpuRegistersSet m_Regs;

        // add value to pc to move to next intruction
        inline void _AddPC(uint16_t bytes)
        {
            m_Regs.SetReg16(
                Reg16::PC, 
                m_Regs.GetReg16(Reg16::PC) + bytes
            );
        }

        // get carry flag
        inline uint8_t _GetCarry() const 
        {
            return (m_Regs.GetFlags() & CpuFlag::C) ? 1: 0;
        } 

        // hli/hld
        void _HLIncDec(bool isInc);

        // do operation on A 
        void _ExecAluOpA(Alu::OperationDestSrc8 op, uint8_t v8, bool addCarry);
    
        // call to adr16
        void _Call(uint16_t adr16, IMemory &memory, InstructionResult &result);
    
        // jump relative
        void _JumpRelative(uint8_t offset8, InstructionResult& result);
    
        // 
    };
} // namespace GBE
