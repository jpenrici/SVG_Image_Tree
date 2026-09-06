cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
  CreateTree
  VERSION 1.0.0
  DESCRIPTION "Randomized top-view SVG tree generator"
  LANGUAGES CXX)

set(PROGRAM_NAME create_svg_tree)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Debug"
      CACHE STRING "Build type" FORCE)
endif()

add_executable(${PROGRAM_NAME} src/main.cpp src/cli.cpp src/geometry.cpp
                               src/svg_writer.cpp src/tree.cpp)

target_include_directories(${PROGRAM_NAME} PRIVATE include)

target_compile_options(${PROGRAM_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                               -Wconversion)
