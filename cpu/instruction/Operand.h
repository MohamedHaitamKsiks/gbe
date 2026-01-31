#pragma once

#include <cstdint>
#include <string>
#include <variant>

#include "util/Assert.h"

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

    // A 3-bit bit index
    struct OperandBit3 { uint8_t Value = 0; };

    // rst's target address, divided by 8        TGT3,
    struct OperandTgt3 { uint8_t Value = 0; };

    struct OperandImm8 {};
    
    struct OperandImm16 {};

    template<typename T>
    struct OperandAddress
    {
        T Op;
    };

    template <typename T>
    static OperandAddress<T> OperandAddressOf(T t)
    {
        OperandAddress<T> opAdr{};
        opAdr.Op = t;
        return opAdr;
    }

    //
    enum class OperandType
    {
        NONE = 0,
        R8,
        R16,
        R16_STK,
        R16_MEM,
        COND,
        BIT3,
        TGT3,
        IMM8,
        IMM16
    };

    class Operand
    {
    private:
        using Variant = std::variant<
            OperandR8,
            OperandR16,
            OperandR16Mem,
            OperandR16Stk,
            OperandCond,
            OperandBit3,
            OperandTgt3,
            OperandImm8,
            OperandImm16>;

    public:
        template <typename T>
        constexpr static OperandType GetOperandType()
        {
            if constexpr(std::is_same_v<T, OperandR16>)
                return OperandType::R16;

            if constexpr (std::is_same_v<T, OperandR16Mem>)
                return OperandType::R16_MEM;

            if constexpr (std::is_same_v < T, OperandR16Stk>)
                return OperandType::R16_STK;

            if constexpr (std::is_same_v < T, OperandR8>)
                return OperandType::R8;

            if constexpr (std::is_same_v<T, OperandBit3>)
                return OperandType::BIT3;

            if constexpr (std::is_same_v<T, OperandTgt3>)
                return OperandType::TGT3;

            if constexpr (std::is_same_v < T, OperandImm8>)
                return OperandType::IMM8;

            if constexpr (std::is_same_v<T, OperandImm16>)
                return OperandType::IMM16;

            return OperandType::NONE;
        };

        template <typename T>
        inline void Set(T op)
        {
            m_Operand = op;
            m_Type = GetOperandType<T>();
        }

        template <typename T>
        inline void Set(OperandAddress<T> op)
        {
            Set<T>(op.Op);
            m_IsAddress = true;
        }

        template <typename T>
        inline const T& Get() const
        {
            GBE_ASSERT(GetOperandType<T>() == m_Type);
            return std::get<T>(m_Operand);
        }

        inline OperandType GetType() const
        {
            return m_Type;
        }

        inline bool IsAddress() const 
        {
            return m_IsAddress;
        }

    private:
        Variant m_Operand;

        OperandType m_Type = OperandType::NONE;
        bool m_IsAddress = false;
    };

} // namespace GBE
