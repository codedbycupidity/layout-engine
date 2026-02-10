cmake_minimum_required(VERSION 3.15)
project(MiniLayoutEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable (update as files are added)
add_executable(layout_engine
    src/main.cpp
    src/core/EdgeValues.cpp
    src/core/LayoutNode.cpp
    src/layout/BlockLayout.cpp
    src/layout/FlexLayout.cpp
)

target_include_directories(layout_engine PRIVATE src)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Test executable (update as test files are added)
add_executable(layout_tests
    # Add test files as created
    tests/test_edge_values.cpp
    tests/tests_layout_node.cpp
    tests/test_layout_constraints.cpp
    tests/test_block_layout.cpp
    tests/test_flex_layout.cpp
    src/core/EdgeValues.cpp
    src/core/LayoutNode.cpp
    src/layout/BlockLayout.cpp
    src/layout/FlexLayout.cpp
)

target_link_libraries(layout_tests gtest_main)
target_include_directories(layout_tests PRIVATE src)

enable_testing()
add_test(NAME layout_tests COMMAND layout_tests)
