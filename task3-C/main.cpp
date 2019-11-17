#include <iostream>
#include "geometry.hpp"

Polygon ReadPolygon(size_t n) {
    Polygon polygon(n);
    for (size_t i = 0; i < n; ++i) {
        double x = 0, y = 0;
        std::cin >> x >> y;
        polygon[i] = { x, y };
    }
    return polygon;
}

int main() {
    size_t n = 0, m = 0;
    std::cin >> n;
    auto firstPolygon = ReadPolygon(n);
    std::cin >> m;
    auto secondPolygon = ReadPolygon(m);
    std::cout << (CheckIntersection(firstPolygon, secondPolygon) ?
        "YES" : "NO") << std::endl;
    return 0;
}
