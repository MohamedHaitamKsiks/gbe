#pragma once

#include "MemoryArea.h"
#include <vector>

namespace GBE
{
    class Ram: public MemoryArea
    {
    public: 
        Ram(uint16_t size);
        ~Ram() = default;

    private:
        std::vector<uint8_t> m_Data{};

        uint8_t _GetImp(uint16_t address) const override;
        void _SetImp(uint16_t address, uint8_t value) override;
    };
} // namespace GBE
