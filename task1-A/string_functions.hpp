#pragma once
#include <vector>
#include <string>
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

template<class T>
std::enable_if_t<std::is_base_of_v<std::istream,
    std::decay_t<T>>, std::vector<int>>
FindSubstring(std::string pattern, T&& input) {
    pattern += "#";
    std::vector<int> indices;
    const std::vector<int> prefixFunction = PrefixFunction(pattern);
    char symbol;
    int currPrefixValue = 0;
    int currIndex = 0;
    while (input.get(symbol)) {
        int j = currPrefixValue;
        while (j > 0 && pattern[j] != symbol) j = prefixFunction[j - 1];
        currPrefixValue = (pattern[j] == symbol) ? j + 1 : j;
        if (currPrefixValue == pattern.length() - 1)
            indices.push_back(currIndex - pattern.length() + 2);
        ++currIndex;
    }
    return indices;
}

template<class T>
std::enable_if_t<std::is_base_of_v<std::istream,
    std::decay_t<T>>, std::vector<int>>
FindSubstringNaive(std::string pattern, T&& input) {
    std::vector<int> indices;
    std::string currentInput;
    char symbol;
    int currIndex = 0;
    while (input.get(symbol)) {
        currentInput += symbol;
        if (currentInput.length() > pattern.length())
            currentInput.erase(0, 1);
        if (currentInput == pattern)
            indices.push_back(currIndex - pattern.length() + 1);
        ++currIndex;
    }
    return indices;
}
