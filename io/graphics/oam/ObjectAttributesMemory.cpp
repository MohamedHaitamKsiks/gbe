#include "ObjectAttributesMemory.h"

namespace GBE
{
    ObjectAttributesMemory::ObjectAttributesMemory()
    {
    }

    ObjectAttributesMemory::~ObjectAttributesMemory()
    {
    }

    void ObjectAttributesMemory::Set(uint16_t address, uint8_t value)
    {
        uint16_t objectIndex = address / OBJECT_ATTRIBUTE_SIZE;
        uint16_t objectLocalAddress = address % OBJECT_ATTRIBUTE_SIZE;

        ObjectAttribute& object = m_Objects.at(objectIndex);
        object.Set(objectLocalAddress, value);
    }

    uint8_t ObjectAttributesMemory::Get(uint16_t address) const
    {
        uint16_t objectIndex = address / OBJECT_ATTRIBUTE_SIZE;
        uint16_t objectLocalAddress = address % OBJECT_ATTRIBUTE_SIZE;

        const ObjectAttribute &object = m_Objects.at(objectIndex);
        return object.Get(objectLocalAddress);
    }

} // namespace GBE
