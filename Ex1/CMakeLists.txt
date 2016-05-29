cmake_minimum_required(VERSION 3.3)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
    MathIsFun.c
    MathIsFun.h main.c)

add_executable(hw1 ${SOURCE_FILES} main.c)