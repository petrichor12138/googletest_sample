# Google Test Sample Project

A comprehensive sample project demonstrating Google Test (gtest) and Google Mock (gmock) features with CMake build system.

## 项目概述 (Project Overview)

This project provides comprehensive examples of Google Test framework usage, including:
- Basic assertions for different data types
- Mock testing with detailed MOCK_METHOD usage
- Test fixtures with SetUp/TearDown and SetUpTestSuite/TearDownTestSuite
- Advanced testing patterns and best practices

## 项目结构 (Project Structure)

```
googletest_sample/
├── CMakeLists.txt              # CMake build configuration
├── README.md                   # This documentation
├── build.sh                    # Build script
├── run_tests.sh               # Test runner script
├── include/                    # Header files
│   ├── calculator.h           # Calculator class for basic assertions
│   └── database_interface.h   # Database interface for mock testing
├── src/                       # Source files
│   ├── calculator.cpp         # Calculator implementation
│   ├── database.cpp           # Database service implementation
│   └── main.cpp              # Main program
└── tests/                     # Test files
    ├── basic_assertions_test.cpp  # Basic assertion examples
    ├── mock_test.cpp             # Mock testing examples
    └── fixture_test.cpp          # Test fixture examples
```

## 构建要求 (Build Requirements)

- CMake 3.14 or higher
- C++17 compatible compiler
- Internet connection (for downloading Google Test)

## 快速开始 (Quick Start)

### 1. 构建项目 (Build Project)

```bash
# Method 1: Use build script (recommended)
./build.sh

# Method 2: Manual build
mkdir build && cd build
cmake ..
make
```

### 2. 运行程序 (Run Main Program)

```bash
./build/bin/sample_main
```

### 3. 运行测试 (Run Tests)

```bash
# Method 1: Use test runner script (recommended)
./run_tests.sh

# Method 2: Direct execution
./build/bin/sample_tests
```

## 测试示例详解 (Test Examples Explained)

### 1. 基本断言测试 (Basic Assertions)

Location: `tests/basic_assertions_test.cpp`

#### Boolean Assertions (布尔断言)
```cpp
// Test boolean conditions
EXPECT_TRUE(calc->isPositive(5));   // Expects true
EXPECT_FALSE(calc->isPositive(-3)); // Expects false
ASSERT_TRUE(calc->isPositive(1));   // Stops test on failure
```

#### Numeric Assertions (数值断言)
```cpp
// Equality and comparison
EXPECT_EQ(8, calc->add(5, 3));      // Equal
EXPECT_NE(10, calc->add(5, 3));     // Not equal
EXPECT_LT(5, calc->add(3, 4));      // Less than
EXPECT_LE(7, calc->add(3, 4));      // Less than or equal
EXPECT_GT(10, calc->add(2, 3));     // Greater than
EXPECT_GE(5, calc->add(2, 3));      // Greater than or equal

// Floating point comparisons
EXPECT_DOUBLE_EQ(2.5, calc->divide(5.0, 2.0));           // Exact equality
EXPECT_NEAR(3.333333, calc->divide(10.0, 3.0), 0.000001); // Approximate equality
```

#### String Assertions (字符串断言)
```cpp
// String equality
EXPECT_STREQ("HelloWorld", calc->concatenate("Hello", "World").c_str());
EXPECT_EQ("HELLO", calc->toUpperCase("hello"));
```

#### Exception Assertions (异常断言)
```cpp
// Test exception throwing
EXPECT_THROW(calc->divide(5.0, 0.0), std::invalid_argument);
EXPECT_NO_THROW(calc->divide(10.0, 2.0));
EXPECT_ANY_THROW(calc->divide(1.0, 0.0));
```

### 2. Mock 测试 (Mock Testing)

Location: `tests/mock_test.cpp`

#### MOCK_METHOD 宏使用要点 (MOCK_METHOD Macro Key Points)

**重要：括号使用规则 (Important: Parentheses Usage Rules)**

