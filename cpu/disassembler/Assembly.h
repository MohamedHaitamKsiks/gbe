#pragma once

#include <vector>
#include <string>
#include <string_view>

#include <magic_enum.hpp>

#include "util/Binary.h"
#include "util/Foreach.h"

#include "cpu/instruction/Operand.h"
#include "cpu/instruction/InstructionType.h"

namespace GBE
{
    class Assembly
    {
    public:
        struct NextAddress
        {
            uint16_t    Address = 0x0;
            bool        IsJump = false;
        };

        Assembly() = default;
        ~Assembly() = default;

        inline void SetAddress(uint16_t address)
        {
            m_Address = address;
        }

        inline uint16_t GetAddress() const
        {
            return m_Address;
        }

        inline void AddNextAddress(const NextAddress& address)
        {
            m_NextAddresses.push_back(address);
        }

        inline const std::vector<NextAddress>& GetNextAddresses() const
        {
            return m_NextAddresses;
        }

        // get non jump next instruiction address
        uint16_t GetNextInstructionAddress() const;
        
        void SetOperation(InstructionType op);
        void AddOperand(Operand op);
        void AddImm8(uint8_t imm8, bool isAddress = false);
        void AddImm16(uint16_t imm16, bool isAddress = false);

        const std::string& ToString() const;
    private:
        mutable std::string m_CachedString = "";

        std::string m_Operation = "";
        std::vector<std::string> m_Operands{};

        uint16_t m_Address = 0x0;
        std::vector<NextAddress> m_NextAddresses{};

        void _AddOperand(std::string_view operand);
        void _AddR8(OperandR8 r8);
        void _AddR16(OperandR16 r16);
        void _AddR16Mem(OperandR16Mem r16mem);
        void _AddR16Stk(OperandR16Stk r16stk);
        void _AddCond(OperandCond cc);
        void _AddBit3(OperandBit3 bit3);
        void _AddTgt3(OperandTgt3 tgt3);
    };
} // namespace GBE
