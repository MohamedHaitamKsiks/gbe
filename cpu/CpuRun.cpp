#include "Cpu.h"

#include "InstructionResult.h"
#include "AluResult.h"
#include "Alu.h"

#include "memory/IMemory.h"

#include <iostream>
#include <exception>


namespace GBE
{

    void Cpu::Run(IMemory &memory, InstructionResult &result)
    {
        // check if interruption is pending
        if (_HandleInterrupts(memory, result))
            return;

        // handle instruction
        result.Cycles = 0;
        uint8_t opcode = GetImm8(memory, result);

        result.Asm.SetAddress(m_Regs.GetReg16(Reg16::PC));
        result.Asm.SetOpcode(opcode);

        // read block instruction (inspired by: https://gbdev.io/pandocs/CPU_Instruction_Set.html)
        // block id is the last 2 bits (6 and 7)
        uint8_t blockId = opcode >> 6;

        switch (blockId)
        {
        // Block 0
        case 0:
            _RunBlock0(opcode, memory, result);
            break;
        // Block 1 8-bit register-to-register loads
        case 1:
            _RunBlock1(opcode, memory, result);
            break;
        // Block 2: 8-bit arithmetic
        case 2:
            _RunBlock2(opcode, memory, result);
            break;
        // Block 3
        case 3:
            _RunBlock3(opcode, memory, result);
            break;
        }

        // handle queue TME
        _HandleIME();

    }

    // Block 0
    void Cpu::_RunBlock0(uint8_t opcode, IMemory &memory, InstructionResult &result)
    {
        // check defined opcodes
        switch (opcode)
        {
        // nop
        case 0x0:
            Nop(result);
            return;
        // ld [imm16], sp
        case 0x8:
            LoadAdrImm16_SP(memory, result);
            return;
        // rlca
        case 0x7:
            RotateR8(Alu::RotateCarry, ShiftDirection::LEFT, OperandR8::A, memory, result, false);
            return;
        // rrca
        case 0xF:
            RotateR8(Alu::RotateCarry, ShiftDirection::RIGHT, OperandR8::A, memory, result, false);
            return;
        // rla
        case 0x17:
            RotateR8(Alu::Rotate, ShiftDirection::LEFT, OperandR8::A, memory, result, false);
            return;
        // rra
        case 0x1F:
            RotateR8(Alu::Rotate, ShiftDirection::RIGHT, OperandR8::A, memory, result, false);
            return;
        // TODO daa
        case 0x27:
            return;
        // cpl
        case 0x2F:
            ComplementA(result);
            return;
        // scf
        case 0x3F:
            SetCarryFlag(result);
            return;
        // ccf
        case 0x47:
            ComplementCarryFlag(result);
            return;
        // jr imm8
        case 0x4F:
            JumpRelativeImm8(memory, result);
            return;
        // TODO: stop
        case 0x57:
            return;
        }

        // check r16 operations (operand is on the 4-5 bits) and the opcode is the first 4 bits
        // first 4 bits
        uint8_t r16Opcode = opcode & 0xF;
        uint8_t r16Operand = (opcode >> 4) & 3;

        switch (r16Opcode)
        {
        // ld r16, imm16
        case 0x1:
            LoadR16_Imm16(static_cast<OperandR16>(r16Operand), memory, result);
            return;
        // ld [r16mem], a
        case 0x2:
            LoadR16Mem_A(static_cast<OperandR16Mem>(r16Operand), memory, result);
            return;
        // ld a, [r16mem]
        case 0xA:
            LoadA_R16Mem(static_cast<OperandR16Mem>(r16Operand), memory, result);
            return;
        // inc r16
        case 0x3:
            ExecAluOpR16(Alu::Increment16, static_cast<OperandR16>(r16Operand), result);
            return;
        // dec r16
        case 0xB:
            ExecAluOpR16(Alu::Decrement16, static_cast<OperandR16>(r16Operand), result);
            return;
        // add hl, r16
        case 0x9:
            AddHL_R16(static_cast<OperandR16>(r16Operand), result);
            return;
        }

        // now check for operations on r8
        // the opcode is the first 3bits
        uint8_t r8Opcode = opcode & 0x7;
        OperandR8 r8Operand = static_cast<OperandR8>((opcode >> 3) & 0x7);

        switch (r8Opcode)
        {
        // inc r8
        case 0x4:
            ExecAluOpR8(Alu::Increment8, r8Operand, memory, result);
            return;
        // dec r8
        case 0x5:
            ExecAluOpR8(Alu::Decrement8, r8Operand, memory, result);
            return;
        // ld r8, imm8
        case 0x6:
            LoadR8_Imm8(r8Operand, memory, result);
            return;
        }

        // check for jr cond, imm8
        if ((r8Opcode == 0) && (opcode & 0x20))
        {
            // get bits 3-4
            OperandCond cc = static_cast<OperandCond>((opcode & 0x18) >> 3);
            JumpRelativeCC_Imm8(cc, memory, result);
            return;
        }

        // if no instruction was found throw an error that the instruction is unrecognized
        _ThrowInvalidInstructtion();
    }

