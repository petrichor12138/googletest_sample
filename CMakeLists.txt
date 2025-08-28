cmake_minimum_required(VERSION 3.14)
project(GoogleTestSample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# Use system-installed Google Test (installed via brew)
find_package(GTest REQUIRED)
include(GoogleTest)

# Include directories
include_directories(src)
include_directories(include)

# Create directories if they don't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Source files
set(SOURCES
    src/calculator.cpp
    src/database.cpp
)

# Create library
add_library(sample_lib ${SOURCES})
target_include_directories(sample_lib PUBLIC include)

# Main executable
add_executable(sample_main src/main.cpp)
target_link_libraries(sample_main sample_lib)

# Create test executable
add_executable(sample_tests
    tests/basic_assertions_test.cpp
    tests/mock_test.cpp
    tests/fixture_test.cpp
)

# Link test executable with libraries
target_link_libraries(sample_tests
    sample_lib
    GTest::gtest_main
    GTest::gmock_main
)

# Enable testing
enable_testing()

# Add test
add_test(NAME SampleTests COMMAND sample_tests)

# Compiler-specific options
if(MSVC)
    target_compile_options(sample_lib PRIVATE /W4)
else()
    target_compile_options(sample_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set output directories
set_target_properties(sample_main sample_tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(sample_lib
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)