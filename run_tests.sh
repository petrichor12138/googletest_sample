#!/bin/bash

# Google Test Sample Project Test Runner
# This script provides various options for running tests

set -e

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

# Check if test executable exists
TEST_EXECUTABLE="build/bin/sample_tests"
if [ ! -f "$TEST_EXECUTABLE" ]; then
    print_error "Test executable not found: $TEST_EXECUTABLE"
    print_status "Please run ./build.sh first to build the project"
    exit 1
fi

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help              Show this help message"
    echo "  -v, --verbose           Run tests with verbose output"
    echo "  -f, --filter PATTERN    Run only tests matching the pattern"
    echo "  -l, --list              List all available tests"
    echo "  -r, --repeat N          Repeat tests N times"
    echo "  -s, --shuffle           Shuffle test execution order"
    echo "  -x, --xml               Generate XML test report"
    echo "  -j, --json              Generate JSON test report"
    echo "  --basic                 Run only basic assertion tests"
    echo "  --mock                  Run only mock tests"
    echo "  --fixture               Run only fixture tests"
    echo "  --all                   Run all tests (default)"
    echo ""
    echo "Examples:"
    echo "  $0                      # Run all tests"
    echo "  $0 -v                   # Run all tests with verbose output"
    echo "  $0 -f "*Mock*"          # Run only mock tests"
    echo "  $0 --basic              # Run only basic assertion tests"
    echo "  $0 -x                   # Generate XML report"
    echo "  $0 -r 5                 # Repeat tests 5 times"
}

# Default values
VERBOSE=false
FILTER=""
LIST_TESTS=false
REPEAT=1
SHUFFLE=false
XML_OUTPUT=false
JSON_OUTPUT=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -f|--filter)
            FILTER="$2"
            shift 2
            ;;
        -l|--list)
            LIST_TESTS=true
            shift
            ;;
        -r|--repeat)
            REPEAT="$2"
            shift 2
            ;;
        -s|--shuffle)
            SHUFFLE=true
            shift
            ;;
        -x|--xml)
            XML_OUTPUT=true
            shift
            ;;
        -j|--json)
            JSON_OUTPUT=true
            shift
            ;;
        --basic)
            FILTER="BasicAssertionsTest*"
            shift
            ;;
        --mock)
            FILTER="*Mock*"
            shift
            ;;
        --fixture)
            FILTER="*Fixture*"
            shift
            ;;
        --all)
            FILTER=""
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Build test command
TEST_CMD="$TEST_EXECUTABLE"

# Add verbose flag
if [ "$VERBOSE" = true ]; then
    TEST_CMD="$TEST_CMD --gtest_verbose"
fi

# Add filter
if [ -n "$FILTER" ]; then
    TEST_CMD="$TEST_CMD --gtest_filter=$FILTER"
fi

# Add repeat
if [ "$REPEAT" -gt 1 ]; then
    TEST_CMD="$TEST_CMD --gtest_repeat=$REPEAT"
fi

# Add shuffle
if [ "$SHUFFLE" = true ]; then
    TEST_CMD="$TEST_CMD --gtest_shuffle"
fi

# Add XML output
if [ "$XML_OUTPUT" = true ]; then
    mkdir -p test_reports
    TEST_CMD="$TEST_CMD --gtest_output=xml:test_reports/test_results.xml"
fi

# Add JSON output
if [ "$JSON_OUTPUT" = true ]; then
    mkdir -p test_reports
    TEST_CMD="$TEST_CMD --gtest_output=json:test_reports/test_results.json"
fi

# List tests if requested
if [ "$LIST_TESTS" = true ]; then
    print_status "Available tests:"
    $TEST_EXECUTABLE --gtest_list_tests
    exit 0
fi

# Run tests
print_status "Running Google Test Sample Project tests..."
print_status "Command: $TEST_CMD"
echo ""

if $TEST_CMD; then
    echo ""
    print_success "All tests completed successfully!"
    
    if [ "$XML_OUTPUT" = true ]; then
        print_status "XML report generated: test_reports/test_results.xml"
    fi
    
    if [ "$JSON_OUTPUT" = true ]; then
        print_status "JSON report generated: test_reports/test_results.json"
    fi
else
    echo ""
    print_error "Some tests failed!"
    exit 1
fi