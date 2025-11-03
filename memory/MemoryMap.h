#pragma once

#include <cstdint>

namespace GBE
{
    // memory map range 
    class MemoryMap
    {
    public:
        constexpr MemoryMap() {};
        constexpr MemoryMap(uint16_t start, uint16_t end): m_Start(start), m_End(end) {}

        constexpr inline uint16_t GetStart() const
        {
            return m_Start;
        }

        constexpr inline uint16_t GetEnd() const
        {
            return m_End;
        }

        // get range size
        constexpr inline uint16_t GetSize() const
        {
            return m_End - m_Start + 1;
        }

        // check addr is inside range
        constexpr inline bool In(uint16_t address) const
        {
            return address >= m_Start && address <= m_End;
        }

        bool operator==(const MemoryMap& other) const;

        bool operator<(const MemoryMap &other) const;

        bool operator>(const MemoryMap &other) const;

    private:
        uint16_t m_Start = 0;
        uint16_t m_End = 0;
        
    };

    // notes from https://gbdev.io/pandocs/Memory_Map.html
    
    // From cartridge, usually a fixed bank
    static constexpr MemoryMap MMAP_ROM_BANK_0(0x0, 0x3FFF);

    // From cartridge, switchable bank via mapper (if any)
    static constexpr MemoryMap MMAP_ROM_BANK_1_N(0x4000, 0x7FFF);


    // Nintendo says use of this area is prohibited.
    static constexpr MemoryMap MMAP_NOT_USABLE(0xFEA0, 0xFEFF);


} // namespace GBE
