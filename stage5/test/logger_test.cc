#include "lib/logger.h"

#include <gtest/gtest.h>

// Basic tests to verify the logger compiles and runs
// (Output verification would need stdout capture, kept simple here)

TEST(LoggerTest, NoThrowOnLog) {
    EXPECT_NO_THROW(logger::debug("test debug"));
    EXPECT_NO_THROW(logger::info("test info"));
    EXPECT_NO_THROW(logger::warning("test warning"));
    EXPECT_NO_THROW(logger::error("test error"));
}

TEST(LoggerTest, NoThrowOnLogLevels) {
    EXPECT_NO_THROW(logger::log(logger::Level::DEBUG, "msg"));
    EXPECT_NO_THROW(logger::log(logger::Level::INFO, "msg"));
    EXPECT_NO_THROW(logger::log(logger::Level::WARNING, "msg"));
    EXPECT_NO_THROW(logger::log(logger::Level::ERROR, "msg"));
}
