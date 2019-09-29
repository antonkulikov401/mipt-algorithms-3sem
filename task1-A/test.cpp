#define CATCH_CONFIG_MAIN
#include "../unit-testing/catch.hpp"
#include "string_functions.hpp"
#include <random>
#include <tuple>


TEST_CASE("SimpleTests") {
    std::vector<std::tuple<std::string, std::string,
            std::string>> tests = {
        { "a", "aaa", "0 1 2 " },
        { "aba", "abababa", "0 2 4 " },
        { "qwerty", "qqqwerty", "2 " },
        { "b", "aaa", "" },
        { "a", "", "" }
    };

    for (auto& test : tests) {
        auto textStream = std::istringstream(std::get<1>(test));
        auto outputStream = std::ostringstream();
        FindSubstring(std::get<0>(test), 
            std::istream_iterator<char>(textStream),
            std::ostream_iterator<int>(outputStream, " "));
        REQUIRE(outputStream.str() == std::get<2>(test));
    }
}

TEST_CASE("RandomTests") {
    constexpr int testCount = 100;
    constexpr int textLength = 300000;
    std::minstd_rand rand;
    rand.seed(0);
    for (int i = 0; i < testCount; ++i) {
        std::string text(textLength, 0);
        std::generate_n(text.begin(), textLength,
            [&rand]() { return "abcdefghijklmnopqrstuvwxyz"[rand() % 26]; });
        auto firstTextStream = std::istringstream(text);
        auto secondTextStream = std::istringstream(text);
        auto firstOutputStream = std::ostringstream();
        auto secondOutputStream = std::ostringstream();
        FindSubstring(text.substr(0, 3),
            std::istream_iterator<char>(firstTextStream),
            std::ostream_iterator<int>(firstOutputStream, " "));
        FindSubstringNaive(text.substr(0, 3),
            std::istream_iterator<char>(secondTextStream),
            std::ostream_iterator<int>(secondOutputStream, " "));
        REQUIRE(firstOutputStream.str() == secondOutputStream.str());
    }
}
