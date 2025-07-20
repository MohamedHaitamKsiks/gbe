#include "ObjectAttribute.h"

#include "util/Binary.h"

namespace GBE
{
    bool ObjectAttribute::GetXFlip() const
    {
        return _TestFlag(ObjectAttributeFlag::X_FLIP);
    }

    bool ObjectAttribute::GetYFlip() const
    {
        return _TestFlag(ObjectAttributeFlag::Y_FLIP);
    }

    uint8_t ObjectAttribute::GetPriority() const
    {
        if (_TestFlag(ObjectAttributeFlag::PRIORITY))
            return 1;

        return 0;
    }

    uint8_t ObjectAttribute::GetDMGPalette() const
    {
        if (_TestFlag(ObjectAttributeFlag::DMG_PALETTE))
            return 1;

        return 0;
    }

    bool ObjectAttribute::_TestFlag(ObjectAttributeFlag flag) const
    {
        uint8_t flags = _GetFlags();
        return Binary::TestBit(flags, static_cast<uint8_t>(flag));
    }

} // namespace GBE
