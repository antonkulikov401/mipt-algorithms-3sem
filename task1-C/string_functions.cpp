#include "string_functions.hpp"
#include "trie.hpp"
#include <unordered_map>
#include <algorithm>

std::vector<std::pair<std::string_view, std::vector<size_t>>>
GetSubpatterns(std::string_view pattern) {
    std::unordered_map<std::string_view, std::vector<size_t>> subpatterns;
    auto curr = pattern.begin();
    auto begin = pattern.begin();
    while (curr != pattern.end()) {
        if (*curr != '?') {
            ++curr;
            continue;
        }
        auto lastChar = curr;
        while (*curr == '?') ++curr;
        subpatterns[std::string_view(begin,
            lastChar - begin)].push_back(begin - pattern.begin());
        begin = curr;
    }
    if (begin != pattern.end())
        subpatterns[std::string_view(begin,
            pattern.end() - begin)].push_back(begin - pattern.begin());
    return std::vector<std::pair<std::string_view,
        std::vector<size_t>>>(subpatterns.begin(), subpatterns.end());
}

std::vector<size_t> FindPattern(std::string_view pattern,
        const std::string& text) {
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
            (i + pattern.size() <= text.size()))
            indices.push_back(i);
    return indices;
}
