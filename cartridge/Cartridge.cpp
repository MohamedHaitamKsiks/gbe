#include "Cartridge.h"
#include <cassert>
#include <fstream>

#ifdef _WIN32

#else
    #include <sys/param.h>
#endif


namespace GBE
{
    void Cartridge::LoadFromAssets(std::string_view path)
    {
        std::string exePath = _GetExeFullPath();
        std::string fullPath = exePath + std::string("/") + std::string(path);

        Load(fullPath);
    }

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

        SetReadFlag(true);
        SetWriteFlag(false);
    }

    void Cartridge::_SetImp(uint16_t address, uint8_t value)
    {
        m_ROM[address] = value;
    }

    uint8_t Cartridge::_GetImp(uint16_t address) const
    {
        return m_ROM[address];
    }

    std::string Cartridge::_GetExeFullPath()
    {
        // get exe path
        char buff[10000];
        size_t len = sizeof(buff);

        int bytes = MIN(readlink("/proc/self/exe", buff, len), len - 1);
        if (bytes >= 0)
            buff[bytes] = '\0';

        std::string exePath = buff;
        
        // erase exe from path (we only want folder)
        size_t exeStartIndex = exePath.find_last_of("/");
        return exePath.substr(0, exeStartIndex);
    }

} // namespace GBE
