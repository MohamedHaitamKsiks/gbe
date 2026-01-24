include(${CMAKE_CURRENT_LIST_DIR}/alu/alu.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/instruction/instruction.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/registers/registers.cmake)

set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/Cpu.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/CpuAlu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CpuJump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CpuLoad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CpuStack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CpuRun.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Cpu.cpp
)