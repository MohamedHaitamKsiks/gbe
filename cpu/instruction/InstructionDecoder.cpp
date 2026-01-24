#include "InstructionDecoder.h"
#include "cpu/Cpu.h"
#include "cpu/alu/Alu.h"


namespace GBE
{
    const Instruction &InstructionDecoder::Decode(uint8_t opcode) const
    {
        Instruction& instr = m_Instructions.at(opcode);
        if (instr.GetType() != InstructionType::NONE)
            return instr;

        
        // read block instruction (inspired by: https://gbdev.io/pandocs/CPU_Instruction_Set.html)
        // block id is the last 2 bits (6 and 7)
        instr.SetOpcode(opcode);
        uint8_t blockId = opcode >> 6;

        switch (blockId)
        {
        // Block 0
        case 0:
            _DecodeBlock0(instr);
            break;
        // Block 1 8-bit register-to-register loads
        case 1:
            _DecodeBlock1(instr);
            break;
        // Block 2: 8-bit arithmetic
        case 2:
            _DecodeBlock2(instr);
            break;
        // Block 3
        case 3:
            _DecodeBlock3(instr);
            break;
        }

        // decode
        return instr;
    }

    const Instruction &InstructionDecoder::DecodePrefix(uint8_t opcode) const
    {
        Instruction &instr = m_PrefixInstructions.at(opcode);
        if (instr.Type != InstructionType::NONE)
            return instr;

        instr.Opcode = opcode;
        _DecodePrefixInstruction(instr);

        // decode
        return instr;
    }

    void InstructionDecoder::_DecodeBlock0(Instruction &instr)
    {
        uint8_t opcode = instr.Opcode;

        // check defined opcodes
        switch (opcode)
        {
        // nop
        case 0x0:
            instr.Type = InstructionType::NOP;
            instr.SetMethod(&Cpu::Nop);
            return;
        // ld [imm16], sp
        case 0x8:
            instr.Type = InstructionType::LD;
            instr.SetMethod(&Cpu::LoadAdrImm16_SP);
            return;
        // rlca
        case 0x7:
            instr.Type = InstructionType::RLCA;
            return;
        // rrca
        case 0xF:
            instr.Type = InstructionType::RRCA;
            return;
        // rla
        case 0x17:
            instr.Type = InstructionType::RLA;
            return;
        // rra
        case 0x1F:
            instr.Type = InstructionType::RRA;  
            return;
        // daa
        case 0x27:
            instr.Type = InstructionType::DAA;
            instr.Method = &Cpu::DecimalAdjustAccumulator;
            return;
        // cpl
        case 0x2F:
            instr.Type = InstructionType::CPL;
            instr.Method = &Cpu::ComplementA;
            return;
        // scf
        case 0x37:
            instr.Type = InstructionType::SCF;
            instr.Method = &Cpu::SetCarryFlag;
            return;
        // ccf
        case 0x3F:
            instr.Type = InstructionType::CCF;
            instr.Method = &Cpu::ComplementCarryFlag;
            return;
        // jr imm8
        case 0x18:
            instr.Type = InstructionType::JR;
            instr.Method = &Cpu::JumpRelativeImm8;
            return;
        // TODO: stop
        case 0x57:
            instr.Type = InstructionType::STOP;

            return;
        }

        // check r16 operations (operand is on the 4-5 bits) and the opcode is the first 4 bits
        // first 4 bits
        uint8_t r16Opcode = opcode & 0xF;
        OperandR16 r16Operand = static_cast<OperandR16>((opcode >> 4) & 3);

        switch (r16Opcode)
        {
        // ld r16, imm16
        case 0x1:
            instr.Type = InstructionType::LD;
            instr.Method = &Cpu::LoadR16_Imm16;
            instr.AddOperand(r16Operand);
            instr.AddOperand(OperandImm16{});
            return;
        // ld [r16mem], a
        case 0x2:
            instr.Type = InstructionType::LD;
            instr.Method = &Cpu::LoadR16Mem_A;
            instr.AddOperand(r16Operand);
            instr.AddOperand(OperandR8::A);
            return;

        // ld a, [r16mem]
        case 0xA:
            instr.Type = InstructionType::LD;
            instr.Method = &Cpu::LoadA_R16Mem;
            instr.AddOperand(OperandR8::A);
            instr.AddOperand(r16Operand);
            return;

        // inc r16
        case 0x3:
            instr.Type = InstructionType::INC;
            instr.Method = &Cpu::ExecAluOpR16<&Alu::Increment16>;
            instr.AddOperand(r16Operand);
            return;
        // dec r16
        case 0xB:
            instr.Type = InstructionType::DEC;
            instr.Method = &Cpu::ExecAluOpR16<&Alu::Decrement16>;
            instr.AddOperand(r16Operand);
            return;
        // add hl, r16
        case 0x9:
            instr.Type = InstructionType::ADD;
            instr.Method = &Cpu::AddHL_R16;
            instr.AddOperand(OperandR16::HL);
            instr.AddOperand(r16Operand);
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
            instr.Type = InstructionType::INC;
            instr.AddOperand(r8Operand);
            return;
        // dec r8
        case 0x5:
            instr.Type = InstructionType::DEC;
            instr.AddOperand(r8Operand);
            return;
        // ld r8, imm8
        case 0x6:
            instr.Type = InstructionType::LD;
            instr.AddOperand(r8Operand);
            instr.AddOperand(OperandImm8{});
            return;
        }

        // check for jr cond, imm8
        if ((r8Opcode == 0) && (opcode & 0x20))
        {
            // get bits 3-4
            OperandCond cc = static_cast<OperandCond>((opcode & 0x18) >> 3);
            instr.Type = InstructionType::JR;
            instr.AddOperand(cc);
            instr.AddOperand(OperandImm8{});

            return;
        }

        // if no instruction was found throw an error that the instruction is unrecognized
        _ThrowInvalidInstruction();
    }

