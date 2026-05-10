#include "GuiMemoryDump.h"

#include "frontend/gui/GuiUtils.h"

#include "memory/Memory.h"
#include "memory/Ram.h"
#include "gameboy/Gameboy.h"
#include "util/Binary.h"

#include "imgui.h"

namespace GBE
{
    GuiMemoryDump::GuiMemoryDump(
        std::shared_ptr<Window> window, 
        std::shared_ptr<Renderer> renderer, 
        std::shared_ptr<Gameboy> gameboy
    ):
        GuiWindow(window, renderer, gameboy)
    {
        SetName("Memory dump");
        // SetVisible(true);

        m_DumpRange[0] = static_cast<int>(MMAP_WRAM.GetStart());
        m_DumpRange[1] = static_cast<int>(MMAP_WRAM.GetEnd());

        m_NumberOfCols = 16;
    }

    void GuiMemoryDump::_RenderWindow()
    {
        Memory& memory = m_Gameboy->GetMemory();

        ImGui::PushID("#MemoryDumpRange");
        GuiUtils::InputHex<uint16_t>("", m_DumpRange.begin(), m_DumpRange.size());
        ImGui::PopID();

        ImGui::InputInt("Number of columns", &m_NumberOfCols);

        ImGui::NewLine();


        ImGui::BeginTable("Memory Dump", m_NumberOfCols + 1);

        uint16_t dumpStartAddress = m_DumpRange[0];
        uint16_t dumpEndAddress = m_DumpRange[1];

        uint16_t dumpSize = dumpEndAddress - dumpStartAddress;

        uint16_t numberOfCols = static_cast<uint16_t>(m_NumberOfCols);
        uint16_t numberOfLines = dumpSize / static_cast<uint16_t>(m_NumberOfCols);

        uint16_t currentAddress = dumpStartAddress;

        // optimization to avoid creating string for each address
        std::array<char, 16> hexCurrentAddress = { 0 };
        std::array<char, 16> hexCurrentValue = { 0 };

        for (uint16_t line = 0; line < numberOfLines; line++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            // show address at each line
            hexCurrentAddress.fill(0);
            std::format_to_n(hexCurrentAddress.begin(), hexCurrentAddress.size() - 1, "{0:#06x}", currentAddress);
            ImGui::Text(hexCurrentAddress.cbegin());
            for (uint16_t col = 0; col < numberOfCols; col++)
            {
                ImGui::TableNextColumn();

                if (currentAddress > dumpEndAddress)
                {
                    ImGui::Text("0x--");
                    continue;
                }


                uint8_t value = memory.Get(currentAddress);

                hexCurrentValue.fill(0);
                std::format_to_n(hexCurrentValue.begin(), hexCurrentValue.size() - 1, "{0:#04x}", value);
                ImGui::Text(hexCurrentValue.cbegin());

                currentAddress++;
            }
        }

        ImGui::EndTable();
    }
} // namespace GBE
