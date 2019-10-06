#pragma once
#include <vector>
#include <string>
#include <iterator>
#include <type_traits>

size_t GetNextPrefixValue(const std::string& str,
        const std::vector<size_t> prefixFunction, char currSymbol,
        size_t prevPrefixValue) {
    size_t j = prevPrefixValue;
    while (j > 0 && currSymbol != str[j]) j = prefixFunction[j - 1];
    return currSymbol == str[j] ? j + 1 : j;
}

std::vector<size_t> PrefixFunction(const std::string& str) {
    std::vector<size_t> result(str.length(), 0);
    for (size_t i = 1; i < str.length(); ++i)
        result[i] = GetNextPrefixValue(str, result, str[i], result[i - 1]);
    return result;
}

template<class IStream, class OStream>
void FindSubstring(std::string pattern, IStream inIteratorBegin,
        IStream inIteratorEnd, OStream outIterator) {
    pattern += "#";
    const std::vector<size_t> prefixFunction = PrefixFunction(pattern);
    size_t currPrefixValue = 0;
    size_t currIndex = 0;
    while (inIteratorBegin != inIteratorEnd) {
        char symbol = *inIteratorBegin;
        currPrefixValue = GetNextPrefixValue(pattern, prefixFunction,
            symbol, currPrefixValue);
        if (currPrefixValue == pattern.length() - 1)
            *outIterator = currIndex - pattern.length() + 2;
        ++currIndex;
        ++inIteratorBegin;
    }
}
