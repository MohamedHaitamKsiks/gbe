#include "Operand.h"

namespace GBE
{
    template <>
    OperandType Operand::GetOperandType<OperandR8>()
    {
        return OperandType::R8;
    }

    template <>
    OperandType Operand::GetOperandType<OperandR16>()
    {
        return OperandType::R16;
    }

    template <>
    OperandType Operand::GetOperandType<OperandR16Mem>()
    {
        return OperandType::R16_MEM;
    }

    template <>
    OperandType Operand::GetOperandType<OperandR16Stk>()
    {
        return OperandType::R16_STK;
    }

    template <>
    OperandType Operand::GetOperandType<OperandBit3>()
    {
        return OperandType::BIT3;
    }

    template <>
    OperandType Operand::GetOperandType<OperandTgt3>()
    {
        return OperandType::TGT3;
    }

} // namespace GBE