    // Block 1 8-bit register-to-register loads
    void Cpu::_RunBlock1(uint8_t opcode, IMemory &memory, InstructionResult &result)
    {
        // TODO halt 
        if (opcode == 0x76)
        {
            return;
        }

        // dest (next 3 bits)
        OperandR8 dest8 = static_cast<OperandR8>((opcode >> 3) & 0x7);

        // src (first 3 bits)
        OperandR8 src8 = static_cast<OperandR8>(opcode & 0x7);

        // ld r8, r8
        LoadR8_R8(src8, dest8, memory, result);
    }

    // Block 2: 8-bit arithmetic
    void Cpu::_RunBlock2(uint8_t opcode, IMemory &memory, InstructionResult &result)
    {
        // operand is first 3 bits
        // and opcode is retrieved from bits 5-3
        OperandR8 r8 = static_cast<OperandR8>(opcode & 0x7);
        uint8_t r8Opcode = (opcode >> 3) & 0x7;

        switch (r8Opcode)
        {
        // add a, r8
        case 0x0:
            ExecAluOpA_R8(Alu::Add8, r8, memory, result);
            return;
        // adc a, r8
        case 0x1:
            ExecAluOpA_R8(Alu::Add8, r8, memory, result, true);
            return;
        // sub a, r8
        case 0x2:
            ExecAluOpA_R8(Alu::Sub8, r8, memory, result);
            return;
        // sbc a, r8
        case 0x3:
            ExecAluOpA_R8(Alu::Sub8, r8, memory, result, true);
            return;
        // and a, r8
        case 0x4:
            ExecAluOpA_R8(Alu::And8, r8, memory, result);
            return;
        // xor a, r8
        case 0x5:
            ExecAluOpA_R8(Alu::Xor8, r8, memory, result);
            return;
        // or  a, r8
        case 0x6:
            ExecAluOpA_R8(Alu::Or8, r8, memory, result);
            return;
        // cp  a, r8
        case 0x7:
            ExecAluOpA_R8(Alu::Cmp8, r8, memory, result);
            return;
        }

        _ThrowInvalidInstructtion();
    }

