cmake_minimum_required(VERSION 3.20)
project(gbe LANGUAGES CXX)

# Set C++ standard

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# params
option(TEST "Enable tests" OFF)
option(COVERAGE "Enable coverage" OFF)

# enable coverage for test
if (COVERAGE)
    enable_testing()
    include(CTest)
endif()

#add debendencies
add_subdirectory(dependencies/doctest)

#add modules
if (TEST)
    include(tests/tests.cmake)
    include_directories(tests)
endif()

include(cpu/cpu.cmake)
include(memory/memory.cmake)

#create core library
add_library(gbelib STATIC
    ${GBE_SOURCES}
    ${GBE_INCLUDE}
    ${GBE_TESTS_SOURCES}
)

# Define the executable
if (TEST)
    set (MAIN_SOURCE platforms/tests/main.cpp)
else()
    set (MAIN_SOURCE platforms/desktop/main.cpp)
endif()

include_directories(.)
add_executable(gbe ${MAIN_SOURCE} ${GBE_TESTS_SOURCES})

target_link_libraries(gbelib
    PRIVATE
    doctest
)

#create library
target_link_libraries(gbe
    PRIVATE
    doctest
    gbelib
)

# add coverage
if (COVERAGE)

    target_compile_options(gbe PRIVATE -coverage)
    target_link_options(gbe PRIVATE -coverage)
    
    add_test(NAME gbe_coverarge COMMAND gbe)
endif()