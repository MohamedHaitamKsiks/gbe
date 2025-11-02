#include "MemoryMap.h"
#include <unordered_map>


// 3. Create a hash function specialization
namespace std
{
    template <>
    struct hash<GBE::MemoryMap>
    {
        std::size_t operator()(const GBE::MemoryMap &obj) const
        {
            return std::hash<int>()(obj.GetStart());
        }
    };
}

namespace GBE
{    
    // in hash map two memory maps cannot have same start so we only use start to hash and compare
    bool MemoryMap::operator==(const MemoryMap &other) const
    {
        return other.m_Start == m_Start;
    }

    bool MemoryMap::operator<(const MemoryMap &other) const
    {
        return m_Start < other.m_Start;
    }

    bool MemoryMap::operator>(const MemoryMap &other) const
    {
        return m_Start > other.m_Start;
    }
} // namespace GBE