    // Block 3
    void Cpu::_RunBlock3(uint8_t opcode, IMemory &memory, InstructionResult &result)
    {
        // predefined opcodes
        switch (opcode)
        {
        // add a, imm8
        case 0xC6:
            ExecAluOpA_Imm8(Alu::Add8, memory, result);
            return;
        // adc a, imm8
        case 0xCE:
            ExecAluOpA_Imm8(Alu::Add8, memory, result, true);
            return;
        // sub a, imm8
        case 0xD6:
            ExecAluOpA_Imm8(Alu::Sub8, memory, result);
            return;
        // sbc a, imm8
        case 0xDE:
            ExecAluOpA_Imm8(Alu::Sub8, memory, result, true);
            return;
        // and a, imm8
        case 0xE6:
            ExecAluOpA_Imm8(Alu::And8, memory, result);
            return;
        // xor a, imm8
        case 0xEE:
            ExecAluOpA_Imm8(Alu::Xor8, memory, result);
            return;
        // or a, imm8
        case 0xF6:
            ExecAluOpA_Imm8(Alu::Or8, memory, result);
            return;
        // cp a, imm8
        case 0xFE:
            ExecAluOpA_Imm8(Alu::Cmp8, memory, result);
            return;
        // ret
        case 0xC9:
            Return(memory, result);
            return;
        // reti
        case 0xD9:
            ReturnAndEnableInterrupts(memory, result);
            return;
        // jp imm16
        case 0xC3:
            JumpImm16(memory, result);
            return;
        // jp hl
        case 0xE9:
            JumpHL(result);
            return;
        // call imm16
        case 0xCD:
            CallImm16(memory, result);
        // $CB prefix
        case 0xCB:
            _RunPrefixInstructions(memory, result);
            return;
        // ldh [c], a
        case 0xE2:
            LoadHighC_A(memory, result);
            return;
        // ldh [imm8], a
        case 0xE0:
            LoadHighImm8_A(memory, result);
            return;
        // ldh a, [c]
        case 0xF2:
            LoadA_HighC(memory, result);
            return;
        // ldh a, [imm8]
        case 0xF0:
            LoadA_HighImm8(memory, result);
            return;
        // ld [imm16], a
        case 0xEA:
            LoadAdrImm16_A(memory, result);
            return;
        // ld a, [imm16]
        case 0xFA:
            LoadA_AdrImm16(memory, result);
            return;
        // add sp, imm8
        case 0xE8:
            AddSP_Imm8(memory, result);
            return;
        // ld hl, sp + imm8
        case 0xF8:
            LoadHL_SPImm8(memory, result);
            return;
        // ld sp, hl
        case 0xF9:
            LoadSP_HL(result);
            return;
        // di
        case 0xF3:
            DisableInterrupts(result);
            return;
        // ei
        case 0xFB:
            EnableInterrupts(result);
            return;
        }

        // jump on cond
        uint8_t jumpOpcode = opcode & 0x7; // first 3 bits
        OperandCond cc = static_cast<OperandCond>((opcode >> 3) & 0x3); // bits 3-4
        uint8_t tgt3 = (opcode >> 3) & 0x7; // bits 5-3

        switch (jumpOpcode)
        {
        // ret cc
        case 0x0:
            ReturnCC(cc, memory, result);
            return;
        // jp cc, imm16
        case 0x2:
            JumpCC_Imm16(cc, memory, result);
            return;
        // call cc, imm16
        case 0x4:
            CallCC_Imm16(cc, memory, result);
            return;
        // rst tgt3
        case 0x7:
            RstVec(tgt3, memory, result);
            return;
        }

        // stack operation
        uint8_t stackOperation = opcode & 0xF;// first 4 bits 
        OperandR16Stk r16stk = static_cast<OperandR16Stk>((opcode >> 4) & 0x3);// bits 5-4

        switch (stackOperation)
        {
        case 0x1:
            PopR16Stk(r16stk, memory, result);
            return;
        case 0x5:
            PushR16Stk(r16stk, memory, result);
            return;
        }

        _ThrowInvalidInstructtion();
    }

    // $CB prefix instructions
    void Cpu::_RunPrefixInstructions(IMemory &memory, InstructionResult &result)
    {
        // retreive opcode 
        uint8_t opcode = GetImm8(memory, result);

        // type of the prefix instruction should is defined with the 7-6
        uint8_t instructionType = (opcode >> 6) & 0x3;
        uint8_t b3 = (opcode >> 3) & 0x7; // bits 5-3
        OperandR8 r8 = static_cast<OperandR8>(opcode & 0x7); // bits 2-0

        // bits operations
        switch (instructionType)
        {
        // bit b3, r8
        case 0x1:
            TestBitR8(b3, r8, memory, result);
            return;
        // res b3, r8
        case 0x2:
            ResetBitR8(b3, r8, memory, result);
            return;
        // set b3, r8
        case 0x3:
            SetBitR8(b3, r8, memory, result);
            return;
        }

        // rotate/shift operations
        uint8_t shiftOpcode = (opcode >> 3) & 0x7;
        switch (shiftOpcode)
        {
        // rlc r8
        case 0x0:
            RotateR8(Alu::RotateCarry, ShiftDirection::LEFT, r8, memory, result);
            return;
        // rrc r8
        case 0x1:
            RotateR8(Alu::RotateCarry, ShiftDirection::RIGHT, r8, memory, result);
            return;
        // rl r8
        case 0x2:
            RotateR8(Alu::Rotate, ShiftDirection::LEFT, r8, memory, result);
            return;
        // rr r8
        case 0x3:
            RotateR8(Alu::Rotate, ShiftDirection::RIGHT, r8, memory, result);
            return;
        // sla r8
        case 0x4:
            ShiftR8(ShiftDirection::LEFT, r8, memory, result);
            return;
        // sra r8
        case 0x5:
            ShiftR8(ShiftDirection::RIGHT, r8, memory, result);
            return;
        // swap r8
        case 0x6:
            SwapR8(r8, memory, result);
            return;
        // srl r8
        case 0x7:
            ShiftR8(ShiftDirection::RIGHT, r8, memory, result, true);
            return;
        }

        _ThrowInvalidInstructtion();
    }

    
    void Cpu::_ThrowInvalidInstructtion()
    {
        throw std::runtime_error("Instruction unknown!");
    }

} // namespace GBE
