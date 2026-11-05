cmake_minimum_required(VERSION 3.20)
project(Lab17 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab17_graph main.cpp graph.cpp traversal.cpp)

enable_testing()
add_test(NAME graph COMMAND lab17_graph --test)
set_tests_properties(graph PROPERTIES
    PASS_REGULAR_EXPRESSION "graph tests: 18/18 passed"
    TIMEOUT 10
)
