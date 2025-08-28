#include <gtest/gtest.h>
#include "calculator.h"
#include <stdexcept>

/**
 * Basic Assertions Test Suite
 * This file demonstrates various types of assertions available in Google Test
 */

class BasicAssertionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures before each test
        calc = std::make_unique<Calculator>();
    }
    
    void TearDown() override {
        // Clean up after each test
        calc.reset();
    }
    
    std::unique_ptr<Calculator> calc;
};

// ============================================================================
// BOOLEAN ASSERTIONS
// ============================================================================

/**
 * Test boolean assertions using EXPECT_TRUE and EXPECT_FALSE
 * These are the most basic assertions for testing boolean conditions
 */
TEST_F(BasicAssertionsTest, BooleanAssertions) {
    // Test EXPECT_TRUE - expects the condition to be true
    EXPECT_TRUE(calc->isPositive(5));
    EXPECT_TRUE(calc->isEven(4));
    EXPECT_TRUE(calc->isEmpty(""));
    
    // Test EXPECT_FALSE - expects the condition to be false
    EXPECT_FALSE(calc->isPositive(-3));
    EXPECT_FALSE(calc->isEven(7));
    EXPECT_FALSE(calc->isEmpty("not empty"));
    
    // Using ASSERT_* versions - these will stop the test on failure
    ASSERT_TRUE(calc->isPositive(1));
    ASSERT_FALSE(calc->isPositive(-1));
}

// ============================================================================
// NUMERIC ASSERTIONS
// ============================================================================

/**
 * Test numeric equality and comparison assertions
 * Demonstrates EXPECT_EQ, EXPECT_NE, EXPECT_LT, EXPECT_LE, EXPECT_GT, EXPECT_GE
 */
TEST_F(BasicAssertionsTest, NumericAssertions) {
    // Test equality assertions
    EXPECT_EQ(8, calc->add(5, 3));        // Equal
    EXPECT_NE(10, calc->add(5, 3));       // Not equal
    
    // Test comparison assertions
    EXPECT_LT(5, calc->add(3, 4));        // Less than
    EXPECT_LE(7, calc->add(3, 4));        // Less than or equal
    EXPECT_GT(10, calc->add(2, 3));       // Greater than
    EXPECT_GE(5, calc->add(2, 3));        // Greater than or equal
    
    // Test with different operations
    EXPECT_EQ(6, calc->subtract(10, 4));
    EXPECT_EQ(35, calc->multiply(5, 7));
    
    // Test floating point numbers with EXPECT_DOUBLE_EQ for exact equality
    EXPECT_DOUBLE_EQ(2.5, calc->divide(5.0, 2.0));
    
    // Test floating point numbers with EXPECT_NEAR for approximate equality
    EXPECT_NEAR(3.333333, calc->divide(10.0, 3.0), 0.000001);
}

/**
 * Test factorial function with various inputs
 * Demonstrates testing mathematical functions with edge cases
 */
TEST_F(BasicAssertionsTest, FactorialTest) {
    // Test basic factorial calculations
    EXPECT_EQ(1, calc->factorial(0));     // 0! = 1
    EXPECT_EQ(1, calc->factorial(1));     // 1! = 1
    EXPECT_EQ(2, calc->factorial(2));     // 2! = 2
    EXPECT_EQ(6, calc->factorial(3));     // 3! = 6
    EXPECT_EQ(24, calc->factorial(4));    // 4! = 24
    EXPECT_EQ(120, calc->factorial(5));   // 5! = 120
}

// ============================================================================
// STRING ASSERTIONS
// ============================================================================

/**
 * Test string-related assertions
 * Demonstrates EXPECT_STREQ, EXPECT_STRNE, and string operations
 */
TEST_F(BasicAssertionsTest, StringAssertions) {
    // Test string equality
    EXPECT_STREQ("HelloWorld", calc->concatenate("Hello", "World").c_str());
    EXPECT_STRNE("hello", calc->concatenate("Hello", "World").c_str());
    
    // Test string operations using EXPECT_EQ with std::string
    EXPECT_EQ("HELLO", calc->toUpperCase("hello"));
    EXPECT_EQ("TEST", calc->toUpperCase("test"));
    EXPECT_EQ("", calc->toUpperCase(""));
    
    // Test string length
    EXPECT_EQ(5, calc->getLength("hello"));
    EXPECT_EQ(0, calc->getLength(""));
    EXPECT_EQ(11, calc->getLength("hello world"));
    
    // Test string concatenation with various inputs
    EXPECT_EQ("ab", calc->concatenate("a", "b"));
    EXPECT_EQ("a", calc->concatenate("a", ""));
    EXPECT_EQ("b", calc->concatenate("", "b"));
    EXPECT_EQ("", calc->concatenate("", ""));
}

