#pragma once

#include <cstdint>
#include <array>

namespace GBE
{
    constexpr uint8_t OBJECT_ATTRIBUTE_SIZE = 4;

    enum class ObjectAttributeFlag
    {
        PRIORITY = 7,
        Y_FLIP = 6,
        X_FLIP = 5,
        DMG_PALETTE = 4,
    };

    class ObjectAttribute
    {
    public:
        ~ObjectAttribute() {}

        inline void Set(uint16_t address, uint8_t value)
        {
            m_Data[address] = value;
        }

        inline uint8_t Get(uint16_t address) const
        {
            return m_Data.at(address);
        }

        // Byte 0 — Y Position
        inline uint8_t GetYPosition() const
        {
            return m_Data.at(0);
        }

        // Byte 1 — X Position
        inline uint8_t GetXPosition() const
        {
            return m_Data.at(1);
        }

        // Byte 2 — Tile Index
        inline uint8_t GetTileIndex() const
        {
            return m_Data.at(2);
        }


        // x flip
        bool GetXFlip() const;

        // y flip
        bool GetYFlip() const;

        // priority
        uint8_t GetPriority() const;

        // get dmg palette
        uint8_t GetDMGPalette() const;

    private:
        std::array<uint8_t, OBJECT_ATTRIBUTE_SIZE> m_Data{};

        // Byte 3 — Attributes/Flags
        inline uint8_t _GetFlags() const
        {
            return m_Data.at(3);
        }

        // Test flag
        bool _TestFlag(ObjectAttributeFlag flag) const;
    };
} // namespace GBE
