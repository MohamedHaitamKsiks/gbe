#include <sstream>
#include <iomanip>
#include <string>

namespace GBE
{
    // namespace to group function on binary numbers
    class Binary
    {
    public:
        template <typename T>
        static std::string ToHex(T value)
        {
            std::stringstream ss;
            ss << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << static_cast<size_t>(value);

            return ss.str();
        }

        template <typename T>
        static T SetBit(T input, T bit)
        {
            T mask = 1 << bit;
            return input | mask;
        }

        template <typename T>
        static bool TestBit(T input, T bit)
        {
            T mask = 1 << bit;
            return (input & mask);
        }

        template <typename T>
        static T ResetBit(T input, T bit)
        {
            T mask = 1 << bit;
            return input & ~(input & mask);
        }
    };
} // namespace GBE