    void InstructionDecoder::_DecodeBlock1(Instruction &instr)
    {
        uint8_t opcode = instr.Opcode;
        instr.Size = 1;

        if (opcode == 0x76)
        {
            instr.Type = InstructionType::HALT;
            return;
        }
        instr.Type = InstructionType::LD;
        instr.Method = &Cpu::LoadR8_R8;

        // dest (next 3 bits)
        OperandR8 dest8 = static_cast<OperandR8>((opcode >> 3) & 0x7);
        instr.AddOperand(dest8);

        // src (first 3 bits)
        OperandR8 src8 = static_cast<OperandR8>(opcode & 0x7);
        instr.AddOperand(src8);
    }

    void InstructionDecoder::_DecodeBlock2(Instruction &instr)
    {
    }

    void InstructionDecoder::_DecodeBlock3(Instruction &instr)
    {
    }

    void InstructionDecoder::_DecodePrefixInstruction(Instruction &instr)
    {
        // retreive opcode
        uint8_t opcode = instr.Opcode;

        // type of the prefix instruction should is defined with the 7-6
        uint8_t instructionType = (opcode >> 6) & 0x3;

        OperandBit3 b3 { static_cast<uint8_t>((opcode >> 3) & 0x7) };                    // bits 5-3
        OperandR8   r8 = static_cast<OperandR8>(opcode & 0x7); // bits 2-0

        instr.AddOperand(b3);
        instr.AddOperand(r8);

        // bits operations
        switch (instructionType)
        {
        // bit b3, r8
        case 0x1:
            instr.Type = InstructionType::BIT;
            return;
        // res b3, r8
        case 0x2:
            instr.Type = InstructionType::RES;
            return;
        // set b3, r8
        case 0x3:
            instr.Type = InstructionType::SET;
            return;
        }

        // rotate/shift operations
        uint8_t shiftOpcode = (opcode >> 3) & 0x7;

        instr.OperandsCount = 0;
        instr.AddOperand(r8);

        switch (shiftOpcode)
        {
        // rlc r8
        case 0x0:
            instr.Type = InstructionType::RLC;
            return;
        // rrc r8
        case 0x1:
            instr.Type = InstructionType::RRC;
            return;

        // rl r8
        case 0x2:
            instr.Type = InstructionType::RL;
            return;
        // rr r8
        case 0x3:
            instr.Type = InstructionType::RR;
            return;

        // sla r8
        case 0x4:
            instr.Type = InstructionType::SLA;
            return;
        // sra r8
        case 0x5:
            instr.Type = InstructionType::SRA;
            return;

        // swap r8
        case 0x6:
            instr.Type = InstructionType::SWAP;
            return;

        // srl r8
        case 0x7:
            instr.Type = InstructionType::SRL;
            return;
        }

        _ThrowInvalidInstruction();
    }

    void InstructionDecoder::_ThrowInvalidInstruction()
    {
        throw std::runtime_error("Instruction unknown!");
    }

} // namespace GBE

