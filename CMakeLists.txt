cmake_minimum_required(VERSION 3.15)
project(dictionary)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(perf)