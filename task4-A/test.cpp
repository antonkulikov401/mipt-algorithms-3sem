#define CATCH_CONFIG_MAIN
#include "../unit-testing/catch.hpp"
#include "biginteger.h"
#include <random>
#include <tuple>
#include <cstdio>
#include <string_view>
#include <functional>

std::string exec(std::string cmd) {
    std::string result;
    std::array<char, 128> buff;
    FILE* pipe = popen(cmd.data(), "r");
    while (fgets(buff.data(), buff.size(), pipe) != nullptr)
        result += buff.data();
    pclose(pipe);
    return result;
}

std::string GenerateRandomBigInt(std::minstd_rand& rand, size_t maxLen) {
    size_t len = rand() % maxLen + 1;
    std::string number(len, 0);
    std::generate_n(number.begin(), len,
        [&rand]() { return "123456789"[rand() % 9]; });
    return (rand() % 2 ? number : "-" + number);
}

void RandomTests(size_t testCount,
    std::function<void(std::string&&, std::string&&)> requirement,
    size_t maxLen, uint32_t seed = 42) {
    
    std::minstd_rand rand;
    rand.seed(seed);
    for (size_t i = 0; i < testCount; ++i)
        requirement(GenerateRandomBigInt(rand, maxLen),
            GenerateRandomBigInt(rand, maxLen));
}

// TEST_CASE("Basic addition and substraction tests") {
//     REQUIRE((BigInteger("0") + BigInteger("0")).toString() == "0");
//     REQUIRE((BigInteger("0") - BigInteger("0")).toString() == "0");
//     REQUIRE((BigInteger("12") + BigInteger("0")).toString() == "12");
//     REQUIRE((BigInteger("5") + BigInteger("-5")).toString() == "0");
//     REQUIRE((BigInteger("-4") + BigInteger("4")).toString() == "0");
//     REQUIRE((BigInteger("-4") - BigInteger("-4")).toString() == "0");
//     REQUIRE((BigInteger("7") - BigInteger("7")).toString() == "0");
//     REQUIRE((BigInteger("-1") - BigInteger("2")).toString() == "-3");
//     REQUIRE((BigInteger("-1") + BigInteger("10")).toString() == "9");
//     REQUIRE((BigInteger("10") - BigInteger("11")).toString() == "-1");
//     REQUIRE((BigInteger("0") - BigInteger("7")).toString() == "-7");
// }

// TEST_CASE("Construction from string and toString() method") {
//     RandomTests(100, [](std::string&& x, std::string&& y) {
//         REQUIRE(BigInteger(x).toString() == x);
//         REQUIRE(BigInteger(std::string(y)).toString() == y);
//     }, 10000);
// }

// TEST_CASE("Unary Plus and Minus") {
//     RandomTests(100, [](std::string&& x, std::string&& y) {
//         REQUIRE((+BigInteger(x)).toString() ==
//             exec("./test.py unary_plus " + x));
//         REQUIRE((-BigInteger(y)).toString() ==
//             exec("./test.py unary_minus " + y));
//     }, 10000);
// }

// TEST_CASE("Operator<") {
//     RandomTests(500, [](std::string&& x, std::string&& y) {
//         REQUIRE(std::to_string(BigInteger(x) < BigInteger(y)) ==
//             exec("./test.py less " + x + " " + y));
//     }, 10000);
// }

// TEST_CASE("Operator==") {
//     RandomTests(500, [](std::string&& x, std::string&& y) {
//         REQUIRE(std::to_string(BigInteger(x) == BigInteger(y)) ==
//             exec("./test.py eq " + x + " " + y));
//     }, 5);
// }

// TEST_CASE("Addition and substraction") {
//     RandomTests(100, [](std::string&& x, std::string&& y) {
//         REQUIRE((BigInteger(x) + BigInteger(y)).toString() ==
//             exec("./test.py addition " + x + " " + y));
//         REQUIRE((BigInteger(x) - BigInteger(y)).toString() ==
//             exec("./test.py substraction " + x + " " + y));
//     }, 10);
// }

// TEST_CASE("Multiplication") {
//     RandomTests(10, [](std::string&& x, std::string&& y) {
//         REQUIRE((BigInteger(x) * BigInteger(y)).toString() ==
//             exec("./test.py multiplication " + x + " " + y));
//     }, 1000);
// }

// TEST_CASE("Division") {
//     RandomTests(10, [](std::string&& x, std::string&& y) {
//         REQUIRE((BigInteger(x) / BigInteger(y)).toString() ==
//             exec("./test.py division " + x + " " + y));
//     }, 100);
// }

TEST_CASE("Division asymptotic test") {
    std::ofstream output("time_test/time.txt");
    for (size_t i = 3; i < 100; ++i) {
        std::string x, y = "1";
        for (size_t j = 0; j < i; ++j) x.push_back('9');
        for (size_t j = 0; j < i; ++j) x.push_back('0');
        for (size_t j = 0; j < i; ++j) y.push_back('0');
        BigInteger lhs(x), rhs(y);
        auto start = std::chrono::steady_clock::now();
        for (size_t j = 0; j < 5; ++j) lhs / rhs;
        auto end = std::chrono::steady_clock::now();
        auto d = end - start;
        auto time = std::chrono::duration<double, std::milli>(d).count() / 5;
        output << i << " " << time << std::endl;
    }
    output.close();
}
