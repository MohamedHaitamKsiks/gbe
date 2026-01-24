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
        if (instr.GetType() != InstructionType::NONE)
            return instr;

        instr.SetOpcode(opcode);
        _DecodePrefixInstruction(instr);

        // decode
        return instr;
    }

    void InstructionDecoder::_DecodeBlock0(Instruction &instr)
    {
        uint8_t opcode = instr.GetOpcode();

        // check defined opcodes
        switch (opcode)
        {
        // nop
        case 0x0:
            instr.SetType(InstructionType::NOP);
            instr.SetMethod(&Cpu::Nop);
            return;
        // ld [imm16], sp
        case 0x8:
            instr.SetType(InstructionType::LD);
            instr.AddOperand(OperandAddressOf(OperandImm16{}), OperandR16::SP);
            instr.SetMethod(&Cpu::LoadAdrImm16_SP);
            return;
        // rlca
        case 0x7:
            instr.SetType(InstructionType::RLCA);
            instr.SetMethod(&Cpu::RotateR8<Alu::RotateCarry, ShiftDirection::LEFT>);
            return;
        // rrca
        case 0xF:
            instr.SetType(InstructionType::RRCA);
            instr.SetMethod(&Cpu::RotateR8<Alu::RotateCarry, ShiftDirection::RIGHT>);
            return;
        // rla
        case 0x17:
            instr.SetType(InstructionType::RLA);
            instr.SetMethod(&Cpu::RotateR8<Alu::Rotate, ShiftDirection::LEFT>);
            return;
        // rra
        case 0x1F:
            instr.SetType(InstructionType::RRA);
            instr.SetMethod(&Cpu::RotateR8<Alu::Rotate, ShiftDirection::RIGHT>);
            return;
        // daa
        case 0x27:
            instr.SetType(InstructionType::DAA);
            instr.SetMethod(&Cpu::DecimalAdjustAccumulator);
            return;
        // cpl
        case 0x2F:
            instr.SetType(InstructionType::CPL);
            instr.SetMethod(&Cpu::ComplementA);
            return;
        // scf
        case 0x37:
            instr.SetType(InstructionType::SCF);
            instr.SetMethod(&Cpu::SetCarryFlag);
            return;
        // ccf
        case 0x3F:
            instr.SetType(InstructionType::CCF);
            instr.SetMethod(&Cpu::ComplementCarryFlag);
            return;
        // jr imm8
        case 0x18:
            instr.SetType(InstructionType::JR);
            instr.AddOperand(OperandImm8{});
            instr.SetMethod(&Cpu::JumpRelativeImm8);
            return;
        // TODO: stop
        case 0x10:
            instr.SetType(InstructionType::STOP);
            instr.SetMethod(&Cpu::Stop);
            return;
        }

        // check r16 operations (operand is on the 4-5 bits) and the opcode is the first 4 bits
        // first 4 bits
        uint8_t r16Opcode = opcode & 0xF;
        OperandR16 r16Operand = static_cast<OperandR16>((opcode >> 4) & 3);
        OperandR16Mem r16MemOperand = static_cast<OperandR16Mem>((opcode >> 4) & 3);

        switch (r16Opcode)
        {
        // ld r16, imm16
        case 0x1:
            instr.SetType(InstructionType::LD);
            instr.SetMethod(&Cpu::LoadR16_Imm16);
            instr.AddOperand(r16Operand, OperandImm16{});
            return;
        // ld [r16mem], a
        case 0x2:
            instr.SetType(InstructionType::LD);
            instr.SetMethod(&Cpu::LoadR16Mem_A);
            instr.AddOperand(OperandAddressOf(r16MemOperand), OperandR8::A);
            return;

        // ld a, [r16mem]
        case 0xA:
            instr.SetType(InstructionType::LD);
            instr.SetMethod(&Cpu::LoadA_R16Mem);
            instr.AddOperand(OperandR8::A, OperandAddressOf(r16MemOperand));
            return;

        // inc r16
        case 0x3:
            instr.SetType(InstructionType::INC);
            instr.SetMethod(&Cpu::ExecAluOpR16<&Alu::Increment16>);
            instr.AddOperand(r16Operand);
            return;
        // dec r16
        case 0xB:
            instr.SetType(InstructionType::DEC);
            instr.SetMethod(&Cpu::ExecAluOpR16<&Alu::Decrement16>);
            instr.AddOperand(r16Operand);
            return;
        // add hl, r16
        case 0x9:
            instr.SetType(InstructionType::ADD);
            instr.SetMethod(&Cpu::AddHL_R16);
            instr.AddOperand(OperandR16::HL, r16Operand);
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
            instr.SetType(InstructionType::INC);
            instr.AddOperand(r8Operand);
            instr.SetMethod(&Cpu::ExecAluOpR8<&Alu::Increment8>);
            return;
        // dec r8
        case 0x5:
            instr.SetType(InstructionType::DEC);
            instr.AddOperand(r8Operand);
            instr.SetMethod(&Cpu::ExecAluOpR8<&Alu::Decrement8>);
            return;
        // ld r8, imm8
        case 0x6:
            instr.SetType(InstructionType::LD);
            instr.AddOperand(r8Operand, OperandImm8{});
            instr.SetMethod(&Cpu::LoadR8_Imm8);
            return;
        }

        if ((r8Opcode == 0) && (opcode & 0x20))
        {
            // get bits 3-4
            OperandCond cc = static_cast<OperandCond>((opcode & 0x18) >> 3);
            instr.SetType(InstructionType::JR);
            instr.AddOperand(cc, OperandImm8{});
            instr.SetMethod(&Cpu::JumpRelativeCC_Imm8);
            return;
        }

        // if no instruction was found throw an error that the instruction is unrecognized
        _ThrowInvalidInstruction();
    }

    void InstructionDecoder::_DecodeBlock1(Instruction &instr)
    {
        uint8_t opcode = instr.GetOpcode();
        instr.SetSize(1);

        if (opcode == 0x76)
        {
            instr.SetType(InstructionType::HALT);
            instr.SetMethod(&Cpu::Halt);
            return;
        }

        instr.SetType(InstructionType::LD);
        instr.SetMethod(&Cpu::LoadR8_R8);

        // dest (next 3 bits)
        // src (last 3 bits)
        OperandR8 dest8 = static_cast<OperandR8>((opcode >> 3) & 0x7);
        OperandR8 src8 = static_cast<OperandR8>(opcode & 0x7);

        instr.AddOperand(dest8, src8);
    }

    void InstructionDecoder::_DecodeBlock2(Instruction &instr)
    {
        uint8_t opcode = instr.GetOpcode();
        
        // operand is first 3 bits
        // and opcode is retrieved from bits 5-3
        uint8_t r8Opcode = (opcode >> 3) & 0x7;
        OperandR8 r8 = static_cast<OperandR8>(opcode & 0x7);
        instr.AddOperand(OperandR8::A, r8);

        switch (r8Opcode)
        {
        // add a, r8
        case 0x0:
            instr.SetType(InstructionType::ADD);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::Add8, false>);
            return;
        // adc a, r8
        case 0x1:
            instr.SetType(InstructionType::ADC);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::Add8, true>);
            return;
        // sub a, r8
        case 0x2:
            instr.SetType(InstructionType::SUB);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::Sub8, false>);
            return;
        // sbc a, r8
        case 0x3:
            instr.SetType(InstructionType::SBC);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::Sub8, true>);
            return;
        // and a, r8
        case 0x4:
            instr.SetType(InstructionType::AND);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::And8, false>);
            return;
        // xor a, r8
        case 0x5:
            instr.SetType(InstructionType::XOR);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::Xor8, false>);
            return;
        // or  a, r8
        case 0x6:
            instr.SetType(InstructionType::OR);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::Or8, false>);
            return;
        // cp  a, r8
        case 0x7:
            instr.SetType(InstructionType::CP);
            instr.SetMethod(&Cpu::ExecAluOpA_R8<&Alu::Cmp8, false>);
            return;
        }

        _ThrowInvalidInstruction();
    }

    void InstructionDecoder::_DecodeBlock3(Instruction &instr)
    {
        uint8_t opcode = instr.GetOpcode();

        // predefined opcodes
        switch (opcode)
        {
        // add a, imm8
        case 0xC6:
            instr.SetType(InstructionType::ADD);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::Add8, false>);
            return;
        // adc a, imm8
        case 0xCE:
            instr.SetType(InstructionType::ADC);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::Add8, true>);
            return;
        // sub a, imm8
        case 0xD6:
            instr.SetType(InstructionType::SUB);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::Sub8, false>);
            return;
        // sbc a, imm8
        case 0xDE:
            instr.SetType(InstructionType::SBC);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::Sub8, true>);
            return;
        // and a, imm8
        case 0xE6:
            instr.SetType(InstructionType::AND);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::And8, false>);
            return;
        // xor a, imm8
        case 0xEE:
            instr.SetType(InstructionType::XOR);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::Xor8, false>);
            return;
        // or a, imm8
        case 0xF6:
            instr.SetType(InstructionType::OR);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::Or8, false>);
            return;
        // cp a, imm8
        case 0xFE:
            instr.SetType(InstructionType::CP);
            instr.AddOperand(OperandR8::A, OperandImm8{});
            instr.SetMethod(&Cpu::ExecAluOpA_Imm8<&Alu::Cmp8, false>);
            return;
        // ret
        case 0xC9:
            instr.SetType(InstructionType::RET);
            instr.SetMethod(&Cpu::Return);
            return;
        // reti
        case 0xD9:
            instr.SetType(InstructionType::RETI);
            instr.SetMethod(&Cpu::ReturnAndEnableInterrupts);
            return;
        // jp imm16
        case 0xC3:
            instr.SetType(InstructionType::JP);
            instr.AddOperand(OperandImm16{});
            instr.SetMethod(&Cpu::JumpImm16);
            return;
        // jp hl
        case 0xE9:
            instr.SetType(InstructionType::JP);
            instr.AddOperand(OperandR16::HL);
            instr.SetMethod(&Cpu::JumpHL);
            return;
        // call imm16
        case 0xCD:
            instr.SetType(InstructionType::CALL);
            instr.AddOperand(OperandImm16{});
            instr.SetMethod(&Cpu::CallImm16);
            return;
        // $CB prefix
        case 0xCB:
            instr.SetType(InstructionType::PREFIX_INST);
            return;
        // ldh [c], a
        case 0xE2:
            instr.SetType(InstructionType::LDH);
            instr.AddOperand(OperandAddressOf(OperandR8::C), OperandR8::A);
            instr.SetMethod(&Cpu::LoadHighC_A);
            return;
        // ldh [imm8], a
        case 0xE0:
            instr.SetType(InstructionType::LDH);
            instr.AddOperand(OperandAddressOf(OperandImm8{}), OperandR8::A);
            instr.SetMethod(&Cpu::LoadHighImm8_A);
            return;
        // ldh a, [c]
        case 0xF2:
            instr.SetType(InstructionType::LDH);
            instr.AddOperand(OperandR8::A, OperandAddressOf(OperandR8::C));
            instr.SetMethod(&Cpu::LoadA_HighC);
            return;
        // ldh a, [imm8]
        case 0xF0:
            instr.SetType(InstructionType::LDH);
            instr.AddOperand(OperandR8::A, OperandAddressOf(OperandImm8{}));
            instr.SetMethod(&Cpu::LoadA_HighImm8);
            return;
        // ld [imm16], a
        case 0xEA:
            instr.SetType(InstructionType::LD);
            instr.AddOperand(OperandAddressOf(OperandImm16{}), OperandR8::A);
            instr.SetMethod(&Cpu::LoadAdrImm16_A);
            return;
        // ld a, [imm16]
        case 0xFA:
            instr.SetType(InstructionType::LD);
            instr.AddOperand(OperandR8::A, OperandAddressOf(OperandImm16{}));
            instr.SetMethod(&Cpu::LoadA_AdrImm16);
            return;
        // add sp, imm8
        case 0xE8:
            instr.SetType(InstructionType::ADD);
            instr.AddOperand(OperandR16::SP, OperandImm8{});
            instr.SetMethod(&Cpu::AddSP_Imm8);
            return;
        // ld hl, sp + imm8
        case 0xF8:
            instr.SetType(InstructionType::LD);
            instr.AddOperand(OperandR16::HL, OperandR16::SP, OperandImm8{});
            instr.SetMethod(&Cpu::LoadHL_SPImm8);
            return;
        // ld sp, hl
        case 0xF9:
            instr.SetType(InstructionType::LD);
            instr.AddOperand(OperandR16::SP, OperandR16::HL);
            instr.SetMethod(&Cpu::LoadSP_HL);
            return;
        // di
        case 0xF3:
            instr.SetType(InstructionType::DI);
            instr.SetMethod(&Cpu::DisableInterrupts);
            return;
        // ei
        case 0xFB:
            instr.SetType(InstructionType::EI);
            instr.SetMethod(&Cpu::EnableInterrupts);
            return;
        }

        // jump on cond
        uint8_t jumpOpcode = opcode & 0x7;                              // first 3 bits
        OperandCond cc = static_cast<OperandCond>((opcode >> 3) & 0x3); // bits 3-4
        OperandTgt3 tgt3{ static_cast<uint8_t>((opcode >> 3) & 0x7) };                             // bits 5-3

        switch (jumpOpcode)
        {
        // ret cc
        case 0x0:
            instr.SetType(InstructionType::RET);
            instr.AddOperand(cc);
            instr.SetMethod(&Cpu::ReturnCC);
            return;
        // jp cc, imm16
        case 0x2:
            instr.SetType(InstructionType::JP);
            instr.AddOperand(cc, OperandImm16{});
            instr.SetMethod(&Cpu::JumpCC_Imm16);
            return;
        // call cc, imm16
        case 0x4:
            instr.SetType(InstructionType::CALL);
            instr.AddOperand(cc, OperandImm16{});
            instr.SetMethod(&Cpu::CallCC_Imm16);
            return;
        // rst tgt3
        case 0x7:
            instr.SetType(InstructionType::RST);
            instr.AddOperand(tgt3);
            instr.SetMethod(&Cpu::RstVec);
            return;
        }

        // stack operation
        uint8_t stackOperation = opcode & 0xF;                                  // first 4 bits
        OperandR16Stk r16stk = static_cast<OperandR16Stk>((opcode >> 4) & 0x3); // bits 5-4

        switch (stackOperation)
        {
        case 0x1:
            instr.SetType(InstructionType::POP);
            instr.AddOperand(r16stk);
            instr.SetMethod(&Cpu::PopR16Stk);
            return;
        case 0x5:
            instr.SetType(InstructionType::PUSH);
            instr.AddOperand(r16stk);
            instr.SetMethod(&Cpu::PushR16Stk);
            return;
        }

        _ThrowInvalidInstruction();
    }

    void InstructionDecoder::_DecodePrefixInstruction(Instruction &instr)
    {
        // retreive opcode
        uint8_t opcode = instr.GetOpcode();
        
        // type of the prefix instruction should is defined with the 7-6
        uint8_t instructionType = (opcode >> 6) & 0x3;

        OperandBit3 b3 { static_cast<uint8_t>((opcode >> 3) & 0x7) };                    // bits 5-3
        OperandR8   r8 = static_cast<OperandR8>(opcode & 0x7); // bits 2-0

        instr.AddOperand(b3, r8);

        // bits operations
        switch (instructionType)
        {
        // bit b3, r8
        case 0x1:
            instr.SetType(InstructionType::BIT);
            instr.SetMethod(&Cpu::TestBitR8);
            return;
        // res b3, r8
        case 0x2:
            instr.SetType(InstructionType::RES);
            instr.SetMethod(&Cpu::ResetBitR8);
            return;
        // set b3, r8
        case 0x3:
            instr.SetType(InstructionType::SET);
            instr.SetMethod(&Cpu::SetBitR8);
            return;
        }

        // rotate/shift operations
        uint8_t shiftOpcode = (opcode >> 3) & 0x7;
        instr.ClearOperands();
        instr.AddOperand(r8);

        switch (shiftOpcode)
        {
        // rlc r8
        case 0x0:
            instr.SetType(InstructionType::RLC);
            instr.SetMethod(&Cpu::RotateR8<Alu::RotateCarry, ShiftDirection::LEFT>);
            return;
        // rrc r8
        case 0x1:
            instr.SetType(InstructionType::RRC);
            instr.SetMethod(&Cpu::RotateR8<Alu::RotateCarry, ShiftDirection::RIGHT>);
            return;

        // rl r8
        case 0x2:
            instr.SetType(InstructionType::RL);
            instr.SetMethod(&Cpu::RotateR8<Alu::Rotate, ShiftDirection::LEFT>);
            return;
        // rr r8
        case 0x3:
            instr.SetType(InstructionType::RR);
            instr.SetMethod(&Cpu::RotateR8<Alu::Rotate, ShiftDirection::RIGHT>);
            return;

        // sla r8
        case 0x4:
            instr.SetType(InstructionType::SLA);
            instr.SetMethod(&Cpu::ShiftR8<ShiftDirection::LEFT, false>);
            return;
        // sra r8
        case 0x5:
            instr.SetType(InstructionType::SRA);
            instr.SetMethod(&Cpu::ShiftR8<ShiftDirection::RIGHT, false>);
            return;

        // swap r8
        case 0x6:
            instr.SetType(InstructionType::SWAP);
            instr.SetMethod(&Cpu::SwapR8);
            return;

        // srl r8
        case 0x7:
            instr.SetType(InstructionType::SRL);
            instr.SetMethod(&Cpu::ShiftR8<ShiftDirection::RIGHT, true>);
            return;
        }

        _ThrowInvalidInstruction();
    }

    void InstructionDecoder::_ThrowInvalidInstruction()
    {
        throw std::runtime_error("Instruction unknown!");
    }

} // namespace GBE

