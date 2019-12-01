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
        return std::hash<double>()(v.x) ^ std::hash<double>()(v.y);
    }
};

template<>
struct std::hash<Edge> {
    size_t operator()(const Edge& e) const {
        return std::hash<Vector2>()(e.first) ^ std::hash<Vector2>()(e.second);
    }
};

Edge::Edge(const Vector2& first, const Vector2& second) :
    first(first), second(second) {}

bool Edge::operator==(const Edge& other) const {
    return first == other.first && second == other.second;
}

void Edge::Sort() {
    if (second < first) std::swap(first, second);
}

Vector2 PointToVector2(const Point& point) {
    return { point.x, point.y };
}

std::vector<Edge> GetEdges(const Face& face) {
    std::vector<Edge> edges;
    edges.push_back(Edge(PointToVector2(face.first),
        PointToVector2(face.second)));
    edges.push_back(Edge(PointToVector2(face.first),
        PointToVector2(face.third)));
    edges.push_back(Edge(PointToVector2(face.third),
        PointToVector2(face.second)));
    for (auto& edge : edges) edge.Sort();
    return edges;
}

std::unordered_set<Vector2> GetVertices(std::vector<Face> hull) {
    std::unordered_set<Vector2> points;
    for (const auto& face : hull) {
        points.insert(PointToVector2(face.first));
        points.insert(PointToVector2(face.second));
        points.insert(PointToVector2(face.third));
    }
    return points;
}

std::pair<std::unordered_set<Edge>, std::unordered_set<Edge>>
GetEdges(std::vector<Face>& hull) {
    std::unordered_set<Edge> outerEdges;
    std::unordered_set<Edge> innerEdges;
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
