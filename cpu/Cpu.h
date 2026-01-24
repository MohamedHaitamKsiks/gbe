#pragma once

#include "registers/CpuRegistersSet.h"
#include "alu/Alu.h"
#include "instruction/InstructionDecoder.h"

#include "io/interrupts/InterruptFlag.h"

namespace GBE
{
    class Memory;
    class InstructionResult;

    // cpu of the game boy
    // executes uintruction set
    class Cpu
    {
    public:
        ~Cpu();

        // init after boot load
        void Init();

        // run current instruction
        void Run(Memory &memory, InstructionResult& result);

        // get registers
        inline CpuRegistersSet& GetRegisters() 
        {
            return m_Regs;
        }   

        /*
            Get/Set Instruction operands
        */

        // get value at register address
        uint8_t GetReg16Adr(Reg16 adr, Memory& memory) const;

        // set value at register adress
        void SetReg16Adr(Reg16 adr, uint8_t value, Memory& memory);

        /*
            Get/ Set operand
        */
        uint8_t GetOperandR8(OperandR8 r8, Memory& memory, InstructionResult& result);
        void SetOperandR8(OperandR8 r8, uint8_t value, Memory &memory, InstructionResult &result);

        uint16_t GetOperandR16(OperandR16 r16, InstructionResult &result);
        void SetOperandR16(OperandR16 r16, uint16_t value, InstructionResult &result);

        uint16_t GetOperandR16Stk(OperandR16Stk r16stk, InstructionResult &result);
        void SetOperandR16Stk(OperandR16Stk r16stk, uint16_t value, InstructionResult &result);

        uint16_t GetOperandR16Mem(OperandR16Mem r16mem, Memory &memory, InstructionResult &result);
        void SetOperandR16Mem(OperandR16Mem r16mem, uint8_t value, Memory &memory, InstructionResult &result);

        // check operand is true relative to flags
        bool CheckOperandCond(OperandCond cc);

        /*
            Get/Set Immediate data 
        */

        // get immediate 8bits
        uint8_t GetImm8(Memory& memory, InstructionResult& result);

        // get immediate 16bits
        uint16_t GetImm16(Memory &memory, InstructionResult &result);

        // set immediate 8bits
        void SetImm8(uint8_t imm8, Memory &memory);

        // set immediate 16bits
        void SetImm16(uint16_t imm16, Memory &memory);

        /*
            Nop
        */

        inline void Nop(const Instruction &instr, Memory &memory, InstructionResult &result) {}

        /*
            Load Operations
        */

