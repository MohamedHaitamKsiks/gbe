set (GBE_TESTS_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/GBETestSuite.h
    ${CMAKE_CURRENT_LIST_DIR}/cpu/CpuTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpu/CpuRegistersSetTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpu/CpuRegisterTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cpu/AluTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/memory/MemoryTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/graphics/LcdPaletteTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/graphics/TileDataTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io/graphics/PpuTest.cpp
)