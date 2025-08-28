#include <gtest/gtest.h>
#include "calculator.h"
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

/**
 * Test Fixtures Demonstration
 * This file demonstrates various types of test fixtures in Google Test:
 * 1. Basic Test Fixtures with SetUp() and TearDown()
 * 2. Test Suite Fixtures with SetUpTestSuite() and TearDownTestSuite()
 * 3. Global Setup and Teardown
 * 4. Parameterized Test Fixtures
 */

// ============================================================================
// GLOBAL TEST ENVIRONMENT
// ============================================================================

/**
 * Global test environment - runs once for the entire test program
 * Use this for expensive setup that should happen only once
 */
class GlobalTestEnvironment : public ::testing::Environment {
public:
    // Called before any test case is run
    void SetUp() override {
        std::cout << "[GLOBAL] Setting up global test environment" << std::endl;
        // Example: Initialize logging, create test databases, etc.
        global_resource_initialized = true;
    }
    
    // Called after all test cases have been run
    void TearDown() override {
        std::cout << "[GLOBAL] Tearing down global test environment" << std::endl;
        // Example: Cleanup global resources, close connections, etc.
        global_resource_initialized = false;
    }
    
    static bool global_resource_initialized;
};

bool GlobalTestEnvironment::global_resource_initialized = false;

// Register the global environment - this is needed for the tests to work
static ::testing::Environment* const global_env = ::testing::AddGlobalTestEnvironment(new GlobalTestEnvironment);

// ============================================================================
// BASIC TEST FIXTURE
// ============================================================================

/**
 * Basic Test Fixture Class
 * 
 * SetUp() and TearDown() methods are called before and after EACH test method
 * Use this pattern when:
 * - Each test needs a fresh, clean state
 * - Setup/cleanup is relatively inexpensive
 * - Tests should not affect each other
 */
class CalculatorFixtureTest : public ::testing::Test {
protected:
    /**
     * SetUp() is called immediately before each test method
     * Use this to:
     * - Initialize test objects
     * - Set up test data
     * - Prepare the environment for the test
     */
    void SetUp() override {
        std::cout << "[FIXTURE] SetUp() called - preparing test" << std::endl;
        
        // Create a fresh calculator for each test
        calculator = std::make_unique<Calculator>();
        
        // Initialize test data
        test_numbers = {1, 2, 3, 4, 5};
        test_strings = {"hello", "world", "test"};
        
        // Set initial state
        calculator->setValue(0);
        setup_call_count++;
        
        // Verify global environment is ready
        ASSERT_TRUE(GlobalTestEnvironment::global_resource_initialized);
    }
    
    /**
     * TearDown() is called immediately after each test method
     * Use this to:
     * - Clean up resources
     * - Reset state
     * - Verify post-conditions
     */
    void TearDown() override {
        std::cout << "[FIXTURE] TearDown() called - cleaning up test" << std::endl;
        
        // Clean up test objects
        calculator.reset();
        
        // Clear test data
        test_numbers.clear();
        test_strings.clear();
        
        teardown_call_count++;
        
        // Verify that SetUp was called
        EXPECT_GT(setup_call_count, 0);
    }
    
    // Test fixture data members
    std::unique_ptr<Calculator> calculator;
    std::vector<int> test_numbers;
    std::vector<std::string> test_strings;
    
    // Static counters to verify fixture behavior
    static int setup_call_count;
    static int teardown_call_count;
};

// Initialize static members
int CalculatorFixtureTest::setup_call_count = 0;
int CalculatorFixtureTest::teardown_call_count = 0;

/**
 * Test using the basic fixture
 * Each test gets a fresh Calculator object and clean test data
 */
