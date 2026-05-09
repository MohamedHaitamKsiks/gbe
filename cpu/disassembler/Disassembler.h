#pragma once

#include <memory>
#include <cstdint>
#include <flat_map>
#include <flat_set>

#include "Assembly.h"
#include "util/Class.h"

namespace GBE
{
    class InstructionDecoder;
    class Memory;
    class Instruction;

    struct AssemblySection
    {
        uint16_t StartAddress   = 0x0;
        uint16_t EndAddress     = 0x0;
    };

    class Disassembler
    {
    public:
        GBE_CLASS_NO_COPY_NO_MOVE(Disassembler)

        Disassembler(const std::shared_ptr<Memory>& memory, const std::shared_ptr<InstructionDecoder>& decoder);
        ~Disassembler() = default;
        
        void Disassemble(uint16_t startAddress, const AssemblySection& secion);
        void DisassembleInstruction(uint16_t address, Assembly& assembly);
        
        inline const std::flat_map<uint16_t, AssemblySection>& GetAssemblySections() const
        {
            return m_AssemblySections;
        }

        inline const Assembly& GetAssemblyInstruction(uint16_t address) const
        {
            return m_AssemblyInstructions.at(address);
        }

        
    private:
        std::shared_ptr<Memory> m_Memory = nullptr;
        std::shared_ptr<InstructionDecoder> m_Decoder = nullptr;

        std::array<Assembly, UINT16_MAX + 1 > m_AssemblyInstructions{};
        std::array<bool, UINT16_MAX + 1> m_IsDisassembled = { false };

        std::flat_map<uint16_t, AssemblySection> m_AssemblySections;
        std::flat_set<uint16_t> m_JumpAddresses{};

        AssemblySection m_MaxSection{};

        void _Disassemble(uint16_t address);

        void _ComputeAssemblySections();

        void _CreateAssembly(uint16_t address, const Instruction &instr, Assembly &assembly, bool isPrefix);
        void _AddOperands(uint16_t address, const Instruction& instr, Assembly& assembly);
        void _AddJumpOrCallNextAddress(uint16_t address, const Instruction &instr, Assembly &assembly);
        void _AddJumpRelativeNextAddress(uint16_t address, const Instruction &instr, Assembly &assembly);
        bool _IsNonConditionalJumpOrReturn(const Instruction &instr);
    };
} // namespace GBE
