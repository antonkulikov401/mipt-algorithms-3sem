#pragma once
#include <iostream>
#include <vector>
#include <string>

class BigInteger {
public:
    BigInteger();
    BigInteger(int);
    explicit BigInteger(const std::string&);
    BigInteger(const BigInteger&);
    BigInteger(BigInteger&&) noexcept;
    BigInteger& operator=(const BigInteger&)&;
    BigInteger& operator=(BigInteger&&)& noexcept;
    ~BigInteger() = default;

    BigInteger& operator+=(const BigInteger&);
    BigInteger& operator-=(const BigInteger&);
    BigInteger& operator*=(const BigInteger&);
    BigInteger& operator/=(const BigInteger&);
    BigInteger& operator%=(const BigInteger&);
    BigInteger operator+() const;
    BigInteger operator-() const;

    friend BigInteger operator+(const BigInteger&, const BigInteger&);
    friend BigInteger operator-(const BigInteger&, const BigInteger&);
    friend BigInteger operator*(const BigInteger&, const BigInteger&);
    friend BigInteger operator/(const BigInteger&, const BigInteger&);
    friend BigInteger operator%(const BigInteger&, const BigInteger&);

    BigInteger& operator++();
    BigInteger& operator--();
    BigInteger operator++(int);
    BigInteger operator--(int);

    friend bool operator<(const BigInteger&, const BigInteger&);
    friend bool operator>(const BigInteger&, const BigInteger&);
    friend bool operator<=(const BigInteger&, const BigInteger&);
    friend bool operator>=(const BigInteger&, const BigInteger&);
    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator!=(const BigInteger&, const BigInteger&);

    std::string toString() const;
    friend std::istream& operator>>(std::istream&, BigInteger&);
    friend std::ostream& operator<<(std::ostream&, const BigInteger&);

    explicit operator bool() const;

private:
    const static uint32_t baseExp = 4;
    const static uint32_t base = 10000;
    bool sign;
    std::vector<uint32_t> digits;

    void unsignedAddition(const BigInteger&);
    void unsignedSubstraction(const BigInteger&);
    static BigInteger unsignedMultiplication(
        const BigInteger&, const BigInteger&);
    static bool unsignedComparison(const BigInteger&, const BigInteger&);
    static std::pair<BigInteger, BigInteger> Split(const BigInteger&);
    BigInteger& shift(size_t);
    void truncateZeros();
    static size_t appendZeros(BigInteger&, size_t);
};

BigInteger::BigInteger() : sign(0), digits(1, 0) {}

BigInteger::BigInteger(int value) : sign(value < 0) {
    digits.push_back(std::abs(value) % base);
    if (std::abs(value) / base > 0)
        digits.push_back(std::abs(value) / base);
}

BigInteger::BigInteger(const std::string& value) : sign(0) {
    if (value.size() == 0) {
        digits = std::vector<uint32_t>(1, 0);
        return;
    }
    if (value[0] == '-') sign = 1;
    for (int64_t i = value.size() - 1; i >= sign; i -= baseExp) {
        int64_t from = std::max(i - baseExp + 1, static_cast<int64_t>(sign));
        digits.push_back(std::stoi(value.substr(from, i - from + 1)));
    }
    if (digits.size() == 1 && digits[0] == 0) sign = 0;
}

BigInteger::BigInteger(const BigInteger& other) :
    sign(other.sign), digits(other.digits) {}

BigInteger::BigInteger(BigInteger&& other) noexcept :
    sign(other.sign), digits(std::move(other.digits)) {}

BigInteger& BigInteger::operator=(const BigInteger& other)& {
    if (this == &other) return *this;
    BigInteger copy = other;
    sign = other.sign;
    digits.swap(copy.digits);
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other)& noexcept {
    sign = other.sign;
    digits.swap(other.digits);
    return *this;
}

void BigInteger::unsignedAddition(const BigInteger& other) {
    bool carry = 0;
    size_t len = other.digits.size();
    for (size_t i = 0; i < std::max(digits.size(), len) || carry; ++i) {
        if (i >= digits.size()) digits.push_back(0);
        uint32_t sum = digits[i] + carry + (i < len ? other.digits[i] : 0);
        if (i < digits.size()) digits[i] = sum % base;
        else digits.push_back(sum % base);
        carry = digits[i] != sum;
    }
}

void BigInteger::unsignedSubstraction(const BigInteger& other) {
    bool carry = 0;
    auto& big = (unsignedComparison(*this, other) ?
        (sign = 1, other) :
        (sign = 0, *this));
    auto& small = (unsignedComparison(*this, other) ? *this : other);
    size_t bigLen = big.digits.size();
    size_t smallLen = small.digits.size();
    for (size_t i = 0; i < bigLen || carry; ++i) {
        int64_t sub = static_cast<int64_t>(big.digits[i]) -
            (i < smallLen ? small.digits[i] : 0) - carry;
        carry = sub < 0;
        if (carry) sub += base;
        if (i < digits.size()) digits[i] = sub;
        else digits.push_back(sub);
    }
    truncateZeros();
}

std::pair<BigInteger, BigInteger> BigInteger::Split(const BigInteger& x) {
    std::pair<BigInteger, BigInteger> res;
    size_t mid = x.digits.size() / 2;
    std::vector<uint32_t> left(x.digits.begin(), x.digits.begin() + mid);
    std::vector<uint32_t> right(x.digits.begin() + mid, x.digits.end());
    res.first.digits.swap(left);
    res.second.digits.swap(right);
    return res;
}

BigInteger& BigInteger::shift(size_t n) {
    for (size_t i = 0; i < n; ++i)
        digits.insert(digits.begin(), 0);
    return *this;
}