TEST_F(CalculatorFixtureTest, BasicArithmetic) {
    // Calculator is guaranteed to be initialized and have value 0
    EXPECT_EQ(0, calculator->getValue());
    
    // Test basic operations
    EXPECT_EQ(8, calculator->add(5, 3));
    EXPECT_EQ(2, calculator->subtract(5, 3));
    
    // Modify calculator state
    calculator->setValue(42);
    EXPECT_EQ(42, calculator->getValue());
}

TEST_F(CalculatorFixtureTest, StringOperations) {
    // Fresh calculator for this test (SetUp was called again)
    EXPECT_EQ(0, calculator->getValue());
    
    // Test string operations using fixture data
    EXPECT_EQ("helloworld", calculator->concatenate(test_strings[0], test_strings[1]));
    EXPECT_EQ("HELLO", calculator->toUpperCase(test_strings[0]));
    
    // Previous test's modifications don't affect this test
    EXPECT_EQ(0, calculator->getValue());
}

TEST_F(CalculatorFixtureTest, VectorOperations) {
    // Test using the test_numbers vector from fixture
    int sum = 0;
    for (int num : test_numbers) {
        sum = calculator->add(sum, num);
    }
    EXPECT_EQ(15, sum); // 1+2+3+4+5 = 15
}

// ============================================================================
// TEST SUITE FIXTURE
// ============================================================================

/**
 * Test Suite Fixture Class
 * 
 * SetUpTestSuite() and TearDownTestSuite() are called once per test suite
 * Use this pattern when:
 * - Setup/cleanup is expensive (file I/O, network connections, etc.)
 * - Multiple tests can share the same setup
 * - Tests don't modify the shared state, or modifications are acceptable
 */
class DatabaseTestSuite : public ::testing::Test {
protected:
    /**
     * SetUpTestSuite() is called once before the first test in this suite
     * Use this for expensive setup operations that can be shared
     * 
     * IMPORTANT: This method must be static!
     */
    static void SetUpTestSuite() {
        std::cout << "[SUITE] SetUpTestSuite() called - expensive setup" << std::endl;
        
        // Simulate expensive setup (e.g., database connection, file loading)
        shared_database_file = "test_database.txt";
        
        // Create a test database file
        std::ofstream file(shared_database_file);
        file << "user1,Alice,25\n";
        file << "user2,Bob,30\n";
        file << "user3,Charlie,35\n";
        file.close();
        
        // Initialize shared resources
        shared_calculator = std::make_unique<Calculator>();
        suite_setup_called = true;
        
        std::cout << "[SUITE] Expensive setup completed" << std::endl;
    }
    
    /**
     * TearDownTestSuite() is called once after the last test in this suite
     * Use this to clean up expensive resources
     * 
     * IMPORTANT: This method must be static!
     */
    static void TearDownTestSuite() {
        std::cout << "[SUITE] TearDownTestSuite() called - expensive cleanup" << std::endl;
        
        // Clean up shared resources
        shared_calculator.reset();
        
        // Remove test database file
        if (!shared_database_file.empty()) {
            std::remove(shared_database_file.c_str());
        }
        
        suite_teardown_called = true;
        std::cout << "[SUITE] Expensive cleanup completed" << std::endl;
    }
    
    /**
     * SetUp() is still called before each individual test
     * Use this for per-test setup that's different from suite setup
     */
    void SetUp() override {
        std::cout << "[SUITE] Individual test SetUp()" << std::endl;
        
        // Verify suite setup was called
        ASSERT_TRUE(suite_setup_called);
        ASSERT_NE(nullptr, shared_calculator.get());
        
        // Reset calculator state for each test (if needed)
        shared_calculator->setValue(0);
        individual_setup_count++;
    }
    
    void TearDown() override {
        std::cout << "[SUITE] Individual test TearDown()" << std::endl;
        individual_teardown_count++;
    }
    
    // Shared resources (must be static for suite-level fixtures)
    static std::unique_ptr<Calculator> shared_calculator;
    static std::string shared_database_file;
    static bool suite_setup_called;
    static bool suite_teardown_called;
    
