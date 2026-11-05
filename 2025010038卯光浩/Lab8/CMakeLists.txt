cmake_minimum_required(VERSION 3.20)
project(Lab8 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab8_stack main.cpp)
add_executable(lab8_callstack callstack.cpp)
