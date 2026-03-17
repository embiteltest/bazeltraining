#include "lib/greeter.h"

namespace greeter {

std::string greet(const std::string& name) {
    if (name.empty()) {
        return "Hello, stranger!";
    }
    return "Hello, " + name + "!";
}

std::string greet_formal(const std::string& name, const std::string& title) {
    if (name.empty()) {
        return "Good day!";
    }
    if (title.empty()) {
        return "Good day, " + name + "!";
    }
    return "Good day, " + title + " " + name + "!";
}

}  // namespace greeter
