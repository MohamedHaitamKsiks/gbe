#include "Cartridge.h"
#include <cassert>
#include <fstream>

namespace GBE
{
    void Cartridge::Load(std::string_view path)
    {
        // Open
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
        assert(file.is_open());

        // Get file size
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read bytes into buffer
        m_ROM = std::make_unique<uint8_t[]>(size);
        file.read(reinterpret_cast<char *>(m_ROM.get()), size);
    }

    void Cartridge::_SetImp(uint16_t address, uint8_t value)
    {
        m_ROM[address] = value;
    }

    uint8_t Cartridge::_GetImp(uint16_t address) const
    {
        return m_ROM[address];
    }

} // namespace GBE
