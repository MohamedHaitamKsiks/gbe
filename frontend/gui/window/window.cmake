set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/GuiWindow.h
    ${CMAKE_CURRENT_LIST_DIR}/GuiDebugger.h
    ${CMAKE_CURRENT_LIST_DIR}/GuiCpuState.h
    ${CMAKE_CURRENT_LIST_DIR}/GuiDisassembler.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/GuiWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GuiDebugger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GuiCpuState.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GuiDisassembler.cpp
)