BigInteger BigInteger::unsignedMultiplication(
    const BigInteger& lhs, const BigInteger& rhs) {

    if (lhs.digits.size() == 1)
        return BigInteger(lhs.digits[0] * rhs.digits[0]);
    size_t mid = lhs.digits.size() / 2;
    auto lhsParts = Split(lhs);
    auto rhsParts = Split(rhs);
    auto x1 = unsignedMultiplication(lhsParts.first, rhsParts.first);
    auto x2 = unsignedMultiplication(lhsParts.second, rhsParts.second);
    x1.truncateZeros();
    x2.truncateZeros();
    lhsParts.second += lhsParts.first;
    rhsParts.second += rhsParts.first;
    size_t len = std::max(lhsParts.second.digits.size(),
        rhsParts.second.digits.size());
    appendZeros(lhsParts.second, len);
    appendZeros(rhsParts.second, len);
    auto x3 = unsignedMultiplication(lhsParts.second, rhsParts.second);
    x3.truncateZeros();
    x3 -= x1;
    x3 -= x2;
    x2.shift(mid * 2);
    x3.shift(mid);
    BigInteger res;
    res += x1;
    res += x2;
    res += x3;
    res.truncateZeros();
    return res;
}

void BigInteger::truncateZeros() {
    while (digits.size() > 1 && !digits.back()) digits.pop_back();
}

size_t BigInteger::appendZeros(BigInteger& i, size_t n) {
    size_t k = 0;
    for (; i.digits.size() < n; ++k) i.digits.push_back(0);
    return k;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    if (!sign && other.sign) unsignedSubstraction(other);
    else if (sign && !other.sign) {
        unsignedSubstraction(other);
        sign = !sign;
        if (digits.size() == 1 && digits[0] == 0) sign = 0;
    }
    else unsignedAddition(other);
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    if (!sign && other.sign) unsignedAddition(other);
    else if (sign && !other.sign) unsignedAddition(other);
    else if (sign) {
        unsignedSubstraction(other);
        sign = !sign;
        if (digits.size() == 1 && digits[0] == 0) sign = 0;
    }
    else unsignedSubstraction(other);
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    size_t len = std::max(digits.size(), other.digits.size());
    auto x = *this;
    auto y = other;
    appendZeros(x, len);
    appendZeros(y, len);
    auto mul = unsignedMultiplication(x, y);
    digits.swap(mul.digits);
    sign ^= other.sign;
    if (digits.size() == 1 && digits[0] == 0) sign = 0;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    if (this == &other) return *this = BigInteger(1);
    BigInteger res;
    bool newSign = sign ^ other.sign;
    while (!unsignedComparison(*this, other)) {
        auto tmp = other;
        size_t k = 0;
        for (; !unsignedComparison(*this, tmp); ++k) tmp.shift(1);
        tmp.digits.erase(tmp.digits.begin());
        unsignedSubstraction(tmp);
        res += BigInteger(1).shift(k - 1);
    }
    digits.swap(res.digits);
    sign = newSign;
    if (digits.size() == 1 && digits[0] == 0) sign = 0;
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    return *this -= (*this / other) * other;
}

BigInteger BigInteger::operator+() const {
    return BigInteger(*this);
}

BigInteger BigInteger::operator-() const {
    BigInteger tmp = *this;
    if (tmp != 0) tmp.sign = !tmp.sign;
    return tmp;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp = lhs;
    tmp += rhs;
    return tmp;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp = lhs;
    tmp -= rhs;
    return tmp;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp = lhs;
    tmp *= rhs;
    return tmp;
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp = lhs;
    tmp /= rhs;
    return tmp;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger tmp = lhs;
    tmp %= rhs;
    return tmp;
}

BigInteger& BigInteger::operator++() {
    return *this += 1;
}

BigInteger& BigInteger::operator--() {
    return *this -= 1;
}

BigInteger BigInteger::operator++(int) {
    BigInteger tmp = *this;
    ++*this;
    return tmp;
}

BigInteger BigInteger::operator--(int) {
    BigInteger tmp = *this;
    --*this;
    return tmp;
}

bool BigInteger::unsignedComparison(const BigInteger& lhs,
    const BigInteger& rhs) {

    if (lhs.digits.size() < rhs.digits.size()) return true;
    if (rhs.digits.size() < lhs.digits.size()) return false;
    size_t i = lhs.digits.size() - 1;
    for (; (lhs.digits[i] == rhs.digits[i]) && (i > 0); --i);
    return lhs.digits[i] < rhs.digits[i];
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
    if (!lhs.sign && rhs.sign) return false;
    if (lhs.sign && !rhs.sign) return true;
    if (lhs.sign && rhs.sign) 
        return BigInteger::unsignedComparison(rhs, lhs);
    return BigInteger::unsignedComparison(lhs, rhs);
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
    return rhs < lhs;
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs < rhs);
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs < rhs) && !(lhs > rhs);
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs == rhs);
}

std::string BigInteger::toString() const {
    std::string tmp(sign ? "-" : "");
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        auto num = std::to_string(*it);
        size_t padding = baseExp - num.size();
        if (it == digits.rbegin()) padding = 0;
        tmp.append(std::string(padding, '0') + num);
    }
    return tmp;
}

std::istream& operator>>(std::istream& input, BigInteger& i) {
    std::string value;
    input >> value;
    i = BigInteger(value);
    return input;
}

std::ostream& operator<<(std::ostream& output, const BigInteger& i) {
    output << i.toString();
    return output;
}

BigInteger::operator bool() const {
    return *this != 0;
}
