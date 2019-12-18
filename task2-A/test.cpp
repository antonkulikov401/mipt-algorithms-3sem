#define CATCH_CONFIG_MAIN
#include "../unit-testing/catch.hpp"
#include "string_functions.hpp"
#include <algorithm>
#include <unordered_set>

class RandomStringGenerator {
public:
    RandomStringGenerator(size_t maxStrLength);
    std::string Next();

private:
    const size_t maxStrLength;
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<size_t> dist;
    std::minstd_rand rand;
} stringGenerator(100);

RandomStringGenerator::RandomStringGenerator(size_t maxStrLength) :
        maxStrLength(maxStrLength), mt(rd()), dist(1, maxStrLength) {
    rand.seed(0);
}

std::string RandomStringGenerator::Next() {
    size_t strLength = dist(mt);
    std::string str(strLength, 0);
    std::generate_n(str.begin(), strLength, [this]() {
        return "abcdefghijklmnopqrstuvwxyz"[this->rand() % 26];
    });
    return str;
}

std::vector<size_t> BuildSuffixArrayNaive(const std::string& str) {
    std::vector<size_t> suffixArray(str.size());
    std::vector<std::pair<std::string, size_t>> suffixes(str.size());
    for (size_t i = 0; i < str.size(); ++i)
        suffixes[i] = std::make_pair(str.substr(i), i);
    std::sort(suffixes.begin(), suffixes.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
    });
    for (size_t i = 0; i < str.size(); ++i)
        suffixArray[i] = suffixes[i].second;
    return suffixArray;
}

size_t FindNumberOfUniqueSubstringsNaive(const std::string& str) {
    std::unordered_set<std::string> substrings;
    for (size_t i = 0; i < str.size(); ++i)
        for (size_t len = 1; len <= str.size() - i; ++len)
            substrings.insert(str.substr(i, len));
    return substrings.size();
}

TEST_CASE("Suffix array tests") {
    constexpr size_t testCount = 10000;
    for (size_t i = 0; i < testCount; ++i) {
        auto str = stringGenerator.Next();
        REQUIRE(BuildSuffixArray(str) == BuildSuffixArrayNaive(str));
    }
}

TEST_CASE("Number of unique substrings tests") {
    constexpr size_t testCount = 10000;
    for (size_t i = 0; i < testCount; ++i) {
        auto str = stringGenerator.Next();
        REQUIRE(FindNumberOfUniqueSubstrings(str) ==
            FindNumberOfUniqueSubstringsNaive(str));
    }
}
