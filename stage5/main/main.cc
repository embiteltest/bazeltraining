#include "lib/logger.h"
#include "lib/platform.h"

#include <iostream>

int main(int argc, char** argv) {
    std::cout << "=== Bazel Stage 5: Configurable Builds ===" << std::endl;
    std::cout << platform::get_platform_info() << std::endl;
    std::cout << std::endl;

    logger::debug("This is a debug message (only visible in verbose mode)");
    logger::info("Application started (only visible in verbose mode)");
    logger::warning("This is a warning (always visible)");
    logger::error("This is an error (always visible)");

    std::cout << "\n--- Build Configuration ---" << std::endl;
    std::cout << "Platform: " << platform::get_os_name() << std::endl;
    std::cout << "Build mode: " << platform::get_build_mode() << std::endl;

#ifdef ENABLE_VERBOSE_LOGGING
    std::cout << "Verbose logging: ENABLED" << std::endl;
#else
    std::cout << "Verbose logging: DISABLED" << std::endl;
#endif

    return 0;
}
