#pragma once
#include <vector>
#include <string>
#include <iterator>
#include <type_traits>

std::vector<int> PrefixFunction(const std::string& str) {
    std::vector<int> result(str.length(), 0);
    for (int i = 1; i < str.length(); ++i) {
        int j = result[i - 1];
        while (j > 0 && str[i] != str[j]) j = result[j - 1];
        result[i] = (str[i] == str[j]) ? j + 1 : j;
    }
    return result;
}

template<class IStream, class OStream>
std::enable_if_t<std::is_base_of_v<std::istream_iterator<char>,
    std::decay_t<IStream>>, std::enable_if_t<
    std::is_base_of_v<std::ostream_iterator<int>,
    std::decay_t<OStream>>, void>>
FindSubstring(std::string pattern, IStream&& input, OStream&& output) {
    pattern += "#";
    const std::vector<int> prefixFunction = PrefixFunction(pattern);
    char symbol;
    int currPrefixValue = 0;
    int currIndex = 0;
    while (input != std::istream_iterator<char>()) {
        symbol = *input;
        int j = currPrefixValue;
        while (j > 0 && pattern[j] != symbol) j = prefixFunction[j - 1];
        currPrefixValue = (pattern[j] == symbol) ? j + 1 : j;
        if (currPrefixValue == pattern.length() - 1)
            *output = currIndex - pattern.length() + 2;
        ++currIndex;
        ++input;
    }
}

template<class IStream, class OStream>
std::enable_if_t<std::is_base_of_v<std::istream_iterator<char>,
    std::decay_t<IStream>>, std::enable_if_t<
    std::is_base_of_v<std::ostream_iterator<int>,
    std::decay_t<OStream>>, void>>
FindSubstringNaive(std::string pattern, IStream&& input, OStream&& output) {
    std::string currentInput;
    char symbol;
    int currIndex = 0;
    while (input != std::istream_iterator<char>()) {
        symbol = *input;
        currentInput += symbol;
        if (currentInput.length() > pattern.length())
            currentInput.erase(0, 1);
        if (currentInput == pattern)
            *output = currIndex - pattern.length() + 1;
        ++currIndex;
        ++input;
    }
}
