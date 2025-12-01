cmake_minimum_required(VERSION 3.10.0)
project(AdventOfCode2025 VERSION 0.1.0 LANGUAGES C CXX)

set(DAY 1 CACHE STRING "Day of Advent of Code to build")

# Create utility library
add_library(utils src/util/input.cpp)
target_include_directories(utils PUBLIC src/util)
target_compile_definitions(utils PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

#  Create executable for the specified day
add_executable(day${DAY} src/day${DAY}/day${DAY}.cpp)
target_link_libraries(day${DAY} PRIVATE utils)
target_compile_definitions(day${DAY} PRIVATE 
    DAY=${DAY}
    SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)