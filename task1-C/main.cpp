#include <iostream>
#include "string_functions.hpp"

int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;
    auto indices = FindPattern(pattern, text);
    for (int i : indices)
        std::cout << i << ' ';
    return 0;
}