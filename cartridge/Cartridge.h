#pragma once

#include "memory/MemoryArea.h"

#include <memory>
#include <string_view>

namespace GBE
{
    class Cartridge: public MemoryArea
    {
    public:
        ~Cartridge() {}

        void LoadFromAssets(std::string_view path);
        void Load(std::string_view path);
    private:
        std::unique_ptr<uint8_t[]> m_ROM = nullptr;
        std::unique_ptr<uint8_t[]> m_RAM = nullptr;

        void _SetImp(uint16_t address, uint8_t value) override;
        uint8_t _GetImp(uint16_t address) const override;
        std::string _GetExeFullPath();
    };
} // namespace GBE
