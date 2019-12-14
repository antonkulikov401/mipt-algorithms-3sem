#include <iostream>
#include "string_functions.hpp"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::string pattern;
    std::cin >> pattern;
    FindSubstring(pattern,
        std::istream_iterator<char>(std::cin),
        std::istream_iterator<char>(),
        std::ostream_iterator<size_t>(std::cout, " "));
    return 0;
}
