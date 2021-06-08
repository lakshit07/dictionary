enable_testing()

add_executable(suffixTreeTest
        SuffixTree.t.cpp)

target_link_libraries(suffixTreeTest
        GTest::GTest
        GTest::Main)

gtest_discover_tests(suffixTreeTest WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})