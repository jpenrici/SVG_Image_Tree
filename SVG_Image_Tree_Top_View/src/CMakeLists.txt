cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
  CreateTree
  VERSION 1.0.0
  DESCRIPTION "Simple top-view SVG tree generator"
  LANGUAGES CXX)

set(PROGRAM_NAME create_svg_tree)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Debug"
      CACHE STRING "Build type" FORCE)
endif()

set(SOURCES main.cpp svg.hpp tree.hpp tree.cpp)

add_executable(${PROGRAM_NAME} ${SOURCES})

target_compile_options(${PROGRAM_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                               -Wconversion)
