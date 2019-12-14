#include "geometry.hpp"
#include <cmath>

bool Vector::operator==(const Vector& other) const {
    constexpr double eps = 0.0000001;
    return std::abs(x - other.x) <= eps &&
        std::abs(y - other.y) <= eps &&
        std::abs(z - other.z) <= eps;
}

Vector& Vector::operator+=(const Vector& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
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

Vector operator*(double c, const Vector& v) {
    Vector result = v;
    result.x *= c;
    result.y *= c;
    result.z *= c;
    return result;
}

double Vector::DotProduct(const Vector& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector Vector::CrossProduct(const Vector& other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

double Vector::Norm() const {
    return std::sqrt(DotProduct(*this));
}

bool Vector::BelongsToSegment(const Segment& seg) const {
    auto v1 = *this - seg.p1;
    auto v2 = seg.p2 - seg.p1;
    if (v1.CrossProduct(v2) == Vector() && v1.DotProduct(v2) >= 0 &&
        v1.DotProduct(v2) <= v2.DotProduct(v2)) return true;
    return false;
}

Segment::Segment(const Vector& _p1, const Vector& _p2) :
        p1(_p1), p2(_p2) {}
    
double Segment::Length() const {
    return (p2 - p1).Norm();
}

bool Segment::IsParallelTo(const Segment& other) const {
    return (p2 - p1).CrossProduct(other.p2 - other.p1) == Vector();
}

Vector Segment::Direction() const {
    return p2 - p1;
}

Segment ShortestSegmentBetweenLines(const Segment& l1, const Segment& l2) {
    Vector d1 = l1.Direction();
    Vector d2 = l2.Direction();
    Vector n1 = d1.CrossProduct(d1.CrossProduct(d2));
    Vector n2 = d2.CrossProduct(d2.CrossProduct(d1));
    return {
        l1.p1 + ((l2.p1 - l1.p1).DotProduct(n2) / d1.DotProduct(n2)) * d1,
        l2.p1 + ((l1.p1 - l2.p1).DotProduct(n1) / d2.DotProduct(n1)) * d2
    };
}

void AddMinimumOnBoundary(const Vector& p1, const Vector& p2,
        const Vector& d, std::vector<double>& distances) {
    double t = (p1 - p2).DotProduct(d) / d.DotProduct(d);
    if (t >= 0 && t <= 1)
        distances.push_back(Segment(p1, p2 + t * d).Length());
}

double DistanceBetweenSegments(const Segment& first, const Segment& second) {
    std::vector<double> distances;

    distances.push_back(Segment(first.p1, second.p1).Length());
    distances.push_back(Segment(first.p1, second.p2).Length());
    distances.push_back(Segment(first.p2, second.p1).Length());
    distances.push_back(Segment(first.p2, second.p2).Length());

    AddMinimumOnBoundary(second.p1, first.p1, first.Direction(), distances);
    AddMinimumOnBoundary(second.p2, first.p1, first.Direction(), distances);
    AddMinimumOnBoundary(first.p1, second.p1, second.Direction(), distances);
    AddMinimumOnBoundary(first.p2, second.p1, second.Direction(), distances);

    if (!first.IsParallelTo(second)) {
        auto seg = ShortestSegmentBetweenLines(first, second);
        if (seg.p1.BelongsToSegment(first) && seg.p2.BelongsToSegment(second))
            distances.push_back(seg.Length());
    }

    return *std::min_element(distances.begin(), distances.end());
}
