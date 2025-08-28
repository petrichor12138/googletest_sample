#include "database_interface.h"

DatabaseService::DatabaseService(std::shared_ptr<DatabaseInterface> db) 
    : database_(db) {
}

bool DatabaseService::initializeConnection(const std::string& connectionString) {
    if (!database_) {
        return false;
    }
    
    bool connected = database_->connect(connectionString);
    if (connected) {
        initialized_ = true;
    }
    return connected;
}

bool DatabaseService::createUser(const std::string& name, int age) {
    if (!initialized_ || !database_->isConnected()) {
        return false;
    }
    
    return database_->insertUser(name, age);
}

std::string DatabaseService::getUserInfo(int userId) {
    if (!initialized_ || !database_->isConnected()) {
        return "";
    }
    
    std::string name = database_->getUserName(userId);
    int age = database_->getUserAge(userId);
    
    if (name.empty()) {
        return "";
    }
    
    return "Name: " + name + ", Age: " + std::to_string(age);
}

bool DatabaseService::removeUser(int userId) {
    if (!initialized_ || !database_->isConnected()) {
        return false;
    }
    
    return database_->deleteUser(userId);
}

int DatabaseService::getTotalUsers() {
    if (!initialized_ || !database_->isConnected()) {
        return -1;
    }
    
    return database_->getUserCount();
}