// ============================================================================
// EXCEPTION ASSERTIONS
// ============================================================================

/**
 * Test exception handling assertions
 * Demonstrates EXPECT_THROW, EXPECT_NO_THROW, and EXPECT_ANY_THROW
 */
TEST_F(BasicAssertionsTest, ExceptionAssertions) {
    // Test that specific exceptions are thrown
    EXPECT_THROW(calc->divide(5.0, 0.0), std::invalid_argument);
    EXPECT_THROW(calc->factorial(-1), std::invalid_argument);
    EXPECT_THROW(calc->squareRoot(-4.0), std::invalid_argument);
    
    // Test that no exceptions are thrown for valid inputs
    EXPECT_NO_THROW(calc->divide(10.0, 2.0));
    EXPECT_NO_THROW(calc->factorial(5));
    EXPECT_NO_THROW(calc->squareRoot(16.0));
    
    // Test that any exception is thrown (useful when you don't care about the specific type)
    EXPECT_ANY_THROW(calc->divide(1.0, 0.0));
}

// ============================================================================
// FLOATING POINT ASSERTIONS
// ============================================================================

/**
 * Test floating point specific assertions
 * Demonstrates proper handling of floating point comparisons
 */
TEST_F(BasicAssertionsTest, FloatingPointAssertions) {
    // Test exact floating point equality (use with caution)
    EXPECT_FLOAT_EQ(4.0f, static_cast<float>(calc->squareRoot(16.0)));
    EXPECT_DOUBLE_EQ(3.0, calc->squareRoot(9.0));
    
    // Test approximate floating point equality (recommended for most cases)
    EXPECT_NEAR(2.236, calc->squareRoot(5.0), 0.001);
    EXPECT_NEAR(1.414, calc->squareRoot(2.0), 0.001);
    
    // Test division results with floating point precision
    EXPECT_NEAR(0.333333, calc->divide(1.0, 3.0), 0.000001);
    EXPECT_NEAR(1.666667, calc->divide(5.0, 3.0), 0.000001);
}

// ============================================================================
// DEATH TESTS (Advanced)
// ============================================================================

/**
 * Test death tests - for testing that the program terminates in expected ways
 * Note: Death tests should be used sparingly and are platform-dependent
 */
TEST_F(BasicAssertionsTest, DeathTests) {
    // These tests are commented out as they require specific build configurations
    // and may not work in all environments
    
    // EXPECT_DEATH(calc->divide(1.0, 0.0), ".*");
    // EXPECT_EXIT(calc->factorial(-1), ::testing::ExitedWithCode(1), ".*");
}

// ============================================================================
// PREDICATE ASSERTIONS
// ============================================================================

/**
 * Custom predicate function for demonstration
 */
bool IsEvenNumber(int n) {
    return n % 2 == 0;
}

/**
 * Test predicate assertions using EXPECT_PRED*
 * Demonstrates how to use custom predicates in tests
 */
TEST_F(BasicAssertionsTest, PredicateAssertions) {
    // Test using predicate functions
    EXPECT_PRED1(IsEvenNumber, 4);
    EXPECT_PRED1(IsEvenNumber, calc->multiply(2, 3));
    
    // Using lambda expressions as predicates
    auto isPositive = [](int n) { return n > 0; };
    EXPECT_PRED1(isPositive, calc->add(5, 3));
    EXPECT_PRED1(isPositive, calc->multiply(2, 4));
}

// ============================================================================
// TYPED TESTS (Advanced)
// ============================================================================

/**
 * Example of testing with different numeric types
 * This demonstrates how to write generic tests
 */
template<typename T>
class NumericTest : public ::testing::Test {
protected:
    T zero = static_cast<T>(0);
    T one = static_cast<T>(1);
    T two = static_cast<T>(2);
};

// Define the types we want to test with
typedef ::testing::Types<int, long, float, double> NumericTypes;
TYPED_TEST_SUITE(NumericTest, NumericTypes);

TYPED_TEST(NumericTest, BasicOperations) {
    // Test that work with any numeric type
    EXPECT_EQ(this->zero, this->one - this->one);
    EXPECT_EQ(this->two, this->one + this->one);
    EXPECT_LT(this->zero, this->one);
}