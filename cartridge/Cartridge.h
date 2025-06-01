#pragma once

#include <memory>
#include <string_view>

namespace GBE
{
    class Cartridge
    {
    public:
        ~Cartridge() {}
        void Load(std::string_view path);
    private:
        std::unique_ptr<uint8_t[]> m_ROM = nullptr;
        std::unique_ptr<uint8_t[]> m_RAM = nullptr;
    };
} // namespace GBE