    // Per-test counters
    static int individual_setup_count;
    static int individual_teardown_count;
};

// Initialize static members
std::unique_ptr<Calculator> DatabaseTestSuite::shared_calculator = nullptr;
std::string DatabaseTestSuite::shared_database_file = "";
bool DatabaseTestSuite::suite_setup_called = false;
bool DatabaseTestSuite::suite_teardown_called = false;
int DatabaseTestSuite::individual_setup_count = 0;
int DatabaseTestSuite::individual_teardown_count = 0;

/**
 * Tests using the suite fixture
 * All tests share the same expensive setup but get individual SetUp/TearDown
 */
TEST_F(DatabaseTestSuite, ReadDatabaseFile) {
    // Shared resources are available
    ASSERT_NE(nullptr, shared_calculator.get());
    ASSERT_FALSE(shared_database_file.empty());
    
    // Read from the shared database file
    std::ifstream file(shared_database_file);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) {
        line_count++;
        EXPECT_FALSE(line.empty());
    }
    
    EXPECT_EQ(3, line_count); // Should have 3 users
}

TEST_F(DatabaseTestSuite, SharedCalculatorOperations) {
    // Use the shared calculator
    ASSERT_NE(nullptr, shared_calculator.get());
    
    // Calculator state was reset in SetUp()
    EXPECT_EQ(0, shared_calculator->getValue());
    
    // Perform operations
    EXPECT_EQ(10, shared_calculator->add(4, 6));
    EXPECT_EQ("HELLO", shared_calculator->toUpperCase("hello"));
    
    // Modify state (this won't affect other tests due to SetUp() reset)
    shared_calculator->setValue(100);
}

TEST_F(DatabaseTestSuite, AnotherSharedTest) {
    // Calculator state was reset again
    EXPECT_EQ(0, shared_calculator->getValue());
    
    // Database file is still available
    std::ifstream file(shared_database_file);
    EXPECT_TRUE(file.is_open());
}

// ============================================================================
// PARAMETERIZED TEST FIXTURE
// ============================================================================

/**
 * Parameterized Test Fixture
 * Use this to run the same test with different input values
 */
class ParameterizedCalculatorTest : public ::testing::TestWithParam<std::pair<int, int>> {
protected:
    void SetUp() override {
        calculator = std::make_unique<Calculator>();
        
        // Get the current test parameter
        auto param = GetParam();
        first_number = param.first;
        second_number = param.second;
    }
    
    void TearDown() override {
        calculator.reset();
    }
    
    std::unique_ptr<Calculator> calculator;
    int first_number;
    int second_number;
};

/**
 * Parameterized test - runs with different parameter values
 */
TEST_P(ParameterizedCalculatorTest, AdditionTest) {
    int result = calculator->add(first_number, second_number);
    int expected = first_number + second_number;
    
    EXPECT_EQ(expected, result);
    
    // Also test that addition is commutative
    int reverse_result = calculator->add(second_number, first_number);
    EXPECT_EQ(result, reverse_result);
}

TEST_P(ParameterizedCalculatorTest, MultiplicationTest) {
    int result = calculator->multiply(first_number, second_number);
    int expected = first_number * second_number;
    
    EXPECT_EQ(expected, result);
    
    // Test that multiplication is commutative
    int reverse_result = calculator->multiply(second_number, first_number);
    EXPECT_EQ(result, reverse_result);
}

// Define the parameter values for the parameterized tests
INSTANTIATE_TEST_SUITE_P(
    BasicOperations,
    ParameterizedCalculatorTest,
    ::testing::Values(
        std::make_pair(1, 2),
        std::make_pair(5, 3),
        std::make_pair(-2, 4),
        std::make_pair(0, 10),
        std::make_pair(-5, -3)
    )
);

// ============================================================================
// TYPED TEST FIXTURE
// ============================================================================

/**
 * Typed Test Fixture for testing with different numeric types
 */
