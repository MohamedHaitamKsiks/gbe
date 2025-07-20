
set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/lcd/LcdPalette.h
    ${CMAKE_CURRENT_LIST_DIR}/lcd/LcdScreen.h
    ${CMAKE_CURRENT_LIST_DIR}/lcd/LcdControl.h

    ${CMAKE_CURRENT_LIST_DIR}/oam/ObjectAttribute.h
    ${CMAKE_CURRENT_LIST_DIR}/oam/ObjectAttributesMemory.h

    ${CMAKE_CURRENT_LIST_DIR}/vram/TileData.h
    ${CMAKE_CURRENT_LIST_DIR}/vram/TileMap.h
    ${CMAKE_CURRENT_LIST_DIR}/vram/Vram.h

    ${CMAKE_CURRENT_LIST_DIR}/Ppu.h
    ${CMAKE_CURRENT_LIST_DIR}/PpuTask.h
    ${CMAKE_CURRENT_LIST_DIR}/PpuTaskManager.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/lcd/LcdPalette.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lcd/LcdControl.cpp

    ${CMAKE_CURRENT_LIST_DIR}/oam/ObjectAttribute.cpp
    ${CMAKE_CURRENT_LIST_DIR}/oam/ObjectAttributesMemory.cpp

    ${CMAKE_CURRENT_LIST_DIR}/vram/TileData.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vram/TileMap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vram/Vram.cpp

    ${CMAKE_CURRENT_LIST_DIR}/Ppu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PpuTask.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PpuTaskManager.cpp
)
