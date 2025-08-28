#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <stdexcept>

/**
 * Calculator class for demonstrating basic assertions in Google Test
 * This class provides basic arithmetic operations and string manipulation
 */
class Calculator {
public:
    Calculator() = default;
    ~Calculator() = default;

    // Basic arithmetic operations
    int add(int a, int b);
    int subtract(int a, int b);
    int multiply(int a, int b);
    double divide(double a, double b);
    
    // Boolean operations for boolean assertion demonstrations
    bool isPositive(int number);
    bool isEven(int number);
    bool isEmpty(const std::string& str);
    
    // String operations for string assertion demonstrations
    std::string concatenate(const std::string& str1, const std::string& str2);
    std::string toUpperCase(const std::string& str);
    int getLength(const std::string& str);
    
    // Value operations
    void setValue(double value);
    double getValue() const;
    
    // Advanced operations
    long long factorial(int n);
    double squareRoot(double x);
    
private:
    double stored_value_ = 0.0;
    int value_ = 0;
};

#endif // CALCULATOR_H