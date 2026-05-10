include(${CMAKE_CURRENT_LIST_DIR}/window/window.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/menu/menu.cmake)


set (GBE_HEADERS ${GBE_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/GuiManager.h
    ${CMAKE_CURRENT_LIST_DIR}/GuiLayer.h
    ${CMAKE_CURRENT_LIST_DIR}/GuiUtils.h
)

set(GBE_SOURCES ${GBE_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/GuiManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GuiLayer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GuiUtils.cpp
)
