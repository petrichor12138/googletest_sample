#include <iostream>
#include "calculator.h"
#include "database_interface.h"

int main() {
    std::cout << "Google Test Sample Project" << std::endl;
    std::cout << "==========================" << std::endl;
    
    // Demonstrate Calculator functionality
    Calculator calc;
    std::cout << "\nCalculator Demo:" << std::endl;
    std::cout << "5 + 3 = " << calc.add(5, 3) << std::endl;
    std::cout << "10 - 4 = " << calc.subtract(10, 4) << std::endl;
    std::cout << "6 * 7 = " << calc.multiply(6, 7) << std::endl;
    std::cout << "15.0 / 3.0 = " << calc.divide(15.0, 3.0) << std::endl;
    
    std::cout << "\nBoolean operations:" << std::endl;
    std::cout << "Is 5 positive? " << (calc.isPositive(5) ? "Yes" : "No") << std::endl;
    std::cout << "Is 4 even? " << (calc.isEven(4) ? "Yes" : "No") << std::endl;
    
    std::cout << "\nString operations:" << std::endl;
    std::cout << "Concatenate 'Hello' + ' World': " << calc.concatenate("Hello", " World") << std::endl;
    std::cout << "Uppercase 'test': " << calc.toUpperCase("test") << std::endl;
    
    std::cout << "\nTo run tests, use: ./bin/sample_tests" << std::endl;
    std::cout << "To build: mkdir build && cd build && cmake .. && make" << std::endl;
    
    return 0;
}