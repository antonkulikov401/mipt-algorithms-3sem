#include <unordered_set>
#include <tuple>
#include "convex_hull.hpp"
#include "voronoi.hpp"

std::istream& operator>>(std::istream& input, Vector2& v) {
    input >> v.x >> v.y;
    return input;
}

bool Vector2::operator<(const Vector2& other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
}

bool Vector2::operator==(const Vector2& other) const {
    return x == other.x && y == other.y;
}

template<>
struct std::hash<Vector2> {
    size_t operator()(const Vector2& v) const {
        size_t h1 = std::hash<double>()(v.x);
        size_t h2 = std::hash<double>()(v.y);
        return h1 ^ (h2 << 1);
    }
};

template<>
struct std::hash<Segment> {
    size_t operator()(const Segment& e) const {
        size_t h1 = std::hash<Vector2>()(e.first);
        size_t h2 = std::hash<Vector2>()(e.second);
        return h1 ^ (h2 << 1);
    }
};

bool Segment::operator==(const Segment& other) const {
    return first == other.first && second == other.second;
}

void Segment::Sort() {
    if (second < first) std::swap(first, second);
}

Vector2 ProjectionXY(const Point& point) {
    return { point.x, point.y };
}

std::vector<Segment> GetEdges(const Face& face) {
    std::vector<Segment> edges;
    edges.push_back({ ProjectionXY(face.first),
        ProjectionXY(face.second) });
    edges.push_back({ ProjectionXY(face.first),
        ProjectionXY(face.third) });
    edges.push_back({ ProjectionXY(face.third),
        ProjectionXY(face.second) });
    for (auto& edge : edges) edge.Sort();
    return edges;
}

std::unordered_set<Vector2> GetVertices(std::vector<Face> hull) {
    std::unordered_set<Vector2> points;
    for (const auto& face : hull) {
        points.insert(ProjectionXY(face.first));
        points.insert(ProjectionXY(face.second));
        points.insert(ProjectionXY(face.third));
    }
    return points;
}

std::pair<std::unordered_set<Segment>, std::unordered_set<Segment>>
GetEdges(std::vector<Face>& hull) {
    std::unordered_set<Segment> outerEdges;
    std::unordered_set<Segment> innerEdges;
    for (const auto& face : hull) {
        auto edges = GetEdges(face);
        for (const auto& edge : edges) {
            if (outerEdges.count(edge)) {
                outerEdges.erase(edge);
                innerEdges.insert(edge);
                continue;
            }
            outerEdges.insert(edge);
        }
    }
    return { innerEdges, outerEdges };
}

double GetVoronoiAverageEdgeCount(std::vector<Vector2>& points) {
    std::vector<Point> hullPoints(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        hullPoints[i] = Point(i, points[i].x, points[i].y,
            points[i].x * points[i].x + points[i].y * points[i].y);
    auto hull = BuildLowerConvexHull(hullPoints);

    auto [innerEdges, outerEdges] = GetEdges(hull);
    auto innerPoints = GetVertices(hull);
    for (const auto& edge : outerEdges) {
        innerPoints.erase(edge.first);
        innerPoints.erase(edge.second);
    }
    double degree = 0;
    for (const auto& edge : innerEdges) {
        if (innerPoints.count(edge.first)) ++degree;
        if (innerPoints.count(edge.second)) ++degree;
    }
    return innerPoints.size() == 0 ? 
    	0 : degree / innerPoints.size();
}
