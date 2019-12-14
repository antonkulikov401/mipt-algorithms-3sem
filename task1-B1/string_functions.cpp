#include "string_functions.hpp"
#include <unordered_set>

std::unordered_set<char> GetProhibitedCharacters(
        const std::vector<size_t>& prefixFunction, 
        const std::string& str,
        size_t index) {
    std::unordered_set<char> prohibitedCharacters;
    while (index >= 1) {
        prohibitedCharacters.insert(str[prefixFunction[index - 1]]);
        index = prefixFunction[index - 1];
    }
    return prohibitedCharacters;
}

std::string GetStringByPrefixFunction(
        const std::vector<size_t>& prefixFunction,
        size_t alphabetSize, char firstSymbol) {
    std::string result;
    for (size_t i = 0; i < prefixFunction.size(); ++i) {
        if (prefixFunction[i] != 0)
            result += result[prefixFunction[i] - 1];
        else {
            auto prohibitedCharacters = 
                GetProhibitedCharacters(prefixFunction, result, i);
            for (char ch = firstSymbol; ch < firstSymbol + alphabetSize; 
                    ++ch) {
                if (!prohibitedCharacters.count(ch)) {
                    result += ch;
                    break;
                }
            }
        }
    }
    return result;
}
