#pragma once

#include <string>
#include <cstdint>
#include <string_view>

namespace GBE
{
    // cpu register pair like BC ..
    class CpuRegister
    {
    public:
        CpuRegister() = default;
        ~CpuRegister();

        // set full 16 bits 
        void Set16(uint16_t value);

        // get as 16 bits number
        uint16_t Get16() const;

        // set high 8bits
        inline void SetHigh(uint8_t value)
        {
            m_High = value & m_HighMask;
        }

        // get high 8bits
        inline uint8_t GetHigh() const
        {
            return m_High;
        }

        // set low 8 bits
        inline void SetLow(uint8_t value)
        {
            m_Low = value & m_LowMask;
        }

        // get low 8bits
        uint8_t GetLow() const
        {
            return m_Low;
        }

        std::string ToString(std::string_view name, bool showHighLow = true) const;

        inline void SetHightMask(uint8_t mask)
        {
            m_HighMask = mask;
        }

        inline void SetLowMask(uint8_t mask)
        {
            m_LowMask = mask;
        }

    private:
        uint8_t m_High = 0;
        uint8_t m_HighMask = 0xFF;

        uint8_t m_Low = 0;
        uint8_t m_LowMask = 0xFF;
    };
} // namespace GBE
