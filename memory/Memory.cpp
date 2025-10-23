#include "Memory.h"
#include <iostream>

namespace GBE
{
    void Memory::MapMemoryArea(const std::vector<MemoryMap> &mmaps, std::shared_ptr<MemoryArea> area)
    {
        std::set<MemoryMap> orderedMMaps{};
        for (const auto& mmap: mmaps)
            orderedMMaps.insert(mmap);

        m_MemoryAreas[area] = std::move(orderedMMaps);
    }

    void Memory::Set(uint16_t address, uint8_t value)
    {
        std::shared_ptr<MemoryArea> marea = nullptr;
        uint16_t localAddress = _FindMemoryArea(address, marea);

        if (marea)
            marea->Set(localAddress, value);
    }

    uint8_t Memory::Get(uint16_t address) const
    {
        std::shared_ptr<MemoryArea> marea = nullptr;
        uint16_t localAddress = _FindMemoryArea(address, marea);

        if (!marea)
            return 0xFF;

        return marea->Get(localAddress);
    }

    uint16_t Memory::_FindMemoryArea(uint16_t address, std::shared_ptr<MemoryArea> &oMemoryArea) const
    {
        for (const auto& [marea, mmaps]: m_MemoryAreas)
        {
            uint16_t localAddress = 0;
            for (const auto& mmap: mmaps)
            {
                if (mmap.In(address))
                {
                    oMemoryArea = marea;
                    localAddress += address - mmap.GetStart();
                    return localAddress;
                }
                localAddress += mmap.GetSize();
            }
        }

        oMemoryArea = nullptr;
        return 0;
    }

} // namespace GBE
