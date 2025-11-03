#include "Memory.h"
#include <iostream>

namespace GBE
{
    void Memory::_MapMemoryArea(const std::vector<MemoryMap> &mmaps, std::shared_ptr<MemoryArea> area)
    {
        std::set<MemoryMap> orderedMMaps{};
        for (const auto& mmap: mmaps)
            orderedMMaps.insert(mmap);

        m_MemoryAreas[area] = std::move(orderedMMaps);
    }

    void Memory::Set(uint16_t address, uint8_t value)
    {
        MemoryArea* marea = nullptr;
        uint16_t localAddress = _FindMemoryArea(address, marea);

        if (marea)
            marea->Set(localAddress, value);
    }

    uint8_t Memory::Get(uint16_t address) const
    {
        MemoryArea* marea = nullptr;
        uint16_t localAddress = _FindMemoryArea(address, marea);

        if (!marea)
            return 0x0;

        return marea->Get(localAddress);
    }

    uint16_t Memory::_FindMemoryArea(uint16_t address, MemoryArea*& oMemoryArea) const
    {
        AddressCache& cache = m_AddressCache[address];
        if (cache.Area)
        {
            oMemoryArea = cache.Area.get();
            return cache.LocalAddress;
        }

        for (const auto& [marea, mmaps]: m_MemoryAreas)
        {
            uint16_t localAddress = 0;
            for (const auto& mmap: mmaps)
            {
                if (mmap.In(address))
                {
                    oMemoryArea = marea.get();
                    localAddress += address - mmap.GetStart();
                    
                    // cache area
                    cache.Area = marea;
                    cache.LocalAddress = localAddress;
                    
                    return localAddress;
                }
                localAddress += mmap.GetSize();
            }
        }

        oMemoryArea = nullptr;
        return 0;
    }

    void Memory::Set16(uint16_t address, uint16_t value)
    {
        Set(address, value & 0xff);
        Set(address + 1, value >> 8);
    }

    uint16_t Memory::Get16(uint16_t address) const
    {
        uint16_t little = Get(address);
        uint16_t big = Get(address + 1);

        return little + (big << 8);
    }

    void Memory::CopyBuffer(uint16_t address, const void *data, uint16_t size)
    {
        for (uint16_t i = 0; i < size; i++)
        {
            Set(i + address, static_cast<const uint8_t *>(data)[i]);
        }
    }

} // namespace GBE
