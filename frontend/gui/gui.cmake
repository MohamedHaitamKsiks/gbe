include(${CMAKE_CURRENT_LIST_DIR}/window/window.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/menu/menu.cmake)


set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/GuiManager.h
    ${CMAKE_CURRENT_LIST_DIR}/GuiLayer.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/GuiManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GuiLayer.cpp
)
