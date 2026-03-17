#include "lib/calculator.h"
#include "lib/greeter.h"

#include <iostream>

int main(int argc, char** argv) {
    std::string name = "world";
    if (argc > 1) {
        name = argv[1];
    }

    std::cout << greeter::greet(name) << std::endl;
    std::cout << greeter::greet_formal(name, "Dr.") << std::endl;

    std::cout << "\n--- Calculator Demo ---" << std::endl;
    std::cout << "5 + 3 = " << calc::add(5, 3) << std::endl;
    std::cout << "5 - 3 = " << calc::subtract(5, 3) << std::endl;
    std::cout << "5 * 3 = " << calc::multiply(5, 3) << std::endl;
    std::cout << "10 / 3 = " << calc::divide(10, 3) << std::endl;
    std::cout << "5! = " << calc::factorial(5) << std::endl;

    return 0;
}
