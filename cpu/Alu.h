#pragma once

#include <cstdint>

namespace GBE
{
    struct AluResult;
    
    // alu shift/rotation direction
    enum class ShiftDirection
    {
        LEFT,
        RIGHT
    };

    // arithmetic and logical functions
    class Alu
    {
    public:
        /*
            8bit operartions with 2 operands
        */
        using OperationDestSrc8 = void (*)(uint8_t, uint8_t, AluResult& result, uint8_t );

        // add two 8bit numbers
        static void Add8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry = 0);

        // first arg - second arg
        static void Sub8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry = 0);

        // compare a and b (same as sub8 but without the result)
        static void Cmp8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry = 0);

        // bitewise and
        static void And8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry = 0);

        // bitewise or
        static void Or8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry = 0);

        // bitewise xor
        static void Xor8(uint8_t a, uint8_t b, AluResult &result, uint8_t carry = 0);

        /*
            8bit operations with one operand
        */
        using OperationDest8 = void (*) (uint8_t, AluResult& result);
        
        static void Increment8(uint8_t input, AluResult& result);

        static void Decrement8(uint8_t input, AluResult &result);

        /*
            8bit flag instructions
        */
       
        static void TestBit(uint8_t bit, uint8_t input, AluResult& result);
        
        static void SetBit(uint8_t bit, uint8_t input, AluResult& result);

        static void ResetBit(uint8_t bit, uint8_t input, AluResult& result);

        /*
            Shift/Rotation
        */

        using OperationRotateSrc = void (*) (uint8_t input, uint8_t, ShiftDirection, AluResult&, bool);

        static void Rotate(uint8_t input, uint8_t carry, ShiftDirection direction, AluResult& result, bool checkZero = true);

        static void RotateCarry(uint8_t input, uint8_t carry, ShiftDirection direction, AluResult &result, bool checkZero = true);

        static void Shift(uint8_t input, ShiftDirection direction, AluResult &result, bool isLogical = false);

        static void Swap(uint8_t input, AluResult &result);

        /*
            16bits operations with one operands
        */

        using OperationDest16 = void (*)(uint16_t, AluResult &result);

        static void Increment16(uint16_t input, AluResult& result);

        static void Decrement16(uint16_t input, AluResult& result);

        /*
            16 bits operations with two operands
        */

        // add two 16bit numbers
        static void Add16(uint16_t a, uint16_t b, AluResult& result);

        // offset 16bit number with signer 8 bit number
        static void Offset16(uint16_t a, int8_t e, AluResult& result);
    private:
        // get carry bit (last or first depending on the direction)
        static uint8_t _GetCarryBit(uint8_t input, ShiftDirection direction);
    };
} // namespace GBE
