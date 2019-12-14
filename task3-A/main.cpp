#include <iostream>
#include "geometry.hpp"

int main() {
    Vector p[4];
    double x = 0, y = 0, z = 0;
    for (size_t i = 0; i < 4; ++i) {
        std::cin >> x >> y >> z;
        p[i] = { x, y, z };
    }
    std::cout.precision(10);
    std::cout << DistanceBetweenSegments({ p[0], p[1] }, { p[2], p[3] });
    return 0;
}
