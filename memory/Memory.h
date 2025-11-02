#pragma once

#include "MemoryArea.h"
#include "MemoryMap.h"


#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <array>

namespace GBE
{
    // this is the interace used by the cpu to interact with different part of the hardware
    class Memory
    {
    public:
        ~Memory() = default;

        // expecting iterable of MemoryMap
        template<typename CollectionType>
        void MapMemoryArea(const CollectionType& mmaps, std::shared_ptr<MemoryArea> area)
        {
            std::vector<MemoryMap> vectorOfMaps{};
            for (const auto& map: mmaps)
            {
                vectorOfMaps.push_back(map);
            }

            _MapMemoryArea(vectorOfMaps, area);
        }

        void MapMemoryArea(std::initializer_list<MemoryMap> mmaps, std::shared_ptr<MemoryArea> area)
        {
            std::vector<MemoryMap> vectorOfMaps = mmaps;
            _MapMemoryArea(vectorOfMaps, area);
        }

        // set value at adress
       void Set(uint16_t address, uint8_t value);

        // get value from adress
        uint8_t Get(uint16_t address) const;

        // set value at adress
        void Set16(uint16_t address, uint16_t value);

        // get value from adress
        uint16_t Get16(uint16_t address) const;

        // copy buffer to memory
        void CopyBuffer(uint16_t address, const void *data, uint16_t size);

    private:
        uint16_t _FindMemoryArea(uint16_t address, MemoryArea*& oMemoryArea) const;
        
        struct AddressCache
        {
            uint16_t LocalAddress = 0;
            std::shared_ptr<MemoryArea> Area{nullptr};
        };

        mutable std::array<AddressCache, UINT16_MAX + 1> m_AddressCache{}; // cache should be modifiable even when const
        std::map<std::shared_ptr<MemoryArea>, std::set<MemoryMap>> m_MemoryAreas{};
        const std::shared_ptr<MemoryArea> m_MemoryAreaNotFound = nullptr;

        // map memory area
        void _MapMemoryArea(const std::vector<MemoryMap> &mmaps, std::shared_ptr<MemoryArea> area);
    };
} // namespace GBE
