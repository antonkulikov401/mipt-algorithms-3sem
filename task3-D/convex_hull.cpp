#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>
#include <cmath>
#include "convex_hull.hpp"

Point::Point() : n(0), x(0), y(0), z(0), prev(nullptr), next(nullptr) {}

Point::Point(size_t n, double x, double y, double z) :
    n(n), x(x), y(y), z(z), prev(nullptr), next(nullptr) {}

bool Point::operator<(const Point& other) const {
    return n < other.n;
}

bool Point::Action() {
    if (prev->next != this) {
        next->prev = prev->next = this;
        return true;
    }
    prev->next = next;
    next->prev = prev;
    return false;
}

Face::Face(const Point& first, const Point& second, const Point& third) :
    first(first), second(second), third(third) {}

void Face::Sort() {
    if (second.n < first.n && second.n < third.n) {
        std::swap(first, second);
        std::swap(second, third);
    }
    else if (third.n < first.n && third.n < second.n) {
        std::swap(first, third);
        std::swap(second, third);
    }
}

bool Face::operator<(const Face& other) const {
    return std::tie(first, second, third) <
        std::tie(other.first, other.second, other.third);
}

std::istream& operator>>(std::istream& input, Point& p) {
    input >> p.x >> p.y >> p.z;
    return input;
}

std::ostream& operator<<(std::ostream& output, const Face& f) {
    output << "3 " << f.first.n << " " << f.second.n << " "
        << f.third.n << "\n";
    return output;
}

double Turn(Point* p, Point* q, Point* r) {
    if (!p || !q || !r) return std::numeric_limits<double>::infinity();
    return (q->x - p->x) * (r->y - q->y) - (q->y - p->y) * (r->x - q->x);
}

double Time(Point* p, Point* q, Point* r) {
    if (!p || !q || !r) return std::numeric_limits<double>::infinity();
    return ((q->x - p->x) * (r->z - q->z) -
        (q->z - p->z) * (r->x - q->x)) / Turn(p, q, r);
}

void FindBridge(Point*& left, Point*& right) {
    while (true) {
        if (Turn(left, right, right->next) < 0)
            right = right->next;
        else if (Turn(left->prev, left, right) < 0)
            left = left->prev; 
        else return;
    }
}

std::pair<double, size_t> GetNextAction(Point* left, Point* right,
        Point* u, Point* v, double prevTime) {
    std::vector<double> times(6);
    times[0] = left ? Time(left->prev, left, left->next) :
        std::numeric_limits<double>::infinity();
    times[1] = right ? Time(right->prev, right, right->next) :
        std::numeric_limits<double>::infinity();
    times[2] = Time(u->prev, u, v);
    times[3] = Time(u, u->next, v);
    times[4] = Time(u, v, v->next);
    times[5] = Time(u, v->prev, v);
    double min = std::numeric_limits<double>::infinity();
    size_t action = 0;
    for (size_t i = 0; i < 6; ++i) {
        if (times[i] > prevTime && times[i] < min)  {
            min = times[i];
            action = i;
        }
    }
    return { min, action };
}

void UpdateMovie(std::vector<Point*>& leftHull, 
        std::vector<Point*>& rightHull, std::vector<Point*>& hull,
        Point*& u, Point*& v, size_t& left, size_t& right, size_t& action) {
    if (action == 0) {
        if (leftHull[left]->x < u->x)
            hull.push_back(leftHull[left]);
        leftHull[left++]->Action();
    }
    else if (action == 1) {
        if (rightHull[right]->x > v->x)
            hull.push_back(rightHull[right]);
        rightHull[right++]->Action();
    }
    else if (action == 2) {
        hull.push_back(u);
        u = u->prev;
    }
    else if (action == 3) {
        u = u->next;
        hull.push_back(u);
    }
    else if (action == 4) {
        hull.push_back(v);
        v = v->next;
    }
    else if (action == 5) {
        v = v->prev;
        hull.push_back(v);
    }
}

std::vector<Point*> BuildLowerHull(PointIt begin, PointIt end) {
    if (end - begin == 1) return {};
    auto mid = begin + (end - begin) / 2;
    auto leftHull = BuildLowerHull(begin, mid);
    auto rightHull = BuildLowerHull(mid, end);
    Point* u = (mid - 1).base();
    Point* v = mid.base();
    FindBridge(u, v);
    size_t left = 0, right = 0;
    std::vector<Point*> hull;
    for (double time = -std::numeric_limits<double>::infinity();;) {
        auto [nextTime, action] = GetNextAction(
            left < leftHull.size() ? leftHull[left] : nullptr,
            right < rightHull.size() ? rightHull[right] : nullptr,
            u, v, time);
        if (nextTime == std::numeric_limits<double>::infinity()) break;
        UpdateMovie(leftHull, rightHull, hull, u, v, left, right, action);
        time = nextTime;
    }
    u->next = v;
    v->prev = u;
    for (auto it = hull.rbegin(); it != hull.rend(); ++it) {
        Point* curr = *it;
        if (curr->x > u->x && curr->x < v->x) {
            v->prev = u->next = curr;
            curr->prev = u;
            curr->next = v;
            if (curr->x <= (mid - 1)->x) u = curr;
            else v = curr;
            continue;
        }
        curr->Action();
        if (curr == u) u = u->prev;
        if (curr == v) v = v->next;
    }
    return hull;
}

void Perturbate(Point& point, double angle) {
    auto rotate = [angle](double x, double y) -> std::pair<double, double> {
        return { x * std::cos(angle) + y * std::sin(angle),
            -x * std::sin(angle) + y * std::cos(angle) };
    };
    std::tie(point.x, point.z) = rotate(point.x, point.z);
    std::tie(point.z, point.y) = rotate(point.z, point.y);
    std::tie(point.x, point.y) = rotate(point.x, point.y);
}

void AddHalfHull(std::vector<Point>& points, std::vector<Face>& hull,
        bool isLower) {
    auto lowerHull = BuildLowerHull(points.begin(), points.end());
    for (auto& p : lowerHull)
        hull.push_back(isLower != p->Action() ? Face(*p, *p->prev, *p->next) :
            Face(*p->prev, *p, *p->next));
}

std::vector<Face> BuildConvexHull(std::vector<Point>& points) {
    std::vector<Face> hull;
    for (auto& p : points) Perturbate(p, 0.0000001);
    std::sort(points.begin(), points.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.x < rhs.x; 
        }
    );
    AddHalfHull(points, hull, true);
    for (auto& p : points) {
        p.next = p.prev = nullptr;
        p.z *= -1;
    }
    AddHalfHull(points, hull, false);
    for (auto& face : hull) face.Sort();
    std::sort(hull.begin(), hull.end());
    return hull;
}

std::vector<Face> BuildLowerConvexHull(std::vector<Point>& points) {
    std::vector<Face> hull;
    for (auto& p : points) Perturbate(p, 0.0000001);
    std::sort(points.begin(), points.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.x < rhs.x;
        }
    );
    AddHalfHull(points, hull, true);
    return hull;
}
