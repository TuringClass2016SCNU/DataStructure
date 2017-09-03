cmake_minimum_required(VERSION 3.6)
project(AVL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        AVL.h
        AVL_delete.cpp
        AVL_insert.cpp
        general_function.cpp)

add_executable(AVL ${SOURCE_FILES})