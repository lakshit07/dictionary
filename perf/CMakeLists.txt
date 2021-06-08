IF(NOT DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

add_executable(suffixTreePerf
        SuffixTree.b.cpp)
target_link_libraries(suffixTreePerf
        benchmark::benchmark
        benchmark::benchmark_main)