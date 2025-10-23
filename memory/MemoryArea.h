#pragma once

#include <cstdint>

namespace GBE
{

    // base class for any memory area 
    // memory area has read and write privileges
    // make sure to declare are you memory areas as shared pointers since they will be owned by the memory interface too
    class MemoryArea
    {
    public:
        virtual ~MemoryArea() {}

        inline bool GetReadFlag() const 
        {
            return m_ReadFlag;
        }

        inline void SetReadFlag(bool readFlag)
        {
            m_ReadFlag = readFlag;
        } 

        inline bool GetWriteFlag() const
        {
            return m_WriteFlag;
        }

        inline void SetWriteFlag(bool writeFlag)
        {
            m_WriteFlag = writeFlag;
        }

        // enable or disable read and write flags at the same time
        void SetReadWriteFlags(bool flags);

        // set byte at address
        void Set(uint16_t address, uint8_t value);
        
        // get byte from address
        uint8_t Get(uint16_t address) const;
    protected:
        virtual void _SetImp(uint16_t address, uint8_t value) = 0;
        virtual uint8_t _GetImp(uint16_t address) const = 0;
        
    private:
        bool m_ReadFlag = false;
        bool m_WriteFlag = false;
    };
} // namespace GBE
