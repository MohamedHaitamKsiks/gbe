#pragma once

#include <memory>
#include <cstdint>

namespace GBE
{
    // this is the interace used by the cpu to interact with different part of the hardware
    class IMemory
    {
    public:
        virtual ~IMemory() {};

        // set value at adress
        virtual void Set(uint16_t address, uint8_t value) = 0;
        
        // get value from adress
        virtual uint8_t Get(uint16_t address) const = 0;

        // set value at adress
        void Set16(uint16_t address, uint16_t value);

        // get value from adress
        uint16_t Get16(uint16_t address) const; 
    
        // copy buffer to memory
        void CopyBuffer(uint16_t address, const void* data, uint16_t size);
    };
} // namespace GBE
