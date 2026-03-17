#include "lib/greeter.h"

#include <gtest/gtest.h>

// Test suite for the greeter library

TEST(GreeterTest, BasicGreeting) {
    EXPECT_EQ(greeter::greet("Alice"), "Hello, Alice!");
    EXPECT_EQ(greeter::greet("Bob"), "Hello, Bob!");
}

TEST(GreeterTest, EmptyNameGreeting) {
    EXPECT_EQ(greeter::greet(""), "Hello, stranger!");
}

TEST(GreeterTest, FormalGreeting) {
    EXPECT_EQ(greeter::greet_formal("Smith", "Dr."), "Good day, Dr. Smith!");
    EXPECT_EQ(greeter::greet_formal("Jones", "Prof."), "Good day, Prof. Jones!");
}

TEST(GreeterTest, FormalGreetingNoTitle) {
    EXPECT_EQ(greeter::greet_formal("Alice", ""), "Good day, Alice!");
}

TEST(GreeterTest, FormalGreetingNoName) {
    EXPECT_EQ(greeter::greet_formal("", "Dr."), "Good day!");
}
