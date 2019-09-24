#include <iostream>
#include <vector>
#include "string_functions.hpp"

int main() {
    std::vector<int> prefixFunction;
    int input = 0;
    while (std::cin >> input)
        prefixFunction.push_back(input);
    std::cout << GetStringByPrefixFunction(prefixFunction);
    return 0;
}