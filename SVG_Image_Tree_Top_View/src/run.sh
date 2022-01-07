#!/bin/bash

name="CreateTree"
files='"CreateTree.cpp" "Utils.h" "Tree.h" "Tree.cpp"'

echo "---------------------------------------------------------------------"
echo "compiling ..."

# check old executable
if [ -f "$name" ]; then
    echo "remove $name ..."
    rm "$name"
fi

# check CMakeLists.txt
if [ ! -f "CMakeLists.txt" ]; then
    echo "create file CMakeLists.txt ..."
    # CMakeLists.txt
    echo "# CMakeLists.txt" > CMakeLists.txt
    echo "cmake_minimum_required(VERSION 3.16)" >> CMakeLists.txt
    echo "" >> CMakeLists.txt
    echo "project( $name )" >> CMakeLists.txt
    echo "" >> CMakeLists.txt
    echo "add_executable( $name $files )" >> CMakeLists.txt
    echo "" >> CMakeLists.txt 
    echo "target_compile_features($name PRIVATE cxx_std_20)" >> CMakeLists.txt
    ################ 
    echo "CMakeLists.txt ... ok"
else
    echo "CMakeLists.txt ... ok"
fi

# check build directory
if [ ! -d "build" ];then
    echo "create build directory ..."
    mkdir "build"
fi

path=$(pwd)
cd "./build"

# cmake
if [ ! -f "Makefile" ]; then
    echo "cmake CMakeLists.txt ..."
    echo "---------------------------- cmake ----------------------------------"
    cmake CMakeLists.txt ..
    echo "---------------------------------------------------------------------"
fi

# make
if [ -f "Makefile" ]; then
    echo "make ..."
    echo "---------------------------- make -----------------------------------"
    make
    echo "---------------------------------------------------------------------"
fi

# copy and run executable
if [ -f "$name" ]; then
    echo "copy $name to $path ..."
    cp $name $path
    cd $path
    echo "run $name ..."
    ./$name &
else
    echo "\"$name\" not found. check make!"
fi

echo "script fineshed!"
echo "---------------------------------------------------------------------"
echo ""
exit 0
