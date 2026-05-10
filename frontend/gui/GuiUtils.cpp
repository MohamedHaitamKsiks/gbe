#include "GuiUtils.h"

#include "util/Assert.h"

#include "imgui.h"

#include <cstdint>
#include <format>
#include <array>

namespace GBE
{
    template <>
    int GuiUtils::InputTypeOf<uint16_t>()
    {
        return ImGuiDataType_U16;
    }

    template <>
    int GuiUtils::InputTypeOf<uint8_t>()
    {
        return ImGuiDataType_U8;
    }

    void GuiUtils::Text(std::string_view text)
    {
        ImGui::Text(text.begin());
    }

    void GuiUtils::_InputHex(std::string_view label, void *values, int count, int type, int size)
    {
        GBE_ASSERT(type != -1);

        // create format
        std::array<char, 33> format = { 0 };
        std::format_to_n(format.begin(), format.size() - 1, "%0{}X", size * 2);

        ImGui::Text("0x");
        ImGui::SameLine();

        ImGui::InputScalarN(label.begin(), ImGuiDataType_U16, values, count, NULL, NULL, format.begin(), ImGuiInputTextFlags_CharsHexadecimal);
    }
} // namespace GBE
