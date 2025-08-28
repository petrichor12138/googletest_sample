#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "database_interface.h"
#include <memory>

/**
 * Google Mock Test Suite
 * This file demonstrates various aspects of Google Mock framework
 * including MOCK_METHOD usage, expectations, and common patterns
 */

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::StrictMock;
using ::testing::NiceMock;

// ============================================================================
// MOCK CLASS DEFINITION
// ============================================================================

/**
 * Mock implementation of DatabaseInterface
 * 
 * IMPORTANT NOTES ABOUT MOCK_METHOD MACRO:
 * 
 * 1. PARENTHESES USAGE:
 *    - For methods with NO parameters: MOCK_METHOD(ReturnType, MethodName, ())
 *    - For methods with parameters: MOCK_METHOD(ReturnType, MethodName, (ParamType1, ParamType2))
 *    - ALWAYS use parentheses around the parameter list, even if empty
 * 
 * 2. CONST METHODS:
 *    - Add 'const' as the fourth parameter: MOCK_METHOD(ReturnType, MethodName, (), (const))
 *    - For const methods with parameters: MOCK_METHOD(ReturnType, MethodName, (ParamType), (const))
 * 
 * 3. OVERLOADED METHODS:
 *    - Use different mock method names or MOCK_METHOD with specific signatures
 * 
 * 4. REFERENCE PARAMETERS:
 *    - Use proper reference syntax: MOCK_METHOD(bool, method, (std::vector<std::string>&))
 */
class MockDatabase : public DatabaseInterface {
public:
    // Basic methods - note the parentheses usage
    MOCK_METHOD(bool, connect, (const std::string& connectionString), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
    
    // Methods with different parameter types
    MOCK_METHOD(bool, insertUser, (const std::string& name, int age), (override));
    MOCK_METHOD(std::string, getUserName, (int userId), (override));
    MOCK_METHOD(int, getUserAge, (int userId), (override));
    MOCK_METHOD(bool, updateUser, (int userId, const std::string& name, int age), (override));
    MOCK_METHOD(bool, deleteUser, (int userId), (override));
    
    // Methods returning containers
    MOCK_METHOD(std::vector<std::string>, getAllUserNames, (), (override));
    MOCK_METHOD(int, getUserCount, (), (override));
    
    // Method with reference parameter - IMPORTANT: Note the reference syntax
    MOCK_METHOD(bool, executeQuery, (const std::string& query, std::vector<std::string>& results), (override));
    
    // Const method - note the (const) specifier
    MOCK_METHOD(std::string, getLastError, (), (const, override));
    MOCK_METHOD(void, clearError, (), (override));
};

// ============================================================================
// BASIC MOCK TESTS
// ============================================================================

class MockDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockDb = std::make_shared<MockDatabase>();
        service = std::make_unique<DatabaseService>(mockDb);
    }
    
    void TearDown() override {
        service.reset();
        mockDb.reset();
    }
    
    std::shared_ptr<MockDatabase> mockDb;
    std::unique_ptr<DatabaseService> service;
};

/**
 * Test basic mock expectations and return values
 * Demonstrates EXPECT_CALL with Return actions
 */
TEST_F(MockDatabaseTest, BasicMockExpectations) {
    // Set up expectations for connection
    EXPECT_CALL(*mockDb, connect("test_connection"))
        .WillOnce(Return(true));
    
    EXPECT_CALL(*mockDb, isConnected())
        .WillRepeatedly(Return(true));
    
    // Test the service method
    bool result = service->initializeConnection("test_connection");
    EXPECT_TRUE(result);
}

/**
 * Test mock function call counting
 * Demonstrates Times(), AtLeast(), AtMost(), Exactly()
 */
TEST_F(MockDatabaseTest, MockCallCounting) {
    // Expect connect to be called exactly once
    EXPECT_CALL(*mockDb, connect(_))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    // Expect isConnected to be called at least twice
    EXPECT_CALL(*mockDb, isConnected())
        .Times(AtLeast(2))
        .WillRepeatedly(Return(true));
    
    // Expect insertUser to be called at most 3 times
    EXPECT_CALL(*mockDb, insertUser(_, _))
        .Times(AtMost(3))
        .WillRepeatedly(Return(true));
    
    // Execute operations
    service->initializeConnection("test");
    service->createUser("Alice", 25);
    service->createUser("Bob", 30);
}

