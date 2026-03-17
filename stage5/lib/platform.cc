#include "lib/platform.h"

namespace platform {

std::string get_os_name() {
#if defined(_WIN32) || defined(_WIN64)
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__)
    return "macOS";
#else
    return "Unknown OS";
#endif
}

std::string get_build_mode() {
#if defined(NDEBUG)
    return "Release (optimized)";
#else
    return "Debug";
#endif
}

std::string get_platform_info() {
    return "OS: " + get_os_name() + " | Build: " + get_build_mode();
}

}  // namespace platform
