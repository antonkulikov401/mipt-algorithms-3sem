#include <iostream>
#include "string_functions.hpp"

int main() {
    std::string pattern;
    std::cin >> pattern;
    std::cin.ignore();
    std::vector<int> indices = FindSubstring(pattern, std::cin);
    for (int i: indices) {
        std::cout << i << ' ';
    }
    return 0;
}