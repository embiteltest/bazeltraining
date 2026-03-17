#ifndef LIB_LOGGER_H_
#define LIB_LOGGER_H_

#include <string>

namespace logger {

enum class Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

// Log a message at the given level
void log(Level level, const std::string& message);

// Convenience functions
void debug(const std::string& message);
void info(const std::string& message);
void warning(const std::string& message);
void error(const std::string& message);

}  // namespace logger

#endif  // LIB_LOGGER_H_
