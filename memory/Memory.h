#pragma once

#include "IMemory.h"
#include "MemoryArea.h"
#include "MemoryMap.h"


#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>

namespace GBE
{
    // this is the interace used by the cpu to interact with different part of the hardware
    class Memory: public IMemory
    {
    public:
        ~Memory() = default;

        // map memory area
        void MapMemoryArea(const std::vector<MemoryMap> &mmaps, std::shared_ptr<MemoryArea> area);

        // set value at adress
       void Set(uint16_t address, uint8_t value) override;

        // get value from adress
        uint8_t Get(uint16_t address) const override;

    private:
        uint16_t _FindMemoryArea(uint16_t address, std::shared_ptr<MemoryArea>& oMemoryArea) const;

        // std::vector<std::pair<>>
        std::map<std::shared_ptr<MemoryArea>, std::set<MemoryMap>> m_MemoryAreas{};
        const std::shared_ptr<MemoryArea> m_MemoryAreaNotFound = nullptr;
    };
} // namespace GBE
