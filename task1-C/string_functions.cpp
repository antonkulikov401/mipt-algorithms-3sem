#include "string_functions.hpp"
#include "trie.hpp"
#include <unordered_map>
#include <algorithm>
#include <string_view>

std::vector<std::pair<std::string_view, std::vector<size_t>>>
GetSubpatterns(std::string& pattern) {
    if (pattern[pattern.size() - 1] != '?')
        pattern += '?';
    std::unordered_map<std::string_view, std::vector<size_t>> subpatterns;
    size_t subpatternBegin = 0;
    size_t index = 0;
    for (size_t subpatternEnd = 0; subpatternEnd < pattern.size();
            ++subpatternEnd) {
        if (pattern[subpatternEnd] == '?') {
            if (pattern[subpatternBegin] != '?')
                subpatterns[std::string_view(&pattern[subpatternBegin],
                    subpatternEnd - subpatternBegin)].push_back(index - 1);
            subpatternBegin = subpatternEnd + 1;
        }
        ++index;
    }
    return std::vector<std::pair<std::string_view,
        std::vector<size_t>>>(subpatterns.begin(), subpatterns.end());
}

std::vector<size_t> FindPattern(const std::string& _pattern,
        const std::string& text) {
    std::string pattern = _pattern;
    std::vector<size_t> indices;
    auto subpatterns = GetSubpatterns(pattern);
    std::vector<std::string_view> subpatternStrings;
    std::transform(subpatterns.begin(), subpatterns.end(),
        std::back_inserter(subpatternStrings),
        [](const auto& p) { return p.first; });
    Trie trie(subpatternStrings);
    
    std::vector<size_t> patternEntries(text.size(), 0);
    for (size_t i = 0; i < text.size(); ++i) {
        trie.NextState(text[i]);
        auto foundPatterns = trie.GetPatternIndices();
        for (int foundPattern : foundPatterns)
            for (int j : subpatterns[foundPattern].second)
                if (i >= j) ++patternEntries[i - j];
    }

    size_t numOfSubpatterns = 0;
    for (const auto& subpattern : subpatterns)
        numOfSubpatterns += subpattern.second.size();

    for (size_t i = 0; i < text.size(); ++i)
        if (patternEntries[i] == numOfSubpatterns &&
            (i + _pattern.size() <= text.size()))
            indices.push_back(i);
    return indices;
}
