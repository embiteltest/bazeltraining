#include "lib/calculator.h"

#include <gtest/gtest.h>
#include <stdexcept>

// Test suite for the calculator library

TEST(CalculatorTest, Add) {
    EXPECT_EQ(calc::add(2, 3), 5);
    EXPECT_EQ(calc::add(-1, 1), 0);
    EXPECT_EQ(calc::add(0, 0), 0);
    EXPECT_EQ(calc::add(-5, -3), -8);
}

TEST(CalculatorTest, Subtract) {
    EXPECT_EQ(calc::subtract(5, 3), 2);
    EXPECT_EQ(calc::subtract(3, 5), -2);
    EXPECT_EQ(calc::subtract(0, 0), 0);
}

TEST(CalculatorTest, Multiply) {
    EXPECT_EQ(calc::multiply(3, 4), 12);
    EXPECT_EQ(calc::multiply(-2, 3), -6);
    EXPECT_EQ(calc::multiply(0, 100), 0);
}

TEST(CalculatorTest, Divide) {
    EXPECT_DOUBLE_EQ(calc::divide(10, 2), 5.0);
    EXPECT_DOUBLE_EQ(calc::divide(7, 2), 3.5);
    EXPECT_DOUBLE_EQ(calc::divide(-6, 3), -2.0);
}

TEST(CalculatorTest, DivideByZeroThrows) {
    EXPECT_THROW(calc::divide(1, 0), std::invalid_argument);
}

TEST(CalculatorTest, Factorial) {
    EXPECT_EQ(calc::factorial(0), 1);
    EXPECT_EQ(calc::factorial(1), 1);
    EXPECT_EQ(calc::factorial(5), 120);
    EXPECT_EQ(calc::factorial(10), 3628800);
}

TEST(CalculatorTest, FactorialNegativeThrows) {
    EXPECT_THROW(calc::factorial(-1), std::invalid_argument);
}
