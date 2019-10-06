#define CATCH_CONFIG_MAIN
#include "../unit-testing/catch.hpp"
#include "string_functions.hpp"

TEST_CASE("SimpleTests") {
    REQUIRE(GetStringByPrefixFunction(
        std::vector<size_t>({0, 1, 2, 3, 0})) == "aaaab");
    REQUIRE(GetStringByPrefixFunction(
        std::vector<size_t>({0})) == "a");
    REQUIRE(GetStringByPrefixFunction(
        std::vector<size_t>({0, 0, 1, 0, 1, 2, 3, 0})) == "abacabad");
    REQUIRE(GetStringByPrefixFunction(
        std::vector<size_t>({0, 0, 1, 2, 3, 4, 0, 1})) == "abababba");
}
