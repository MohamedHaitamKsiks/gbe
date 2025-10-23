include(${CMAKE_CURRENT_LIST_DIR}/graphics/graphics.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/interrupts/interrupts.cmake)

set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/IORegister.h
)

set(GBE_SOURCES ${GBE_SOURCES}
)
