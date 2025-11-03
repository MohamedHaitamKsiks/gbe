#pragma once

#include "MemoryMap.h"
#include "MemoryArea.h"
#include <vector>

namespace GBE
{

    // From cartridge, switchable bank if any
    static constexpr MemoryMap MMAP_EXTERNAL_RAM(0xA000, 0xBFFF);

    // Work RAM
    static constexpr MemoryMap MMAP_WRAM(0xC000, 0xDFFF);

    // Nintendo says use of this area is prohibited.
    static constexpr MemoryMap MMAP_ECHO_RAM(0xE000, 0xFDFF);

    // High RAM
    static constexpr MemoryMap MMAP_HRAM(0xFF80, 0xFFFE);

    class Ram: public MemoryArea
    {
    public: 
        Ram(uint16_t size);
        ~Ram() = default;

    private:
        std::vector<uint8_t> m_Data{};

        uint8_t _GetImp(uint16_t address) const override;
        void _SetImp(uint16_t address, uint8_t value) override;
    };
} // namespace GBE
