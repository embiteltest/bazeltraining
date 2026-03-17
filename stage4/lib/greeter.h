#ifndef LIB_GREETER_H_
#define LIB_GREETER_H_

#include <string>

namespace greeter {

std::string greet(const std::string& name);
std::string greet_formal(const std::string& name, const std::string& title);

}  // namespace greeter

#endif  // LIB_GREETER_H_
