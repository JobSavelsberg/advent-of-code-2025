cmake_minimum_required(VERSION 3.10.0)
project(AdventOfCode2025 LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CUDA_STANDARD 14)

set(DAY 10)

# Use boost
find_package(Boost REQUIRED)

# Use z3 for LP Solver in day 10
set(Z3_ROOT "C:/Tools/z3/z3-4.13.0-x64-win")
set(Z3_INCLUDE_DIR "${Z3_ROOT}/include")
set(Z3_LIBRARY "${Z3_ROOT}/bin/libz3.lib")
set(Z3_DLL "${Z3_ROOT}/bin/libz3.dll")

# Create utility library
add_library(utils src/util/input.cpp)
target_include_directories(utils PUBLIC src/util)
target_compile_definitions(utils PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Create executable for the specified day
file(GLOB DAY_SOURCE "src/day${DAY}/day${DAY}.*pp" "src/day${DAY}/day${DAY}.cu")
add_executable(day${DAY} ${DAY_SOURCE})

# Link libraries
target_link_libraries(day${DAY} PRIVATE utils Boost::boost)

if(DAY EQUAL 10)
    target_include_directories(day${DAY} PRIVATE ${Z3_INCLUDE_DIR})
    target_link_libraries(day${DAY} PRIVATE ${Z3_LIBRARY})

    # Copy Z3 DLL to output directory
    add_custom_command(TARGET day${DAY} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${Z3_DLL}
        $<TARGET_FILE_DIR:day${DAY}>
    )
endif()

target_compile_definitions(day${DAY} PRIVATE
    DAY=${DAY}
    SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
