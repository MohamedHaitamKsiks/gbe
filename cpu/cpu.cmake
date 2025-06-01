set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/Cpu.h
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegistersSet.h
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegister.h
    ${CMAKE_CURRENT_LIST_DIR}/CpuInstructionResult.h
    ${CMAKE_CURRENT_LIST_DIR}/CpuFlags.h
    ${CMAKE_CURRENT_LIST_DIR}/Alu.h
    ${CMAKE_CURRENT_LIST_DIR}/AluResult.h
    ${CMAKE_CURRENT_LIST_DIR}/AssemblyInstruction.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/CpuRun.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Cpu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegistersSet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegister.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Alu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AssemblyInstruction.cpp
)
