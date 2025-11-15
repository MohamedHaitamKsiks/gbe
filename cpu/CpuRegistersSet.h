#pragma once

#include <string>

#include "CpuFlags.h"
#include "CpuRegister.h"

namespace GBE
{
    // R8
    enum class Reg8
    {
        B = 0,
        C = 1,
        D = 2,
        E = 3,
        H = 4,
        L = 5,
        A = 7
    };

    // R16
    enum class Reg16
    {
        BC = 0,
        DE = 1,
        HL = 2,
        SP = 3,
        AF,
        PC
    };

    // cpu registers
    class CpuRegistersSet
    {
    public:
        CpuRegistersSet();
        ~CpuRegistersSet() {}

        // get Reg8
        uint8_t GetReg8(Reg8 r8) const;

        // get Reg16
        uint16_t GetReg16(Reg16 r16) const;

        // set Reg8
        void SetReg8(Reg8 r8, uint8_t value);

        // set Reg16
        void SetReg16(Reg16 r16, uint16_t value);

        // get flags
        uint8_t GetFlags() const;

        // get single flag
        bool GetFlag(CpuFlag flag) const;

        // set flag
        void SetFlag(CpuFlag flag, bool value);

        // set multiple flags
        void SetFlags(uint8_t flags, uint8_t values);

        // to string
        std::string ToString() const;

    private:
        CpuRegister m_AF{};
        CpuRegister m_BC{};
        CpuRegister m_DE{};
        CpuRegister m_HL{};

        uint16_t m_SP = 0;
        uint16_t m_PC = 0;

        std::string _FlagsToString() const;
    };

} // namespace GBE