template<typename T>
class TypedCalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        zero = static_cast<T>(0);
        one = static_cast<T>(1);
        two = static_cast<T>(2);
        five = static_cast<T>(5);
    }
    
    T zero, one, two, five;
};

// Define the types to test with
typedef ::testing::Types<int, long, float, double> NumericTypes;
TYPED_TEST_SUITE(TypedCalculatorTest, NumericTypes);

/**
 * Typed test - runs with different type parameters
 */
TYPED_TEST(TypedCalculatorTest, BasicTypeOperations) {
    // Test basic operations with different numeric types
    EXPECT_EQ(this->two, this->one + this->one);
    EXPECT_EQ(this->zero, this->one - this->one);
    EXPECT_LT(this->zero, this->one);
    EXPECT_GT(this->five, this->two);
}

// ============================================================================
// FIXTURE VERIFICATION TESTS
// ============================================================================

/**
 * Test to verify that fixtures are working correctly
 */
class FixtureVerificationTest : public ::testing::Test {
protected:
    void SetUp() override {
        verification_setup_called = true;
    }
    
    void TearDown() override {
        EXPECT_TRUE(verification_setup_called);
        verification_teardown_called = true;
    }
    
    static bool verification_setup_called;
    static bool verification_teardown_called;
};

bool FixtureVerificationTest::verification_setup_called = false;
bool FixtureVerificationTest::verification_teardown_called = false;

TEST_F(FixtureVerificationTest, VerifySetupCalled) {
    // Verify that SetUp was called
    EXPECT_TRUE(verification_setup_called);
    
    // Verify that TearDown hasn't been called yet
    EXPECT_FALSE(verification_teardown_called);
}

TEST_F(FixtureVerificationTest, VerifyFixtureIsolation) {
    // Each test gets fresh SetUp call
    EXPECT_TRUE(verification_setup_called);
    
    // Previous test's TearDown doesn't affect this test
    // (TearDown is called after each test, but static variable persists)
}

// ============================================================================
// FIXTURE BEST PRACTICES DEMONSTRATION
// ============================================================================

/**
 * Demonstrates best practices for test fixtures
 */
class BestPracticesFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // 1. Always check preconditions
        ASSERT_TRUE(GlobalTestEnvironment::global_resource_initialized);
        
        // 2. Initialize all member variables
        calculator = std::make_unique<Calculator>();
        test_data_initialized = true;
        
        // 3. Use ASSERT for critical setup that must succeed
        ASSERT_NE(nullptr, calculator.get());
    }
    
    void TearDown() override {
        // 1. Clean up in reverse order of setup
        test_data_initialized = false;
        calculator.reset();
        
        // 2. Verify cleanup was successful
        EXPECT_EQ(nullptr, calculator.get());
    }
    
    // Helper method for common test operations
    void PerformBasicCalculations() {
        ASSERT_NE(nullptr, calculator.get());
        EXPECT_EQ(5, calculator->add(2, 3));
        EXPECT_EQ(6, calculator->multiply(2, 3));
    }
    
    std::unique_ptr<Calculator> calculator;
    bool test_data_initialized = false;
};

TEST_F(BestPracticesFixture, DemonstrateHelperMethods) {
    EXPECT_TRUE(test_data_initialized);
    
    // Use helper method to reduce code duplication
    PerformBasicCalculations();
    
    // Additional test-specific operations
    EXPECT_TRUE(calculator->isPositive(5));
}

TEST_F(BestPracticesFixture, DemonstrateProperAssertions) {
    // Use ASSERT for conditions that must be true for the test to continue
    ASSERT_TRUE(test_data_initialized);
    ASSERT_NE(nullptr, calculator.get());
    
    // Use EXPECT for actual test conditions
    EXPECT_EQ("HELLO", calculator->toUpperCase("hello"));
    EXPECT_FALSE(calculator->isEmpty("not empty"));
}