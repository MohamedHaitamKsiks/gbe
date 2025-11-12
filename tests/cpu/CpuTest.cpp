#include "GBETestSuite.h"

#include <iostream>
#include <memory>
#include <vector>

#include "memory/MemoryArea.h"
#include "memory/Memory.h"

#include "cpu/InstructionResult.h"
#include "cpu/Cpu.h"
#include "cpu/Alu.h"

#include "io/IORegister.h"

namespace GBETest
{
    class MemoryCpu : public GBE::MemoryArea
    {
    public:
        MemoryCpu()
        {
            m_MemoryData = std::make_unique<uint8_t[]>(UINT16_MAX + 1);
            SetReadWriteFlags(true);
        }

        ~MemoryCpu() {}

    private:
        uint8_t _GetImp(uint16_t address) const override
        {
            return m_MemoryData[address];
        }

        void _SetImp(uint16_t address, uint8_t value) override
        {
            m_MemoryData[address] = value;
        }

        std::unique_ptr<uint8_t[]> m_MemoryData = nullptr;
    };


} // namespace GBETest

GBE_TEST_SUITE(Cpu)
{
    // arrange
    auto memoryCpu = std::make_shared <GBETest::MemoryCpu>();
    GBE::Memory memory{};
    
    GBE::Cpu cpu{};

    TEST_CASE("Init")
    {
        memory.MapMemoryArea({GBE::MemoryMap{0, 0xFFFF}}, memoryCpu);
    }
    
    TEST_CASE("SetImm8")
    {
        // act 
        cpu.SetImm8(0xAB, memory);

        // assert
        CHECK_EQ(
            memory.Get(cpu.GetRegisters().GetReg16(GBE::Reg16::PC)),
            0xAB
        );
    }

    TEST_CASE("SetImm16")
    {
        // act
        cpu.SetImm16(0xABCD, memory);

        // assert
        CHECK_EQ(
            memory.Get(cpu.GetRegisters().GetReg16(GBE::Reg16::PC)),
            0xCD
        );

        CHECK_EQ(
            memory.Get(cpu.GetRegisters().GetReg16(GBE::Reg16::PC) + 1),
            0xAB
        );
    }

    TEST_CASE("GetImm16")
    {   
        GBE::InstructionResult result{};

        // arrange
        memory.Set(cpu.GetRegisters().GetReg16(GBE::Reg16::PC), 0xCD);
        memory.Set(cpu.GetRegisters().GetReg16(GBE::Reg16::PC) + 1, 0xAB);

        // act
        uint16_t imm16 = cpu.GetImm16(memory, result);

        // assert
        CHECK_EQ(
            imm16,
            0xABCD
        );

    }


    TEST_CASE("LoadR16_Imm16")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.SetImm16(0xABCD, memory);

        // act
        cpu.LoadR16_Imm16(GBE::OperandR16::HL, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::HL),
            0xABCD   
        );

        CHECK_EQ(
            result.Cycles,
            3
        );
    }

    TEST_CASE("LoadR8_Imm8")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.SetImm8(0xAB, memory);
        cpu.GetRegisters().SetReg16(GBE::Reg16::HL, 0x1234);

        // act
        cpu.LoadR8_Imm8(GBE::OperandR8::ADR_HL, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetReg16Adr(GBE::Reg16::HL, memory),
            0xAB  
        );

        CHECK_EQ(
            result.Cycles,
            3
        );
    }

    
    TEST_CASE("LoadReg16Adr_A")
    {
        //arrange
        GBE::InstructionResult result{};
        
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 16);
        cpu.SetImm16(0xABCD, memory);
        cpu.GetRegisters().SetReg16(GBE::Reg16::BC, 0xABCD);

        // act
        cpu.LoadR16Mem_A(GBE::OperandR16Mem::BC, memory, result);

        // assert
        CHECK_EQ(
            memory.Get(0xABCD),
            16
        );

        CHECK_EQ(
            result.Cycles,
            2
        );
    }

    TEST_CASE("LoadReg8_Reg16Adr")
    {
        // arrange
        GBE::InstructionResult result{};

        memory.Set(0xABCD, 16);
        cpu.GetRegisters().SetReg16(GBE::Reg16::BC, 0xABCD);
        
        // act
        cpu.LoadA_R16Mem(GBE::OperandR16Mem::BC, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            16   
        );

        CHECK_EQ(
            result.Cycles,
            2
        );
    }

    TEST_CASE("LoadAdrImm16_SP")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.SetImm16(0xABCD, memory);
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0x1234);

        // act
        cpu.LoadAdrImm16_SP(memory, result);

        // assert
        CHECK_EQ(
            memory.Get16(0xABCD),
            0x1234
        );

        CHECK_EQ(
            result.Cycles,
            5
        );
    }

    TEST_CASE("LoadR8_R8")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetReg8(GBE::Reg8::B, 16);

        // act
        cpu.LoadR8_R8(GBE::OperandR8::B, GBE::OperandR8::D, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::D),
            16
        );
    }

    TEST_CASE("ExecAluOpR16")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetReg16(GBE::Reg16::HL, 0xABCD);

        // act
        cpu.ExecAluOpR16(GBE::Alu::Increment16, GBE::OperandR16::HL, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::HL),
            0xABCE   
        );

        CHECK_EQ(
            result.Cycles,
            2
        );
    }


    TEST_CASE("AddHL_R16")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.GetRegisters().SetReg16(GBE::Reg16::HL, 800);
        cpu.GetRegisters().SetReg16(GBE::Reg16::BC, 1200);

        // act
        cpu.AddHL_R16(GBE::OperandR16::BC, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::HL),
            2000
        );
        
        CHECK_EQ(
            result.Cycles,
            2
        );
    }

    TEST_CASE("ExecAluOpR8")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.SetReg16Adr(GBE::Reg16::HL, 1, memory);

        // act
        cpu.ExecAluOpR8(GBE::Alu::Decrement8, GBE::OperandR8::ADR_HL, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetReg16Adr(GBE::Reg16::HL, memory),
            0
        );

        CHECK_EQ(
            result.Cycles,
            3
        );

        CHECK_NE(
            cpu.GetRegisters().GetFlags() & static_cast<uint8_t>(GBE::CpuFlag::Z),
            0
        );
    }

    TEST_CASE("ExecAluOpA_R8")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 8);
        cpu.GetRegisters().SetReg8(GBE::Reg8::B, 2);

        // act
        cpu.ExecAluOpA_R8(GBE::Alu::Add8, GBE::OperandR8::B, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            10
        );
    }

    TEST_CASE("ExecAluOpA_R8 With carry")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.GetRegisters().SetFlag(GBE::CpuFlag::C, true);
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 10);
        cpu.SetReg16Adr(GBE::Reg16::HL,5, memory);

        // act
        cpu.ExecAluOpA_R8(GBE::Alu::Add8, GBE::OperandR8::ADR_HL, memory, result, true);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            16
        );

        CHECK_EQ(
            result.Cycles,
            2
        );

    }

    TEST_CASE("LoadHighC_A")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.GetRegisters().SetReg8(GBE::Reg8::C, 0xCD);
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 16);

        // act
        cpu.LoadHighC_A(memory, result);

        // assert
        CHECK_EQ(
            memory.Get(0xFFCD),
            16
        );
    }

    TEST_CASE("LoadA_HighC")
    {
        // arrange
        GBE::InstructionResult result{};

        memory.Set(0xFFCD, 16);
        cpu.GetRegisters().SetReg8(GBE::Reg8::C, 0xCD);

        // act
        cpu.LoadA_HighC(memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            16
        );
    }


    TEST_CASE("LoadHighImm8_A")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.SetImm8(0xCD, memory);
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 16);

        // act
        cpu.LoadHighImm8_A(memory, result);

        // assert
        CHECK_EQ(
            memory.Get(0xFFCD),
            16
        );
    }

    TEST_CASE("LoadA_HighImm8")
    {
        // arrange
        GBE::InstructionResult result{};

        memory.Set(0xFFCD, 16);
        cpu.SetImm8(0xCD, memory);

        // act
        cpu.LoadA_HighImm8(memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            16
        );
    }

    TEST_CASE("LoadAdrImm16_A")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.SetImm16(0xABCD, memory);
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 16);

        // act
        cpu.LoadAdrImm16_A(memory, result);

        // assert
        CHECK_EQ(
            memory.Get(0xABCD),
            16
        );
    }

    TEST_CASE("LoadA_AdrImm16")
    {
        // arrange
        GBE::InstructionResult result{};
        memory.Set(0xABCD, 16);
        cpu.SetImm16(0xABCD, memory);

        // act
        cpu.LoadA_AdrImm16(memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            16
        );
    }

    TEST_CASE("AddSP_Imm8")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 1200);
        cpu.SetImm8(32, memory);

        // act
        cpu.AddSP_Imm8(memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::SP),
            1232
        );

        CHECK_EQ(
            result.Cycles,
            3
        );
    }

    TEST_CASE("Push")
    {
        // arrange
        GBE::InstructionResult result{};
        const uint16_t value = 0xABCD;
        const uint16_t initStack = 0xFFFF;
        const uint16_t newStack = initStack - 2;

        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, initStack);

        // act
        cpu.Push(value, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::SP),
            newStack
        );

        CHECK_EQ(
            memory.Get16(newStack),
            value
        );
    }

    TEST_CASE("Pop")
    {
        // arrange
        GBE::InstructionResult result{};
        const uint16_t value = 0xABCD;
        const uint16_t initStack = 0xFFFD;
        const uint16_t newStack = initStack + 2;

        memory.Set16(initStack, value);
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, initStack);

        // act
        uint16_t top = cpu.Pop(memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::SP),
            newStack
        );

        CHECK_EQ(
            top,
            value
        );
    }


    TEST_CASE("CallImm16")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xFFFF);
        cpu.SetImm16(0xABCD, memory);
        uint16_t oldPC = cpu.GetRegisters().GetReg16(GBE::Reg16::PC) + 2;
        
        // act
        cpu.CallImm16(memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::PC),
            0xABCD
        );

        CHECK_EQ(
            cpu.Top(memory),
            oldPC
        );

        CHECK_EQ(
            result.Cycles,
            6
        );
    }

    TEST_CASE("Rst16")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xFFFF);

        uint16_t oldPC = cpu.GetRegisters().GetReg16(GBE::Reg16::PC);

        // act
        cpu.RstVec(2, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::PC),
            16
        );

        CHECK_EQ(
            cpu.Top(memory),
            oldPC
        );

        CHECK_EQ(
            result.Cycles,
            4
        );
    }

    TEST_CASE("CallCC_Imm16 taken")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetFlag(GBE::CpuFlag::Z, true);
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xFFFF);
        
        cpu.SetImm16(0xABCD, memory);
        uint16_t oldPC = cpu.GetRegisters().GetReg16(GBE::Reg16::PC) + 2;
        
        // act
        cpu.CallCC_Imm16(GBE::OperandCond::Z, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::PC),
            0xABCD
        );

        CHECK_EQ(
            cpu.Top(memory),
            oldPC
        );

        CHECK_EQ(
            result.Cycles,
            6
        );
    }


    TEST_CASE("CallCC_Imm16 untaken")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetFlag(GBE::CpuFlag::Z, false);
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xFFFF);
        
        cpu.SetImm16(0xABCD, memory);
        uint16_t oldPC = cpu.GetRegisters().GetReg16(GBE::Reg16::PC) + 2;
        
        // act
        cpu.CallCC_Imm16(GBE::OperandCond::Z, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::PC),
            oldPC
        );

        CHECK_EQ(
            result.Cycles,
            3
        );
    }

    TEST_CASE("JumpRelativeCC_Imm16 taken")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetFlag(GBE::CpuFlag::Z, true);
        cpu.GetRegisters().SetReg16(GBE::Reg16::PC, 100);
        
        cpu.SetImm8(13, memory);

        // act
        cpu.JumpRelativeCC_Imm8(GBE::OperandCond::Z, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::PC),
            114
        );

        CHECK_EQ(
            result.Cycles,
            3
        );
    }

    TEST_CASE("JumpRelativeCC_Imm16 untaken")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetFlag(GBE::CpuFlag::Z, false);
        cpu.GetRegisters().SetReg16(GBE::Reg16::PC, 100);

        cpu.SetImm8(13, memory);

        // act
        cpu.JumpRelativeCC_Imm8(GBE::OperandCond::Z, memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::PC),
            101
        );

        CHECK_EQ(
            result.Cycles,
            2
        );
    }

    TEST_CASE("Return")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xFFFF);
        cpu.Push(0xABCD, memory, result);
        result.Cycles = 1;

        // act
        cpu.Return(memory, result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg16(GBE::Reg16::PC),
            0xABCD
        );

        CHECK_EQ(
            result.Cycles,
            4
        );
    }

    TEST_CASE("ComplementCarryFlag C is reset")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetFlag(GBE::CpuFlag::C, false);

        // act
        cpu.ComplementCarryFlag(result);

        // assert
        CHECK(cpu.GetRegisters().GetFlag(GBE::CpuFlag::C));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::N));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::H));
    }

    TEST_CASE("ComplementCarryFlag C is set")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetFlag(GBE::CpuFlag::C, true);

        // act
        cpu.ComplementCarryFlag(result);

        // assert
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::C));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::N));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::H));
    }

    TEST_CASE("SetCarryFlag C is set")
    {
        // arrange
        GBE::InstructionResult result{};
        cpu.GetRegisters().SetFlags(0xFF, 0);

        // act
        cpu.SetCarryFlag(result);

        // assert
        CHECK(cpu.GetRegisters().GetFlag(GBE::CpuFlag::C));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::N));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::H));
    }

    TEST_CASE("DecimalAdjustAccumulator")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 0x18);
        cpu.GetRegisters().SetReg8(GBE::Reg8::B, 0x24);
        cpu.ExecAluOpA_R8(GBE::Alu::Add8, GBE::OperandR8::B, memory, result);

        // act
        cpu.DecimalAdjustAccumulator(result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            0x42
        );

        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::Z));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::H));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::C));
    }

    TEST_CASE("DecimalAdjustAccumulator C flag")
    {
        // arrange
        GBE::InstructionResult result{};

        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 0x90);
        cpu.GetRegisters().SetReg8(GBE::Reg8::B, 0x12);
        cpu.ExecAluOpA_R8(GBE::Alu::Add8, GBE::OperandR8::B, memory, result);

        // act
        cpu.DecimalAdjustAccumulator(result);

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            0x02
        );

        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::Z));
        CHECK(!cpu.GetRegisters().GetFlag(GBE::CpuFlag::H));
        CHECK(cpu.GetRegisters().GetFlag(GBE::CpuFlag::C));
    }

    TEST_CASE("Run")
    {
        // arrange
        std::vector<uint8_t> program{};

        // Add 5 and 3, store result in register A
        // ld a, 5          ; Load 5 into register A
        program.push_back(0x3E);
        program.push_back(0x05);

        // add a, 3         ; A = A + 3 => A now holds 8
        program.push_back(0xC6);
        program.push_back(0x03);

        // Subtract 2 from A(8 - 2 = 6)
        // sub 2            ; A = A - 2 => A now holds 6
        program.push_back(0xD6);
        program.push_back(0x02);

        // ld b, a          ; Copy A into B (B = 6)
        program.push_back(0x47);
        // ld c, 4          ; C = multiplier
        program.push_back(0x0E);
        program.push_back(0x04);
        // ld a, 0          ; Clear A for accumulation
        program.push_back(0x3E);
        program.push_back(0x0);

        // MultiplyLoop:
        uint16_t multiplyLoopAdress = program.size();
        // add a, b         ; A += B
        program.push_back(0x80);
        // dec c
        program.push_back(0x0D);
        // jr nz, MultiplyLoop
        program.push_back(0x20);
        program.push_back(multiplyLoopAdress - program.size() - 1);
        // nop
        program.push_back(0x0);
        // At this point, A = 24

        // push program to memory
        memory.CopyBuffer(0, program.data(), program.size());

        cpu.GetRegisters().SetReg16(GBE::Reg16::PC, 0);
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xFFFF);
        
        // execute while pc is not at the end of the program
        while (cpu.GetRegisters().GetReg16(GBE::Reg16::PC) < program.size())
        {   
            GBE::InstructionResult result{};
            cpu.Run(memory, result);
        }

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            24
        );
    }


    TEST_CASE("Interrupts")
    {
        // arrange
        // v_bank handeler
        std::vector<uint8_t> vBlankHandler{};
        // ld a, 5          ; Load 5 into register A
        vBlankHandler.push_back(0x3E);
        vBlankHandler.push_back(0x05);
        // reti
        vBlankHandler.push_back(0xD9);

        // push handeler to memory
        memory.CopyBuffer(0x40, vBlankHandler.data(), vBlankHandler.size());

        // main program
        std::vector<uint8_t> program{};
        // ei
        program.push_back(0xFB);
        // nop
        program.push_back(0x0);
        program.push_back(0x0);
        program.push_back(0x0);

        // push program to memory
        memory.CopyBuffer(0x100, program.data(), program.size());

        // setup registers
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 0);
        cpu.GetRegisters().SetReg16(GBE::Reg16::PC, 0x100);
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xF000);

        // set interrupts flag
        memory.Set(static_cast<uint16_t>(GBE::IORegister::IF), 3);
        memory.Set(static_cast<uint16_t>(GBE::IORegister::IE), 1);

        // execute while pc is not at the end of the program
        while (cpu.GetRegisters().GetReg16(GBE::Reg16::PC) < static_cast<uint16_t>(0x100) + program.size())
        {   
            GBE::InstructionResult result{};
            cpu.Run(memory, result);

            // std::cout << result.Asm.ToString() << "\n";
        }

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            5
        );

        CHECK_EQ(
            memory.Get(static_cast<uint16_t>(GBE::IORegister::IF)),
            2
        );

        CHECK(
            cpu.GetIME()
        );
    }

    TEST_CASE("Two Interrupts")
    {
        // arrange
        // v_bank handeler
        std::vector<uint8_t> vBlankHandler{};
        // ld a, 5          ; Load 5 into register A
        vBlankHandler.push_back(0x3E);
        vBlankHandler.push_back(0x05);
        // reti
        vBlankHandler.push_back(0xD9);

        // push handeler to memory
        memory.CopyBuffer(0x40, vBlankHandler.data(), vBlankHandler.size());

        // lcd handeler
        std::vector<uint8_t> lcdHandler{};
        // ld b, a           ; Load A into register B
        lcdHandler.push_back(0x47);
        // reti
        lcdHandler.push_back(0xD9);

        // push handeler to memory
        memory.CopyBuffer(0x48, lcdHandler.data(), lcdHandler.size());

        // main program
        std::vector<uint8_t> program{};
        // nop
        program.push_back(0x0);
        program.push_back(0x0);
        program.push_back(0x0);

        // push program to memory
        memory.CopyBuffer(0x100, program.data(), program.size());

        // setup registers
        cpu.GetRegisters().SetReg8(GBE::Reg8::A, 0);
        cpu.GetRegisters().SetReg8(GBE::Reg8::B, 0);
        cpu.GetRegisters().SetReg16(GBE::Reg16::PC, 0x100);
        cpu.GetRegisters().SetReg16(GBE::Reg16::SP, 0xF000);

        // set interrupts flag
        memory.Set(static_cast<uint16_t>(GBE::IORegister::IF), 3);
        memory.Set(static_cast<uint16_t>(GBE::IORegister::IE), 3);

        // execute while pc is not at the end of the program
        while (cpu.GetRegisters().GetReg16(GBE::Reg16::PC) < static_cast<uint16_t>(0x100) + program.size())
        {
            GBE::InstructionResult result{};
            cpu.Run(memory, result);

            //std::cout << result.Asm.ToString() << "\n";
        }

        // assert
        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::A),
            5
        );

        CHECK_EQ(
            cpu.GetRegisters().GetReg8(GBE::Reg8::B),
            5
        );

        CHECK_EQ(
            memory.Get(static_cast<uint16_t>(GBE::IORegister::IF)),
            0
        );

        CHECK(
            cpu.GetIME()
        );
    }
}
