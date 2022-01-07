# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

project( CreateTree )

add_executable( CreateTree "CreateTree.cpp" "Utils.h" "Tree.h" "Tree.cpp" )

target_compile_features(CreateTree PRIVATE cxx_std_20)
