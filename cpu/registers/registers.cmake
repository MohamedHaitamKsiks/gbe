set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/CpuFlags.h
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegister.h
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegistersSet.h
    
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegister.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CpuRegistersSet.cpp
)
