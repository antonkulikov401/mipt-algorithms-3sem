#include <iostream>
#include <vector>
#include <iomanip>
#include "voronoi.hpp"

int main() {
    std::vector<Vector2> points;
    Vector2 point;
    while (std::cin >> point) points.push_back(point);
    std::cout << std::setprecision(12) << GetVoronoiAverageEdgeCount(points);
    return 0;
}
