#pragma once

#include <cstdint>
#include "Assembly.h"

#include "Operand.h"
#include "InstructionType.h"

namespace GBE
{
    class Cpu;
    class Memory;
    class InstructionResult;


    // represents a decoded instruction
    class Instruction
    {   
    public:
        Instruction() = default;
        ~Instruction() = default;

        template <typename T, typename... Args>
        inline void AddOperand(T operand, Args... args)
        {
            m_Operands[m_OperandsCount++].Set(operand);

            if (Operand::GetOperandType<T>() == OperandType::IMM8)
                m_Size += 1;
            else if (Operand::GetOperandType<T>() == OperandType::IMM16)
                m_Size += 2;

            if constexpr (sizeof...(args) > 0)
                AddOperand<Args...>(args...);
        }

        
        template <typename T>
        inline T GetOperand(size_t indx) const
        {
            GBE_ASSERT(Operand::GetOperandType<T>() == m_Operands[indx].GetType());
            GBE_ASSERT(indx < m_OperandsCount);
            return m_Operands[indx].Get<T>();
        }

        // get operand at as tuple of types T...
        template <typename... T>
        inline std::tuple<T...> GetOperands() const
        {
            GBE_ASSERT(sizeof...(T) == m_OperandsCount);

            auto getOperandsImpl = [this]<std::size_t... I>(std::index_sequence<I...>)
            {
                return std::make_tuple(GetOperand<T>(I)...);
            };
            return getOperandsImpl(std::index_sequence_for<T...>{});
        }

        inline size_t GetOperandsCount() const 
        { 
            return m_OperandsCount; 
        }

        inline void ClearOperands()
        {
            m_OperandsCount = 0;
        }

        inline OperandType GetOperandType(size_t indx) const 
        {
            GBE_ASSERT(indx < m_OperandsCount);
            return m_Operands[indx].GetType(); 
        }

        inline bool IsOperandAddress(size_t indx) const
        {
            GBE_ASSERT(indx < m_OperandsCount);
            return m_Operands[indx].IsAddress();
        }

        inline Operand GetOperand(size_t indx) const
        {
            GBE_ASSERT(indx < m_OperandsCount);
            return m_Operands[indx];
        }

        inline void SetOpcode(uint8_t opcode)
        {
            m_Opcode = opcode;
        }

        inline uint8_t GetOpcode() const
        {
            return m_Opcode;
        }
        
        inline void SetType(InstructionType type)
        {
            m_Type = type;
        }
        
        inline InstructionType GetType() const
        {
            return m_Type;
        }

        inline void SetSize(uint16_t size)
        {
            m_Size = size;
        }

        inline uint16_t GetSize() const
        {
            return m_Size;
        }

        inline void SetMethod(void (Cpu::*method)(const Instruction &istr, Memory &memory, InstructionResult &result))
        {
            m_Method = method;
        }

        inline auto GetMethod() const
        {
            return m_Method;
        }


    private:
        std::array<Operand, 3> m_Operands{{}};
        size_t m_OperandsCount = 0;
        uint8_t m_Opcode = 0xFF;

        InstructionType m_Type = InstructionType::NONE;
        uint16_t m_Size = 1;

        void (Cpu::*m_Method)(const Instruction &istr, Memory &memory, InstructionResult &result) = nullptr;
    };
} // namespace GBE