```cpp
class MockDatabase : public DatabaseInterface {
public:
    // ✅ Correct: No parameters - use empty parentheses
    MOCK_METHOD(void, disconnect, (), (override));
    
    // ✅ Correct: With parameters - wrap in parentheses
    MOCK_METHOD(bool, insertUser, (const std::string& name, int age), (override));
    
    // ✅ Correct: Const method - add (const) specifier
    MOCK_METHOD(std::string, getLastError, (), (const, override));
    
    // ✅ Correct: Reference parameters - proper syntax
    MOCK_METHOD(bool, executeQuery, (const std::string& query, std::vector<std::string>& results), (override));
    
    // ❌ Wrong: Missing parentheses around parameters
    // MOCK_METHOD(bool, insertUser, const std::string& name, int age, (override));
    
    // ❌ Wrong: Missing parentheses for no parameters
    // MOCK_METHOD(void, disconnect, (override));
};
```

#### Mock 期望设置 (Mock Expectations)

```cpp
// Basic expectation with return value
EXPECT_CALL(*mockDb, connect("test_connection"))
    .WillOnce(Return(true));

// Call counting
EXPECT_CALL(*mockDb, connect(_))
    .Times(Exactly(1))           // Exactly once
    .Times(AtLeast(2))          // At least twice
    .Times(AtMost(3))           // At most 3 times
    .WillRepeatedly(Return(true));

// Parameter matching
EXPECT_CALL(*mockDb, insertUser("Alice", 25))  // Specific values
EXPECT_CALL(*mockDb, insertUser(_, 30))        // Any string, specific age

// Multiple return values
EXPECT_CALL(*mockDb, getUserName(1))
    .WillOnce(Return("Alice"))      // First call
    .WillRepeatedly(Return("Unknown")); // Subsequent calls
```

#### 高级 Mock 功能 (Advanced Mock Features)

```cpp
// Reference parameter modification
EXPECT_CALL(*mockDb, executeQuery("SELECT * FROM users", _))
    .WillOnce(DoAll(
        SetArgReferee<1>(std::vector<std::string>{"Alice", "Bob"}),
        Return(true)
    ));

// Custom actions with Invoke
EXPECT_CALL(*mockDb, connect(_))
    .WillRepeatedly(Invoke([](const std::string& conn) {
        return conn.find("valid") != std::string::npos;
    }));

// Ordered expectations
InSequence seq;
EXPECT_CALL(*mockDb, connect(_));
EXPECT_CALL(*mockDb, insertUser(_, _));
EXPECT_CALL(*mockDb, disconnect());
```

### 3. 测试装置 (Test Fixtures)

Location: `tests/fixture_test.cpp`

#### 基本测试装置 (Basic Test Fixture)

```cpp
class CalculatorFixtureTest : public ::testing::Test {
protected:
    // Called before each test
    void SetUp() override {
        calculator = std::make_unique<Calculator>();
        // Initialize test data
    }
    
    // Called after each test
    void TearDown() override {
        calculator.reset();
        // Clean up resources
    }
    
    std::unique_ptr<Calculator> calculator;
};

TEST_F(CalculatorFixtureTest, TestName) {
    // Use calculator - it's guaranteed to be initialized
}
```

#### 测试套件装置 (Test Suite Fixture)

```cpp
class DatabaseTestSuite : public ::testing::Test {
protected:
    // Called once before all tests in this suite
    static void SetUpTestSuite() {
        // Expensive setup (database connections, file loading, etc.)
        shared_resource = std::make_unique<ExpensiveResource>();
    }
    
    // Called once after all tests in this suite
    static void TearDownTestSuite() {
        // Clean up expensive resources
        shared_resource.reset();
    }
    
    // Still called before/after each individual test
    void SetUp() override {
        // Per-test setup
    }
    
    void TearDown() override {
        // Per-test cleanup
    }
    
    static std::unique_ptr<ExpensiveResource> shared_resource;
};
```

