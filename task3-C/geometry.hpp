#pragma once
#include <vector>

struct Vector {
    double x = 0;
    double y = 0;

    Vector() = default;
    Vector(double x, double y);

    bool operator==(const Vector& other) const;
    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector& operator*=(double c);
    double operator^(const Vector& other) const;
    friend Vector operator*(double c, const Vector& v);

    double DotProduct(const Vector& other) const;
    double Norm() const;
};

using Polygon = std::vector<Vector>;

bool CheckIntersection(Polygon&, Polygon&);
