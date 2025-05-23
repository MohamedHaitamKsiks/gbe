

namespace GBE
{
    enum class OperandR8
    {
        B = 0,
        C = 1,
        D = 2,
        E = 3,
        H = 4,
        L = 5,
        ADR_HL = 6,
        A = 7,   
    };

    enum class OperandR16
    {
        BC = 0,
        DE = 1,
        HL = 2,
        SP = 3
    };

    enum class OperandR16Stk
    {
        BC = 0,
        DE = 1,
        HL = 2,
        AF = 3
    };

    enum class OperandR16Mem
    {
        BC = 0,
        DE = 1,
        HLI = 2,
        HLD = 3
    };

    enum class OperandCond
    {
        NZ = 0,
        Z = 1,
        NC = 2,
        C = 3
    };
} // namespace GBE