#### 参数化测试 (Parameterized Tests)

```cpp
class ParameterizedCalculatorTest : public ::testing::TestWithParam<std::pair<int, int>> {
    // Test fixture setup
};

TEST_P(ParameterizedCalculatorTest, AdditionTest) {
    auto param = GetParam();
    int result = calculator->add(param.first, param.second);
    EXPECT_EQ(param.first + param.second, result);
}

// Define test parameters
INSTANTIATE_TEST_SUITE_P(
    BasicOperations,
    ParameterizedCalculatorTest,
    ::testing::Values(
        std::make_pair(1, 2),
        std::make_pair(5, 3),
        std::make_pair(-2, 4)
    )
);
```

## 测试运行选项 (Test Running Options)

### 使用测试运行脚本 (Using Test Runner Script)

```bash
# Run all tests
./run_tests.sh

# Run with verbose output
./run_tests.sh -v

# Run specific test categories
./run_tests.sh --basic     # Basic assertion tests only
./run_tests.sh --mock      # Mock tests only
./run_tests.sh --fixture   # Fixture tests only

# Run tests matching pattern
./run_tests.sh -f "*Mock*"

# Generate XML report
./run_tests.sh -x

# Generate JSON report
./run_tests.sh -j

# Repeat tests multiple times
./run_tests.sh -r 5

# Shuffle test execution order
./run_tests.sh -s

# List all available tests
./run_tests.sh -l
```

### 直接使用 gtest 选项 (Direct gtest Options)

```bash
# Run specific tests
./build/bin/sample_tests --gtest_filter="BasicAssertionsTest.*"

# Run with verbose output
./build/bin/sample_tests --gtest_verbose

# Generate XML output
./build/bin/sample_tests --gtest_output=xml:test_results.xml

# Repeat tests
./build/bin/sample_tests --gtest_repeat=3

# Shuffle tests
./build/bin/sample_tests --gtest_shuffle

# List all tests
./build/bin/sample_tests --gtest_list_tests
```

## 重要注意事项 (Important Notes)

### 1. MOCK_METHOD 宏注意事项
- **必须使用括号**：参数列表必须用括号包围，即使没有参数也要使用 `()`
- **const 方法**：需要在第四个参数位置添加 `(const)`
- **引用参数**：正确使用引用语法，如 `(std::vector<std::string>&)`
- **重载方法**：使用不同的 mock 方法名或具体的签名

### 2. 测试装置最佳实践
- **SetUp/TearDown**：每个测试前后调用，用于测试隔离
- **SetUpTestSuite/TearDownTestSuite**：整个测试套件前后调用一次，用于昂贵的资源管理
- **必须是静态方法**：测试套件级别的方法必须声明为 `static`
- **资源管理**：确保在 TearDown 中正确清理资源

### 3. 断言选择指南
- **EXPECT_* vs ASSERT_***：EXPECT 继续执行，ASSERT 失败时停止
- **浮点数比较**：使用 `EXPECT_NEAR` 而不是 `EXPECT_EQ`
- **字符串比较**：使用 `EXPECT_STREQ` 或 `EXPECT_EQ`
- **异常测试**：使用 `EXPECT_THROW`、`EXPECT_NO_THROW`、`EXPECT_ANY_THROW`

### 4. Mock 测试最佳实践
- **StrictMock**：所有未预期的调用都会失败
- **NiceMock**：忽略未预期的调用
- **默认行为**：使用 `ON_CALL` 设置默认行为
- **调用顺序**：使用 `InSequence` 强制调用顺序

## 扩展学习 (Further Learning)

- [Google Test Documentation](https://google.github.io/googletest/)
- [Google Mock Documentation](https://google.github.io/googletest/gmock_for_dummies.html)
- [CMake Documentation](https://cmake.org/documentation/)

## 许可证 (License)

This project is for educational purposes. Google Test is licensed under the BSD 3-Clause License.
