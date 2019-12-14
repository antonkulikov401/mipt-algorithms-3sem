#pragma once
#include <vector>
#include <algorithm>

struct Segment;

struct Vector {
    double x = 0;
    double y = 0;
    double z = 0;

    bool operator==(const Vector& other) const;

    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    friend Vector operator*(double c, const Vector& v);

    double DotProduct(const Vector& other) const;
    Vector CrossProduct(const Vector& other) const;

    double Norm() const;
    bool BelongsToSegment(const Segment& seg) const;
};

struct Segment {
    Vector p1;
    Vector p2;

    Segment(const Vector& _p1, const Vector& _p2);

    double Length() const;
    bool IsParallelTo(const Segment& other) const;
    Vector Direction() const;
};

double DistanceBetweenSegments(const Segment& first, const Segment& second);
