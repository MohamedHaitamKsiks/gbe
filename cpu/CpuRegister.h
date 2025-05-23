#pragma once

#include <cstdint>

namespace GBE
{
    // cpu register pair like BC ..
    class CpuRegister
    {
    public:
    
        ~CpuRegister();

        // set full 16 bits 
        void Set16(uint16_t value);

        // get as 16 bits number
        uint16_t Get16() const;

        // set high 8bits
        inline void SetHigh(uint8_t value)
        {
            m_High = value;
        }

        // get high 8bits
        inline uint8_t GetHigh() const
        {
            return m_High;
        }

        // set low 8 bits
        inline void SetLow(uint8_t value)
        {
            m_Low = value;
        }

        // get low 8bits
        uint8_t GetLow() const
        {
            return m_Low;
        }

    private:
        uint8_t m_High = 0;
        uint8_t m_Low = 0;

    };
} // namespace GBE
