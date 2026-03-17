#include "lib/platform.h"

#include <gtest/gtest.h>

TEST(PlatformTest, OsNameNotEmpty) {
    std::string os = platform::get_os_name();
    EXPECT_FALSE(os.empty());
    // Should be one of the known OS names
    EXPECT_TRUE(os == "Windows" || os == "Linux" || os == "macOS" || os == "Unknown OS");
}

TEST(PlatformTest, BuildModeNotEmpty) {
    std::string mode = platform::get_build_mode();
    EXPECT_FALSE(mode.empty());
    EXPECT_TRUE(mode == "Debug" || mode == "Release (optimized)");
}

TEST(PlatformTest, PlatformInfoContainsOsAndBuild) {
    std::string info = platform::get_platform_info();
    EXPECT_NE(info.find("OS:"), std::string::npos);
    EXPECT_NE(info.find("Build:"), std::string::npos);
}
