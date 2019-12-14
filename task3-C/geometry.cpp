#include "geometry.hpp"
#include <cmath>
#include <algorithm>
#include <iterator>
#include <limits>

Vector::Vector(double x, double y) : x(x), y(y) {}

bool Vector::operator==(const Vector& other) const {
    constexpr double eps = 0.0000001;
    return std::abs(x - other.x) <= eps &&
        std::abs(y - other.y) <= eps;
}

Vector& Vector::operator+=(const Vector& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector Vector::operator+(const Vector& other) const {
    Vector result = *this;
    result += other;
    return result;
}

Vector Vector::operator-(const Vector& other) const {
    Vector result = *this;
    result -= other;
    return result;
}

Vector& Vector::operator*=(double c) {
    x *= c;
    y *= c;
    return *this;
}

Vector operator*(double c, const Vector& v) {
    Vector result = v;
    result *= c;
    return result;
}

double Vector::operator^(const Vector& other) const {
    return x * other.y - y * other.x;
}

double Vector::DotProduct(const Vector& other) const {
    return x * other.x + y * other.y;
}

double Vector::Norm() const {
    return std::sqrt(DotProduct(*this));
}

bool ComparePolarAngles(const Vector& lhs, const Vector& rhs) {
    auto quadrant = [](const Vector& v) { 
        return v.x >= 0 ? (v.y >= 0 ? 0 : 3) : (v.y >= 0 ? 1 : 2);
    };
    auto angle = [](const Vector& v) { 
        return v.x == 0 ? std::numeric_limits<double>::infinity() :
            std::abs(v.y / v.x);
    };
    if (quadrant(lhs) < quadrant(rhs)) return true;
    if (quadrant(lhs) > quadrant(rhs)) return false;
    return (quadrant(lhs) % 2 ? angle(lhs) > angle(rhs) :
        angle(lhs) < angle(rhs));
}

Polygon MinkowskiSum(const Polygon& firstPolygon,
        const Polygon& secondPolygon) {
    Polygon sum;
    auto it1 = firstPolygon.cbegin();
    auto it2 = secondPolygon.cbegin();
    sum.push_back(*it1 + *it2);
    auto push = [&sum](auto& it) {
        sum.push_back(sum[sum.size() - 1] + *(it + 1) - *it);
        ++it;
    };
    while ((it1 + 1) != firstPolygon.cend() &&
            (it2 + 1) != secondPolygon.cend()) {
        if (ComparePolarAngles(*(it1 + 1) - *it1, *(it2 + 1) - *it2)) {
            push(it2);
            continue;
        }
        push(it1);
    }
    while ((it1 + 1) != firstPolygon.cend()) push(it1);
    while ((it2 + 1) != secondPolygon.cend()) push(it2);
    return sum;
}

bool IsInConvexPolygon(const Vector& point, const Polygon& polygon) {
    bool sign = false;
    for (auto v = polygon.cbegin(); (v + 1) != polygon.cend(); ++v) {
        auto v1 = *v;
        auto v2 = *(v + 1);
        auto e = v2 - v1;
        double pos = point.x * e.y - point.y * e.x + (v2 ^ v1);
        if (pos == 0) return true;
        if (v == polygon.cbegin()) sign = std::signbit(pos);
        if (std::signbit(pos) != sign) return false;
    }
    return true;
}

void OrientPolygon(Polygon& polygon) {
    auto max = std::max_element(polygon.begin(), polygon.end(),
        [](const Vector& lhs, const Vector& rhs) {
            return lhs.y == rhs.y ? lhs.x < rhs.x : lhs.y < rhs.y; 
        }
    );
    size_t dist = std::distance(polygon.begin(), max);
    max = polygon.insert(polygon.end(), polygon.begin(), max + 1);
    polygon.erase(polygon.begin(), polygon.begin() + dist);
}

bool CheckIntersection(Polygon& firstPolygon, Polygon& secondPolygon) {
    for (auto& vertex : secondPolygon) vertex *= -1;
    OrientPolygon(firstPolygon);
    OrientPolygon(secondPolygon);
    auto minkowskiSum = MinkowskiSum(firstPolygon, secondPolygon);
    if (IsInConvexPolygon({ 0, 0 }, minkowskiSum)) return true;
    return false;
}
