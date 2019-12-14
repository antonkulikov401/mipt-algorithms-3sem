#pragma once
#include <iostream>
#include <vector>

struct Vector2 {
    double x;
    double y;

    friend std::istream& operator>>(std::istream&, Vector2&);
    bool operator<(const Vector2&) const;
    bool operator==(const Vector2&) const;
};

struct Segment {
    Vector2 first;
    Vector2 second;
    
    void Sort();
    bool operator==(const Segment&) const;
};

double GetVoronoiAverageEdgeCount(std::vector<Vector2>&);
