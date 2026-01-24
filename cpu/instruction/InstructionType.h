#pragma once

namespace GBE
{
    enum class InstructionType
    {
        NONE = 0,
        // load
        LD,
        LDH,

        // arithmetics
        DEC,
        INC,
        ADD,
        SUB,
        SBC,
        CP,

        // bitwise instructions
        AND,
        OR,
        XOR,
        CPL,

        // bit flag
        BIT,
        RES,
        SET,

        // bit shift
        RL,
        RLA,
        RLC,
        RLCA,
        RR,
        RRA,
        RRC,
        RRCA,
        SLA,
        SRA,
        SRL,
        SWAP,

        // jump & subroutines
        RET,
        RETI,
        CALL,
        JP,
        JR,
        RST,

        // carry flag
        CCF,
        SCF,

        // stack manipulation
        POP,
        PUSH,

        // interrupt
        DI,
        EI,
        HALT,

        // miscellaneous
        DAA,
        NOP,
        STOP,

        // signal to fetch next imm8 for instruction
        PREFIX_INST
    };
} // namespace GBE
