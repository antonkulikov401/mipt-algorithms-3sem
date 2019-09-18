#pragma once
#include <vector>
#include <string>

std::vector<int> PrefixFunction(const std::string& str);

std::vector<int> FindSubstring(std::string pattern, std::istream& input);
std::vector<int> FindSubstring(std::string pattern, std::istream&& input);

std::vector<int> FindSubstringNaive(std::string pattern, std::istream& input);
std::vector<int> FindSubstringNaive(std::string pattern, std::istream&& input);
