#include "lib/logger.h"

#include <ctime>
#include <iostream>

namespace logger {

static std::string level_to_string(Level level) {
    switch (level) {
        case Level::DEBUG:   return "DEBUG";
        case Level::INFO:    return "INFO";
        case Level::WARNING: return "WARN";
        case Level::ERROR:   return "ERROR";
        default:             return "UNKNOWN";
    }
}

static std::string get_timestamp() {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void log(Level level, const std::string& message) {
#ifdef ENABLE_VERBOSE_LOGGING
    std::cout << "[" << get_timestamp() << "] "
              << "[" << level_to_string(level) << "] "
              << message << std::endl;
#else
    // In non-verbose mode, only log WARNING and above
    if (level >= Level::WARNING) {
        std::cout << "[" << level_to_string(level) << "] " << message << std::endl;
    }
#endif
}

void debug(const std::string& message)   { log(Level::DEBUG, message); }
void info(const std::string& message)    { log(Level::INFO, message); }
void warning(const std::string& message) { log(Level::WARNING, message); }
void error(const std::string& message)   { log(Level::ERROR, message); }

}  // namespace logger
