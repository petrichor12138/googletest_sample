#include "calculator.h"
#include <algorithm>
#include <cctype>
#include <cmath>

int Calculator::add(int a, int b) {
    return a + b;
}

int Calculator::subtract(int a, int b) {
    return a - b;
}

int Calculator::multiply(int a, int b) {
    return a * b;
}

double Calculator::divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

bool Calculator::isPositive(int number) {
    return number > 0;
}

bool Calculator::isEven(int number) {
    return number % 2 == 0;
}

bool Calculator::isEmpty(const std::string& str) {
    return str.empty();
}

std::string Calculator::concatenate(const std::string& str1, const std::string& str2) {
    return str1 + str2;
}

std::string Calculator::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

int Calculator::getLength(const std::string& str) {
    return static_cast<int>(str.length());
}

void Calculator::setValue(double value) {
    stored_value_ = value;
}

double Calculator::getValue() const {
    return stored_value_;
}

long long Calculator::factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial is not defined for negative numbers");
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double Calculator::squareRoot(double x) {
    if (x < 0) {
        throw std::invalid_argument("Square root is not defined for negative numbers");
    }
    return std::sqrt(x);
}