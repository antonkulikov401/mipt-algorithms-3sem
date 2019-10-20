#include <iostream>
#include "string_functions.hpp"

int main() {
    std::string input;
    std::cin >> input;
    std::cout << FindNumberOfUniqueSubstrings(input);
    return 0;
}
