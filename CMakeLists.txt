# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(self_defined_protocol)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_EXTENSIONS OFF)
    add_compile_options(-pedantic-errors)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -Wall -Wextra -Werror
        -Wold-style-cast
        -Wno-c++11-extensions
        -Werror=vla
        -Werror=format-security
    )
endif()

add_subdirectory(src)