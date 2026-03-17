#include "lib/calculator.h"

#include <stdexcept>

namespace calc {

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return static_cast<double>(a) / b;
}

int factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Negative factorial");
    }
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

}  // namespace calc
