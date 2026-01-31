#include "Alu.h"
#include "AluResult.h"

#include "util/Binary.h"

#include "cpu/registers/CpuFlags.h"

#include <iostream>

namespace GBE
{
    void Alu::Increment16(uint16_t input, AluResult &result)
    {
        result.AffectedFlags = 0;
        result.Result16 = input + 1;
    }

    void Alu::Decrement16(uint16_t input, AluResult &result)
    {
        result.AffectedFlags = 0;
        result.Result16 = input - 1;
    }

    void Alu::Increment8(uint8_t input, AluResult &result)
    {
        // calculate result
        result.Result8 = input + 1;

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H;
        result.Flags = 0;

        // is zero
        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;

        // overflow 3
        if ((input & 0xf) == 0xf)
            result.Flags |= CpuFlag::H;
    }

    void Alu::Decrement8(uint8_t input, AluResult &result)
    {
        // calculate result
        result.Result8 = input - 1;

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H;
        result.Flags = CpuFlag::N;

        // is zero
        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;

        // overflow 3
        if (!(input & 0xf))
            result.Flags |= CpuFlag::H;
    }


    void Alu::TestBit(uint8_t bit, uint8_t input, AluResult &result)
    {
        result.Result8 = input;

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H;
        result.Flags = CpuFlag::H;

        if (!Binary::TestBit(input, bit))
            result.Flags |= CpuFlag::Z;
    }

    void Alu::SetBit(uint8_t bit, uint8_t input, AluResult &result)
    {
        result.Result8 = Binary::SetBit(input, bit);
        result.AffectedFlags = 0;
    }

    void Alu::ResetBit(uint8_t bit, uint8_t input, AluResult &result)
    {
        result.Result8 = Binary::ResetBit(input, bit);
        result.AffectedFlags = 0;
    }

    void Alu::Rotate(uint8_t input, uint8_t carry, ShiftDirection direction, AluResult &result, bool checkZero)
    {
        // compute result
        if (direction == ShiftDirection::LEFT)
            result.Result8 = (input << 1) | carry;
        else 
            result.Result8 = (input >> 1) | (carry << 7);

        // check flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        if (checkZero && result.Result8 == 0)
            result.Flags |= CpuFlag::Z;

        if (_GetCarryBit(input, direction))
            result.Flags |= CpuFlag::C;
    }

    void Alu::RotateCarry(uint8_t input, uint8_t carry, ShiftDirection direction, AluResult &result, bool checkZero)
    {
        // compute result
        uint8_t carryBit = _GetCarryBit(input, direction);

        if (direction == ShiftDirection::LEFT)
            result.Result8 = (input << 1) | carryBit;
        else
            result.Result8 = (input >> 1) | (carryBit << 7);

        // check flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        if (checkZero && result.Result8 == 0)
            result.Flags |= CpuFlag::Z;

        if (carryBit)
            result.Flags |= CpuFlag::C;
    }

    void Alu::Shift(uint8_t input, ShiftDirection direction, AluResult &result, bool isLogical)
    {
        // compute result
        uint8_t carryBit = _GetCarryBit(input, direction);
        
        if (direction == ShiftDirection::LEFT)
        {
            result.Result8 = input << 1;
        }
        else if (isLogical)
        {
            result.Result8 = input >> 1;
        }
        else
        {
            uint8_t lastBitMask = 1 << 7;
            uint8_t lastBit = input & lastBitMask;
            result.Result8 = ((input >> 1) & (~lastBitMask)) | lastBit;
        }

        // check flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;

        if (carryBit)
            result.Flags |= CpuFlag::C;
    }

    void Alu::Swap(uint8_t input, AluResult &result)
    {
        uint8_t upperBits = (input & 0xF0) >> 4;
        result.Result8 = upperBits | (input << 4);
        
        // flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        if (result.Result8 == 0)
            result.Flags = CpuFlag::Z;
    }

    void Alu::Add8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry )
    {
        // calculate result
        uint32_t result32 = static_cast<uint32_t>(a) + static_cast<uint32_t>(b) + static_cast<uint32_t>(carry);
        result.Result8 = static_cast<uint8_t>(result32);

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        // is zero
        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;

        // overflow 3
        if (((a & 0xf) + (b & 0xf) + carry) > 0xf)
            result.Flags |= CpuFlag::H;

        // overflow 7
        if (result32 > 0xff)
            result.Flags |= CpuFlag::C;
    }

    void Alu::Add16(uint16_t a, uint16_t b, AluResult &result)
    {
        // calculate result
        uint32_t result32 = a + b;
        result.Result16 = static_cast<uint16_t>(result32);

        // set flags
        result.AffectedFlags = CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        // overflow 11
        if (((a & 0xfff) + (b & 0xfff)) > 0xfff)
            result.Flags |= CpuFlag::H;

        // overflow 15  
        if (result32 > 0xffff)
            result.Flags |= CpuFlag::C;
    }

    void Alu::Offset16(uint16_t a, int8_t e, AluResult &result)
    {
        int16_t e16 = static_cast<int16_t>(e); // cast it to 16 bits signed

        result.Result16 = static_cast<uint16_t>(a + e16);

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        // overflow 3
        if (((a & 0xf) + (e16 & 0xf)) > 0xf)
            result.Flags |= CpuFlag::H;

        // overflow 7
        if (((a & 0xff) + (e16 & 0xff)) > 0xff)
            result.Flags |= CpuFlag::C;
    }

    uint8_t Alu::_GetCarryBit(uint8_t input, ShiftDirection direction)
    {
        if (direction == ShiftDirection::LEFT)
            return input >> 7;
        
        return input & 1;
    }

    void Alu::Sub8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry)
    {
        // convert operands to 32
        uint32_t a32 = static_cast<uint32_t>(a);
        uint32_t b32 = static_cast<uint32_t>(b) + static_cast<uint32_t>(carry);

        // compute
        b += carry;
        result.Result8 = (a - b);

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = CpuFlag::N;

        // is zero
        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;

        // overflow 3
        if ((a & 0xf) < (b & 0xf))
            result.Flags |= CpuFlag::H;

        // overflow 7
        if (a32 < b32)
            result.Flags |= CpuFlag::C;
    }

    void Alu::Cmp8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry)
    {
        Sub8(a, b, result);
        result.Result8 = a;
    }

    void Alu::And8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry)
    {
        result.Result8 = a & b;

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = CpuFlag::H;

        // is zero
        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;
    }

    void Alu::Or8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry)
    {
        result.Result8 = a | b;

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        // is zero
        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;
    }

    void Alu::Xor8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry)
    {
        result.Result8 = a ^ b;

        // set flags
        result.AffectedFlags = CpuFlag::Z | CpuFlag::N | CpuFlag::H | CpuFlag::C;
        result.Flags = 0;

        // is zero
        if (result.Result8 == 0)
            result.Flags |= CpuFlag::Z;
    }
} // namespace GBE
