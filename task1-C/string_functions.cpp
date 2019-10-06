#include "string_functions.hpp"
#include "trie.hpp"
#include <unordered_map>

std::vector<std::pair<std::string, std::vector<size_t>>>
GetSubpatterns(std::string pattern) {
    if (pattern[pattern.size() - 1] != '?')
        pattern += '?';
    std::unordered_map<std::string, std::vector<size_t>> subpatterns;
    auto subpatternBegin = pattern.begin();
    size_t index = 0;
    for (auto subpatternEnd = pattern.begin(); subpatternEnd != pattern.end();
            ++subpatternEnd) {
        if (*subpatternEnd == '?') {
            if (*subpatternBegin != '?')
                subpatterns[std::string(subpatternBegin,
                    subpatternEnd)].push_back(index - 1);
            subpatternBegin = subpatternEnd + 1;
        }
        ++index;
    }
    return std::vector<std::pair<std::string,
        std::vector<size_t>>>(subpatterns.begin(), subpatterns.end());
}

std::vector<size_t> FindPattern(const std::string& pattern,
        const std::string& text) {
    std::vector<size_t> indices;
    Trie trie;
    auto subpatterns = GetSubpatterns(pattern);

    for (size_t i = 0; i < subpatterns.size(); ++i)
        trie.AddString(subpatterns[i].first, i);
    trie.BuildSuffixLinks();
    trie.BuildDictSuffixLinks();
    std::vector<size_t> patternEntries(text.size(), 0);
    for (size_t i = 0; i < text.size(); ++i) {
        trie.NextState(text[i]);
        auto foundPatterns = trie.GetPatternIndices();
        for (int foundPattern : foundPatterns) {
            for (int j : subpatterns[foundPattern].second)
                if (i >= j) ++patternEntries[i - j];
        }
    }

    size_t numOfSubpatterns = 0;
    for (const auto& subpattern : subpatterns)
        numOfSubpatterns += subpattern.second.size();

    for (size_t i = 0; i < text.size(); ++i)
        if (patternEntries[i] == numOfSubpatterns)
            if (i + pattern.size() <= text.size())
                indices.push_back(i);
    return indices;
}
