#pragma once
#include <vector>
#include <string>

std::string GetStringByPrefixFunction(
    const std::vector<size_t>& prefixFunction, 
    size_t alphabetSize = 26, char firstSymbol = 'a');
