cmake_minimum_required(VERSION 3.10.0)
project(AdventOfCode2025 LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CUDA_STANDARD 14)

set(DAY 6)

# Use boost
find_package(Boost REQUIRED)

# Create utility library
add_library(utils src/util/input.cpp)
target_include_directories(utils PUBLIC src/util)
target_compile_definitions(utils PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Create executable for the specified day
file(GLOB DAY_SOURCE "src/day${DAY}/day${DAY}.*pp" "src/day${DAY}/day${DAY}.cu")
add_executable(day${DAY} ${DAY_SOURCE})

# Link libraries
target_link_libraries(day${DAY} PRIVATE utils Boost::boost)
target_compile_definitions(day${DAY} PRIVATE
    DAY=${DAY}
    SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
