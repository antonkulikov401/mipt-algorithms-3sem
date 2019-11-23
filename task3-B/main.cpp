#include <iostream>
#include <vector>
#include "geometry.hpp"

int main() {
    size_t m = 0;
    std::cin >> m;
    for (size_t i = 0; i < m; ++i) {
        size_t n = 0;
        std::cin >> n;
        std::vector<Point> points(n);
        for (size_t j = 0; j < n; ++j) {
            points[j].n = j;
            std::cin >> points[j];
        }
        auto hull = BuildConvexHull(points);
        std::cout << hull.size() << "\n";
        for (const auto& face : hull)
            std::cout << face;
    }
    return 0;
}
