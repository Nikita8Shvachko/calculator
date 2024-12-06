cmake_minimum_required(VERSION 3.27)
project(calculator)

set(CMAKE_CXX_STANDARD 20)
add_library(sin SHARED ./plugins/sin.cpp)

add_executable(calculator main.cpp
        Calc/Calculator.h
        Calc/Calculator.cpp)
