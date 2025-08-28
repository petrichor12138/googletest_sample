#!/bin/bash

# Google Test Sample Project Build Script
# This script automates the build process for the Google Test sample project

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    print_error "CMake is not installed. Please install CMake first."
    exit 1
fi

# Check if make is installed
if ! command -v make &> /dev/null; then
    print_error "Make is not installed. Please install build tools first."
    exit 1
fi

print_status "Starting Google Test Sample Project build..."

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    print_warning "Build directory exists. Cleaning..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
print_status "Created build directory: $BUILD_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

# Configure with CMake
print_status "Configuring project with CMake..."
if cmake .. -DCMAKE_BUILD_TYPE=Debug; then
    print_success "CMake configuration completed"
else
    print_error "CMake configuration failed"
    exit 1
fi

# Build the project
print_status "Building project..."
if make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4); then
    print_success "Build completed successfully"
else
    print_error "Build failed"
    exit 1
fi

# Navigate back to project root
cd ..

print_success "Build process completed!"
print_status "Generated files:"
echo "  - Executable: build/bin/sample_main"
echo "  - Test executable: build/bin/sample_tests"
echo "  - Library: build/lib/libsample_lib.a"

print_status "To run the main program: ./build/bin/sample_main"
print_status "To run tests: ./build/bin/sample_tests"
print_status "To run tests with verbose output: ./build/bin/sample_tests --gtest_verbose"
print_status "To run specific tests: ./build/bin/sample_tests --gtest_filter=TestName*"

print_success "Build script completed successfully!"