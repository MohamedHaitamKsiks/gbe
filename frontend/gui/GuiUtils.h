#pragma once

#include <string_view>
#include <format>

namespace GBE
{
    // class with utility functions for gui
    class GuiUtils
    {
    public:
        // input int as hex
        template<typename T> 
        static void InputHex(std::string_view label, T* values, int count)
        {
            int type = InputTypeOf<T>();
            _InputHex(label, values, count, type, sizeof(T));
        }

        template<typename T> 
        static void InputHex(std::string_view label, T* values)
        {
            int type = InputTypeOf<T>();
            _InputHex(label, values, 1, type, sizeof(T));
        }

        // show text using std::format (like std::print)
        template <typename FirstArg, typename... Args>
        static void Text(std::string_view fmt, FirstArg &&firstArg, Args&&...args)
        {
            std::string text = std::format(fmt, std::forward(firstArg, args...));
            Text(text);
        }

        // show text 
        static void Text(std::string_view text);

        template <typename T>
        static int InputTypeOf();

    private:
        static void _InputHex(std::string_view, void* values, int count, int type, int size);
    };
} // namespace GBE
