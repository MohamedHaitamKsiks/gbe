include(${CMAKE_CURRENT_LIST_DIR}/rendering/rendering.cmake)

set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/Application.h
    ${CMAKE_CURRENT_LIST_DIR}/gui/GuiLayer.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/GuiLayer.cpp
)
