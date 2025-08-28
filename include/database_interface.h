#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include <string>
#include <vector>
#include <memory>

/**
 * Abstract database interface for demonstrating Google Mock
 * This interface will be mocked in tests to simulate database operations
 */
class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;
    
    // Basic CRUD operations
    virtual bool connect(const std::string& connectionString) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // Data operations for mock testing
    virtual bool insertUser(const std::string& name, int age) = 0;
    virtual std::string getUserName(int userId) = 0;
    virtual int getUserAge(int userId) = 0;
    virtual bool updateUser(int userId, const std::string& name, int age) = 0;
    virtual bool deleteUser(int userId) = 0;
    
    // Operations with different parameter types for testing MOCK_METHOD
    virtual std::vector<std::string> getAllUserNames() = 0;
    virtual int getUserCount() = 0;
    virtual bool executeQuery(const std::string& query, std::vector<std::string>& results) = 0;
    
    // Operations with const and non-const versions
    virtual std::string getLastError() const = 0;
    virtual void clearError() = 0;
};

/**
 * Database service class that uses DatabaseInterface
 * This class will be tested using mocked database interface
 */
class DatabaseService {
public:
    explicit DatabaseService(std::shared_ptr<DatabaseInterface> db);
    ~DatabaseService() = default;
    
    // Service operations that will be tested with mocks
    bool initializeConnection(const std::string& connectionString);
    bool createUser(const std::string& name, int age);
    std::string getUserInfo(int userId);
    bool removeUser(int userId);
    int getTotalUsers();
    
private:
    std::shared_ptr<DatabaseInterface> database_;
    bool initialized_ = false;
};

#endif // DATABASE_INTERFACE_H