#pragma once

#include <memory>
#include <cstdint>
#include <flat_map>

#include "Assembly.h"
#include "util/Class.h"

namespace GBE
{
    class InstructionDecoder;
    class Memory;
    class Instruction;

    class Disassembler
    {
    public:
        GBE_CLASS_NO_COPY_NO_MOVE(Disassembler)

        Disassembler(const std::shared_ptr<Memory>& memory, const std::shared_ptr<InstructionDecoder>& decoder);
        ~Disassembler() = default;
        
        void Disassemble(uint16_t startAddress);
        void DisassembleInstruction(uint16_t address, Assembly& assembly);
        
    private:
        using AssemblySection = std::flat_map<uint16_t, Assembly>;

        std::shared_ptr<Memory> m_Memory = nullptr;
        std::shared_ptr<InstructionDecoder> m_Decoder = nullptr;
        std::flat_map<uint16_t, AssemblySection> m_Sections;

        void _CreateAssembly(uint16_t address, const Instruction& instr, Assembly& assembly);
        void _AddOperands(uint16_t address, const Instruction& instr, Assembly& assembly);
        void _AddJumpOrCallNextAddress(uint16_t address, const Instruction &instr, Assembly &assembly);
        void _AddJumpRelativeNextAddress(uint16_t address, const Instruction &instr, Assembly &assembly);
        bool _IsNonConditionalJumpOrReturn(const Instruction &instr);
    };
} // namespace GBE
