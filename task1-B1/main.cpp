#include <iostream>
#include <vector>
#include "string_functions.hpp"

int main() {
    std::vector<size_t> prefixFunction;
    size_t input = 0;
    while (std::cin >> input)
        prefixFunction.push_back(input);
    std::cout << GetStringByPrefixFunction(prefixFunction) << "\n";
    return 0;
}
