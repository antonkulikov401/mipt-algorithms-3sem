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

struct Edge {
    Vector2 first;
    Vector2 second;
    
    Edge(const Vector2&, const Vector2&);
    void Sort();
    bool operator==(const Edge&) const;
};

double GetVoronoiAverageEdgeCount(std::vector<Vector2>&);
