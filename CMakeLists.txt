cmake_minimum_required(VERSION 3.15)
project(MiniLayoutEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable (update as files are added)
add_executable(layout_engine
    src/main.cpp
    src/core/EdgeValues.cpp
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
    src/core/EdgeValues.cpp  # Need to link implementation for tests
)

target_link_libraries(layout_tests gtest_main)
target_include_directories(layout_tests PRIVATE src)

enable_testing()
add_test(NAME layout_tests COMMAND layout_tests)
