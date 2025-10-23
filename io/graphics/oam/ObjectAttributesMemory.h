#pragma once

#include <vector>
#include <cstdint>

#include "ObjectAttribute.h"

#include "memory/MemoryMap.h"
#include "memory/MemoryArea.h"

namespace GBE
{
    constexpr uint16_t OBJECT_COUNT = 40;
    constexpr uint16_t OAM_SIZE = OBJECT_ATTRIBUTE_SIZE * OBJECT_COUNT;

    // OAM memory map
    static constexpr MemoryMap MMAP_OAM(0xFE00, 0xFE9F);

    class ObjectAttributesMemory: public MemoryArea
    {
    public:
        ObjectAttributesMemory();
        ~ObjectAttributesMemory();

        inline const ObjectAttribute& GetObject(uint8_t objectID) const
        {
            return m_Objects.at(objectID);
        }

        
    private:
        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;

        std::vector<ObjectAttribute> m_Objects{};
    };

} // namespace GBE
