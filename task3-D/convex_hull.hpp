#pragma once
#include <iostream>
#include <vector>

struct Point {
    size_t n;
    double x;
    double y;
    double z;
    Point* prev;
    Point* next;

    Point();
    Point(size_t, double, double, double);
    bool Action();
    friend std::istream& operator>>(std::istream&, Point&);
    bool operator<(const Point&) const;
};

struct Face {
    Point first;
    Point second;
    Point third;

    Face(const Point&, const Point&, const Point&);
    void Sort();
    friend std::ostream& operator<<(std::ostream&, const Face&);
    bool operator<(const Face& other) const;
};

using PointIt = std::vector<Point>::iterator;

std::vector<Face> BuildConvexHull(std::vector<Point>&);
std::vector<Face> BuildLowerConvexHull(std::vector<Point>&);