        // ld r16, imm16
        void LoadR16_Imm16(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld [r16mem], a
        void LoadR16Mem_A(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld a, [r16mem]
        void LoadA_R16Mem(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld [imm16], sp
        void LoadAdrImm16_SP(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld r8, imm8
        void LoadR8_Imm8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld r8, r8
        void LoadR8_R8(const Instruction& instr, Memory& memory, InstructionResult& result);

        // ld [$ff00 + c], a
        void LoadHighC_A(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld a, [$ff00 + c]
        void LoadA_HighC(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld [$ff00 + imm8], a
        void LoadHighImm8_A(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld a, [$ff00 + imm8]
        void LoadA_HighImm8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld [imm16], a
        void LoadAdrImm16_A(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld a, [imm16]
        void LoadA_AdrImm16(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld HL, sp + imm8
        void LoadHL_SPImm8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ld sp, hl
        void LoadSP_HL(const Instruction &instr, Memory &memory, InstructionResult &result);

        /* 
            ALU Operations
        */ 

        // cpl A
        void ComplementA(const Instruction &instr, Memory &memory, InstructionResult &result);

        // add hl, r16
        void AddHL_R16(const Instruction &instr, Memory &memory, InstructionResult &result);

        // add SP, Imm8
        void AddSP_Imm8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // op r16
        void ExecAluOpR16(Alu::OperationDest16 op, const Instruction &instr, Memory &memory, InstructionResult &result);
        template <Alu::OperationDest16 op>
        inline void ExecAluOpR16(const Instruction &instr, Memory &memory, InstructionResult &result)
        {
            ExecAluOpR16(op, instr, memory, result);
        }

        // op r8
        void ExecAluOpR8(Alu::OperationDest8 op, const Instruction &instr, Memory &memory, InstructionResult &result);
        template <Alu::OperationDest8 op>
        inline void ExecAluOpR8(const Instruction &instr, Memory &memory, InstructionResult &result)
        {
            ExecAluOpR8(op, instr, memory, result);
        }

        // op A, r8
        void ExecAluOpA_R8(Alu::OperationDestSrc8 op, bool addCarry, const Instruction &instr, Memory &memory, InstructionResult &result);
        template <Alu::OperationDestSrc8 op, bool addCarry>
        inline void ExecAluOpA_R8(const Instruction &instr, Memory &memory, InstructionResult &result)
        {
            ExecAluOpA_R8(op, addCarry, instr, memory, result);
        }

        // op A, imm8
        void ExecAluOpA_Imm8(Alu::OperationDestSrc8 op, bool addCarry, const Instruction &instr, Memory &memory, InstructionResult &result);
        template <Alu::OperationDestSrc8 op, bool addCarry>
        inline void ExecAluOpA_Imm8(const Instruction &instr, Memory &memory, InstructionResult &result)
        {
            ExecAluOpA_Imm8(op, addCarry, instr, memory, result);
        }

        /*
            Bit manimulation
        */

        // bit r3, r8
        void TestBitR8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // set r3, r8
        void SetBitR8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // res r3, r8
        void ResetBitR8(const Instruction &instr, Memory &memory, InstructionResult &result);

        /*
            Shift operations 
        */

        // rl/rr/rlc/rrc R8
        void RotateR8(Alu::OperationRotateSrc op, ShiftDirection direction, OperandR8 r8, Memory& memory, InstructionResult& result, bool checkZero = true);

        // sla/sra/srl R8
        void ShiftR8(ShiftDirection direction, OperandR8 r8, Memory &memory, InstructionResult &result, bool isLogical = false);

        // swap r8
        void SwapR8(OperandR8 r8, Memory &memory, InstructionResult &result);

        /*
            Stack manipulation
        */

        // push value to stack
        void Push(uint16_t value, Memory &memory, InstructionResult &result);

        // pop value from stack
        uint16_t Pop(Memory &memory, InstructionResult &result);

        // get top value of the stack
        uint16_t Top(Memory& memory);

        // push r16stk
        void PushR16Stk(const Instruction &instr, Memory &memory, InstructionResult &result);

        // pop r16stk
        void PopR16Stk(const Instruction &instr, Memory &memory, InstructionResult &result);

        /*
            Jumps and subroutine instructions
        */

        // call imm16
        void CallImm16(const Instruction &instr, Memory &memory, InstructionResult &result);

        // call cc, imm16
        void CallCC_Imm16(const Instruction &instr, Memory &memory, InstructionResult &result);

        // jp HL
        void JumpHL(const Instruction &instr, Memory &memory, InstructionResult &result);

        // jp imm16
        void JumpImm16(const Instruction &instr, Memory &memory, InstructionResult &result);

        // jp cc, imm16
        void JumpCC_Imm16(const Instruction &instr, Memory &memory, InstructionResult &result);

        // jr imm16
        void JumpRelativeImm8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // jr cc, imm16
        void JumpRelativeCC_Imm8(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ret
        void Return(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ret cc
        void ReturnCC(const Instruction &instr, Memory &memory, InstructionResult &result);

        // reti
        void ReturnAndEnableInterrupts(const Instruction &instr, Memory &memory, InstructionResult &result);

        // rst vec
        void RstVec(const Instruction &instr, Memory &memory, InstructionResult &result);

        /* Others */

        // ccf
        void ComplementCarryFlag(const Instruction &instr, Memory &memory, InstructionResult &result);

        // scf
        void SetCarryFlag(const Instruction &instr, Memory &memory, InstructionResult &result);

        // di
        void DisableInterrupts(const Instruction &instr, Memory &memory, InstructionResult &result);

        // ei
        void EnableInterrupts(const Instruction &instr, Memory &memory, InstructionResult &result);

        // is IME flag active
        inline bool GetIME() const
        {
            return m_IME;
        }

        // daa
        void DecimalAdjustAccumulator(const Instruction &instr, Memory &memory, InstructionResult &result);

    private: 
        InstructionDecoder m_Decoder{};
        CpuRegistersSet m_Regs{};

        // Flag to enable interrupts
        bool m_IME = false; // Interrupt master enable flag [write only]
        int32_t m_QueueIME = 0; // Are we queuing IME to be set in the next instruction

        // handle IME flag
        void _HandleIME();

        // handle interrupt and return if interrupt found
        bool _HandleInterrupts(Memory &memory, InstructionResult &result);

        // handle one interrupt flag and return if interrupt is found
        bool _HandleInterruptFlag(InterruptFlag flag, Memory &memory, InstructionResult &result);

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
            return (m_Regs.GetFlag(CpuFlag::C)) ? 1: 0;
        } 

        // hli/hld
        void _HLIncDec(bool isInc);

        // do operation on A 
        void _ExecAluOpA(Alu::OperationDestSrc8 op, uint8_t v8, bool addCarry);
    
        // call to adr16
        void _Call(uint16_t adr16, Memory &memory, InstructionResult &result);
    
        // jump relative
        void _JumpRelative(uint8_t offset8, InstructionResult& result);
    
        // offset SP with imm8 and load to register
        void _AddToDestSP_Imm8(Reg16 dest, Memory &memory, InstructionResult &result);
    
        // run instrunction
        void _RunInstruction(const Instruction &instr, Memory &memory, InstructionResult &result);

        // run prefix instruction
        void _RunPrefixInstruction(Memory &memory, InstructionResult &result);
    };
} // namespace GBE