/**
 * Test mock with specific parameter matching
 * Demonstrates parameter matchers and specific value expectations
 */
TEST_F(MockDatabaseTest, ParameterMatching) {
    // Set up connection
    EXPECT_CALL(*mockDb, connect(_)).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, isConnected()).WillRepeatedly(Return(true));
    
    // Expect specific parameters
    EXPECT_CALL(*mockDb, insertUser("Alice", 25))
        .WillOnce(Return(true));
    
    // Expect any string but specific age
    EXPECT_CALL(*mockDb, insertUser(_, 30))
        .WillOnce(Return(true));
    
    // Test with specific parameters
    service->initializeConnection("test");
    EXPECT_TRUE(service->createUser("Alice", 25));
    EXPECT_TRUE(service->createUser("Bob", 30));
}

/**
 * Test mock with different return values for different calls
 * Demonstrates WillOnce(), WillRepeatedly(), and multiple return values
 */
TEST_F(MockDatabaseTest, MultipleReturnValues) {
    // Set up connection
    EXPECT_CALL(*mockDb, connect(_)).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, isConnected()).WillRepeatedly(Return(true));
    
    // First call returns "Alice", subsequent calls return "Unknown"
    EXPECT_CALL(*mockDb, getUserName(1))
        .WillOnce(Return("Alice"))
        .WillRepeatedly(Return("Unknown"));
    
    // Age returns different values for different calls
    EXPECT_CALL(*mockDb, getUserAge(1))
        .WillOnce(Return(25))
        .WillOnce(Return(26))
        .WillRepeatedly(Return(0));
    
    service->initializeConnection("test");
    
    // First call
    std::string info1 = service->getUserInfo(1);
    EXPECT_EQ("Name: Alice, Age: 25", info1);
    
    // Second call - different age, same name becomes "Unknown"
    std::string info2 = service->getUserInfo(1);
    EXPECT_EQ("Name: Unknown, Age: 26", info2);
    
    // Third call - both return default values
    std::string info3 = service->getUserInfo(1);
    EXPECT_EQ("Name: Unknown, Age: 0", info3);
}

// ============================================================================
// ADVANCED MOCK FEATURES
// ============================================================================

/**
 * Test mock with reference parameters
 * Demonstrates SetArgReferee and DoAll actions
 */
TEST_F(MockDatabaseTest, ReferenceParameters) {
    // Set up connection
    EXPECT_CALL(*mockDb, connect(_)).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, isConnected()).WillRepeatedly(Return(true));
    
    // Mock method that modifies reference parameter
    EXPECT_CALL(*mockDb, executeQuery("SELECT * FROM users", _))
        .WillOnce(DoAll(
            SetArgReferee<1>(std::vector<std::string>{"Alice", "Bob", "Charlie"}),
            Return(true)
        ));
    
    service->initializeConnection("test");
    
    // Test the reference parameter modification
    std::vector<std::string> results;
    bool success = mockDb->executeQuery("SELECT * FROM users", results);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(3, results.size());
    EXPECT_EQ("Alice", results[0]);
    EXPECT_EQ("Bob", results[1]);
    EXPECT_EQ("Charlie", results[2]);
}

/**
 * Test mock with custom actions using Invoke
 * Demonstrates how to use custom functions as mock actions
 */
TEST_F(MockDatabaseTest, CustomActions) {
    // Custom function to simulate database behavior
    auto customConnect = [](const std::string& connectionString) -> bool {
        return connectionString.find("valid") != std::string::npos;
    };
    
    // Use Invoke to call custom function
    EXPECT_CALL(*mockDb, connect(_))
        .WillRepeatedly(Invoke(customConnect));
    
    // Test with valid connection string
    EXPECT_TRUE(mockDb->connect("valid_connection"));
    
    // Test with invalid connection string - note: "invalid_connection" contains "valid" so it returns true
    // Let's use a truly invalid string
    EXPECT_FALSE(mockDb->connect("bad_connection"));
}

/**
 * Test ordered expectations using InSequence
 * Demonstrates how to enforce call order
 */
