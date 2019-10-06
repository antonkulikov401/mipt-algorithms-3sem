#pragma once
#include <vector>
#include <string>

std::vector<size_t> FindPattern(const std::string& pattern,
    const std::string& text);

std::vector<std::string> Split(const std::string& str, char delimeter);
