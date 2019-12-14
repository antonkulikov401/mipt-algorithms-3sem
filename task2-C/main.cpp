#include <iostream>
#include "string_functions.hpp"

int main() {
    std::string firstString;
    std::string secondString;
    size_t k;
    std::cin >> firstString >> secondString >> k;
    auto substring = FindKthCommonSubstring(firstString, secondString, k);
    std::cout << (substring == "" ? "-1" : substring);
    return 0;
}
