cmake_minimum_required(VERSION 3.20)
project(Lab6 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(lab6_list linkedlist.c)
add_executable(lab6_circ circlist.c)