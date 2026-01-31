cmake_minimum_required(VERSION 3.20)
project(gbe LANGUAGES CXX C)


# Set C++ standard

set(CMAKE_CXX_STANDARD 23)
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
add_subdirectory(dependencies/magic_enum)

set(GBE_LIBRARIES 
    magic_enum
    doctest
)

#add modules
if (TEST)
    include(tests/tests.cmake)
    include_directories(tests)
else()
    # Add SDL3 as subdirectory (builds it as part of your project)
    # Disable SDL3 tests, examples, and other unnecessary components
    add_subdirectory(dependencies/SDL3 EXCLUDE_FROM_ALL)

    set (GBE_LIBRARIES ${GBE_LIBRARIES}
        SDL3::SDL3
    )

    include(frontend/frontend.cmake)
endif()

include(cpu/cpu.cmake)
include(memory/memory.cmake)
include(io/io.cmake)
include(gameboy/gameboy.cmake)
include(cartridge/cartridge.cmake)
include(util/util.cmake)

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
    ${GBE_LIBRARIES}
)

#create library
target_link_options(gbe PRIVATE -rdynamic)

target_link_libraries(gbe
    PRIVATE
    gbelib
    ${GBE_LIBRARIES}
)

# add coverage
if (COVERAGE)
    target_compile_options(gbe PRIVATE -coverage)
    target_link_options(gbe PRIVATE -coverage)
    
    add_test(NAME gbe_coverarge COMMAND gbe)
endif()