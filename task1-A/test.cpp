#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "string_functions.hpp"
#include <random>

TEST_CASE("SimpleTests") {
    REQUIRE(FindSubstring("a", std::istringstream("aaa")) == 
        std::vector<int>({0, 1, 2}));
    REQUIRE(FindSubstring("aba", std::istringstream("abababa")) == 
        std::vector<int>({0, 2, 4}));
    REQUIRE(FindSubstring("qwerty", std::istringstream("qqqwerty")) == 
        std::vector<int>({2}));
    REQUIRE(FindSubstring("b", std::istringstream("aaa")) == 
        std::vector<int>());
    REQUIRE(FindSubstring("a", std::istringstream("")) == 
        std::vector<int>());
}

TEST_CASE("RandomTests") {
    const int testCount = 100;
    std::minstd_rand rand;
    rand.seed(0);
    for (int i = 0; i < testCount; ++i) {
        const int textLength = 300000;
        std::string text(textLength, 0);
        std::generate_n(text.begin(), textLength, 
            [&rand]() { return "abcdefghijklmnopqrstuvwxyz"[rand() % 26]; });
        REQUIRE(FindSubstring(text.substr(0, 3), std::istringstream(text)) == 
            FindSubstringNaive(text.substr(0, 3), std::istringstream(text)));
    }
}