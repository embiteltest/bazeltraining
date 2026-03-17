#ifndef LIB_PLATFORM_H_
#define LIB_PLATFORM_H_

#include <string>

namespace platform {

// Returns a string identifying the current OS
std::string get_os_name();

// Returns a debug or release label based on build config
std::string get_build_mode();

// Returns a summary string
std::string get_platform_info();

}  // namespace platform

#endif  // LIB_PLATFORM_H_
