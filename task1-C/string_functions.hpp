#pragma once
#include <vector>
#include <string>
#include <string_view>

std::vector<size_t> FindPattern(std::string_view pattern,
    const std::string& text);

std::vector<std::string> Split(const std::string& str, char delimeter);
