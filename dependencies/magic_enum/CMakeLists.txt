cmake_minimum_required(VERSION 3.20)
project(magic_enum LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create an INTERFACE library (header-only)
add_library(magic_enum INTERFACE)

# Include directory (modify "include" to match your folder structure)
target_include_directories(magic_enum INTERFACE .)