TEST_F(MockDatabaseTest, OrderedExpectations) {
    InSequence seq;
    
    // These calls must happen in this exact order
    EXPECT_CALL(*mockDb, connect(_)).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, isConnected()).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, insertUser("Alice", 25)).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, isConnected()).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, getUserCount()).WillOnce(Return(1));
    EXPECT_CALL(*mockDb, disconnect());
    
    // Execute in the expected order
    service->initializeConnection("test");
    service->createUser("Alice", 25);
    int count = service->getTotalUsers();
    mockDb->disconnect();
    
    EXPECT_EQ(1, count);
}

// ============================================================================
// MOCK TYPES: STRICT, NICE, AND DEFAULT
// ============================================================================

/**
 * Test StrictMock - fails on any unexpected calls
 * Use StrictMock when you want to ensure no unexpected method calls
 */
TEST(MockTypesTest, StrictMockTest) {
    StrictMock<MockDatabase> strictMock;
    
    // Only set expectations for calls we expect
    EXPECT_CALL(strictMock, connect("test")).WillOnce(Return(true));
    
    // This call is expected and will pass
    EXPECT_TRUE(strictMock.connect("test"));
    
    // Any unexpected call would cause the test to fail
    // strictMock.disconnect(); // This would fail the test
}

/**
 * Test NiceMock - ignores unexpected calls
 * Use NiceMock when you don't care about unexpected method calls
 */
TEST(MockTypesTest, NiceMockTest) {
    NiceMock<MockDatabase> niceMock;
    
    // Set expectation for one method
    EXPECT_CALL(niceMock, connect("test")).WillOnce(Return(true));
    
    // Expected call
    EXPECT_TRUE(niceMock.connect("test"));
    
    // Unexpected calls are ignored (no test failure)
    niceMock.disconnect();
    niceMock.clearError();
}

// ============================================================================
// MOCK BEST PRACTICES AND COMMON PITFALLS
// ============================================================================

/**
 * Demonstrates common mock patterns and best practices
 */
class MockBestPracticesTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockDb = std::make_shared<MockDatabase>();
    }
    
    std::shared_ptr<MockDatabase> mockDb;
};

/**
 * Test demonstrating proper cleanup and expectation verification
 */
TEST_F(MockBestPracticesTest, ProperCleanup) {
    {
        // Create service in a scope to test proper cleanup
        DatabaseService service(mockDb);
        
        EXPECT_CALL(*mockDb, connect(_)).WillOnce(Return(true));
        EXPECT_CALL(*mockDb, isConnected()).WillRepeatedly(Return(true));
        EXPECT_CALL(*mockDb, disconnect()).Times(1);
        
        service.initializeConnection("test");
        mockDb->disconnect();
    }
    // Service is destroyed here, expectations should be satisfied
}

/**
 * Test demonstrating how to handle methods that should never be called
 */
TEST_F(MockBestPracticesTest, MethodsShouldNeverBeCalled) {
    // Use Times(0) to ensure a method is never called
    EXPECT_CALL(*mockDb, deleteUser(_)).Times(0);
    EXPECT_CALL(*mockDb, clearError()).Times(0);
    
    // Set up normal expectations
    EXPECT_CALL(*mockDb, connect(_)).WillOnce(Return(true));
    EXPECT_CALL(*mockDb, isConnected()).WillRepeatedly(Return(true));
    
    DatabaseService service(mockDb);
    service.initializeConnection("test");
    
    // deleteUser and clearError should never be called
    // If they are called, the test will fail
}

/**
 * Test demonstrating default actions for mocks
 */
TEST_F(MockBestPracticesTest, DefaultActions) {
    // Set default actions that will be used when no specific expectation matches
    ON_CALL(*mockDb, connect(_)).WillByDefault(Return(false));
    ON_CALL(*mockDb, isConnected()).WillByDefault(Return(false));
    ON_CALL(*mockDb, getUserName(_)).WillByDefault(Return("DefaultUser"));
    
    // These calls will use the default actions
    EXPECT_FALSE(mockDb->connect("any_string"));
    EXPECT_FALSE(mockDb->isConnected());
    EXPECT_EQ("DefaultUser", mockDb->getUserName(999));
}