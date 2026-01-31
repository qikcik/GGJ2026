cmake_minimum_required(VERSION 3.30)
project(GGJ2026 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

#set(CMAKE_CXX_FLAGS "-Og")
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/raylib.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/module.cmake")

add_subdirectory("module/core")
add_subdirectory("module/coreEditor")
add_subdirectory("module/editor")