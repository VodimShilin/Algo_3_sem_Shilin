#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

struct Line;

inline bool are_equal(int64_t x, int64_t y) {
    return x == y;
}

struct Point {
    int64_t x = 0.0;
    int64_t y = 0.0;

    Point() = default;

    Point(int64_t x, int64_t y) : x(x), y(y) {}

    Point(const Point& point)  = default;
    Point(Point&& point) noexcept: x(point.x), y(point.y) {}
    Point& operator=(const Point& point) = default;
    Point& operator=(Point&& point) noexcept {
        x = point.x;
        y = point.y;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& point);
    friend Line;

    bool operator==(const Point& right) const {
        return are_equal(x, right.x) && are_equal(y, right.y);
    }

    bool operator!=(const Point& right) const {
        return !(*this == right);
    }

    bool operator<(const Point& right) const {
        return right.x > x || (are_equal(right.x, x) && right.y > y);
    }
//    bool operator<=(const Point& right) const {
//        return *this < right || *this == right;
//    }

    bool operator>(const Point& right) const {
        return right.x < x || (are_equal(right.x, x) && right.y < y);
    }
//    bool operator>=(const Point& right) const {
//        return *this > right || *this == right;
//    }

    Point& operator-=(const Point& right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    Point& operator+=(const Point& right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    Point& operator+=(int64_t right) {
        x += right;
        y += right;
        return *this;
    }

    int64_t modul() const {
        return x * x + y * y;
    }

    int64_t dot(const Point& right) const {
        return x * right.x + y * right.y;
    }

    int64_t cross(const Point& right) const {
        return x * right.y - y * right.x;
    }

    int64_t dist(const Point& second) const {
        if (are_equal(x, second.x)) {
            if (y > second.y) {
                return y - second.y;
            } else {
                return second.y - y;
            }
        } else if (are_equal(y, second.y)) {
            if (x > second.x) {
                return x - second.x;
            } else {
                return second.x - x;
            }
        }
        return sqrtl((x - second.x) * (x - second.x) + (y - second.y) * (y - second.y));
    }
};

std::ostream& operator<<(std::ostream& out, const Point& point) {
    out << point.x << " " << point.y;
    return out;
}

Point operator-(Point left, const Point& right) {
    return left -= right;
}

Point operator+(Point left, const Point& right) {
    return left += right;
}
Point operator+(Point left, int64_t right) {
    return left += right;
}

int64_t dot(Point left, const Point& right) {
    return left.dot(right);
}

int64_t cross(Point left, const Point& right) {
    return left.cross(right);
}

struct Vector : public Point {
    using Point::Point;

    Vector(int64_t x1, int64_t y1, int64_t x2, int64_t y2): Point(x2 - x1, y2 - y1) {}

    Vector(const Point& p1, const Point& p2): Point(p2 - p1) {}

    Vector& operator*=(int64_t k) {
        x *= k;
        y *= k;
        return *this;
    }

    long double length() const {
        return sqrtl(modul());
    }
};

Vector operator*(Vector vec, int64_t k) {
    return vec *= k;
}

class Line {
protected:
    int64_t a = 0.0;
    int64_t b = 0.0;
    int64_t c = 0.0;

public:
    friend std::ostream& operator<<(std::ostream&, const Line&);
    friend Point;
    Line() = default;
    Line(const double& k, const double& b) : a(k), b(-1), c(b) {}
    Line(const Point& point, const double& k) : a(k), b(-1), c(point.y - point.x * k) {}
    Line(const Point& point1, const Point& point2) : a(point1.y - point2.y), b(point2.x - point1.x), c(point1.x* point2.y - point1.y * point2.x) {}
    Line(Point&& point1, Point&& point2) : a(point1.y - point2.y), b(point2.x - point1.x), c(point1.x * point2.y - point1.y * point2.x) {}
    Line(const int64_t& a, const int64_t& b, const int64_t& c) : a(a), b(b), c(c) {}
    long double yVal(const double& x) const {
        return (a * x + c) / (-b);
    }

    bool operator==(const Line& right) const {

    }

    Vector normal() const {
        return {a, b};
    }

    Vector direction() const {
        return {-b, a};
    }

    bool operator!=(const Line& right) const {
        return !(*this == right);
    }

    std::pair<Point, bool> intersectionPoint(const Line& second) const {
        long double det = a * second.b - b * second.a;
        return !are_equal(det, 0) ?
                                  std::make_pair(Point((b * second.c - second.b * c) / det, (c * second.a - second.c * a) / det), 1) :
                                  std::make_pair(Point(), 0);
    }

    bool isBelong(const Point& point) {
        return are_equal(a * point.x + b * point.y + c, 0);
    }

    long double dist(Line second) const {
        long double a1, b1, c1;
        if (!(are_equal(a, 0))) {
            b1 = b / a;
            c1 = c / a;
            a1 = 1;
        } else if (!are_equal(b, 0)) {
            c1 = c / b;
            a1 = a / b;
            b1 = 1;
        } else {
            c1 = c;
        }
        if (!are_equal(second.a, 0)) {
            second.b /= second.a;
            second.c /= second.a;
            second.a = 1;
        } else if (!are_equal(second.b, 0)) {
            second.c /= second.b;
            second.a /= second.b;
            second.b = 1;
        }
        return second.c > c1 ?
                             (second.c - c1) / sqrtl(a1 * a1 + b1 * b1):
                             (c1 - second.c) / sqrtl(a1 * a1 + b1 * b1);
    }
};

std::ostream& operator<<(std::ostream& out, const Line& line) {
    out << "(" << line.a << "," << line.b << "," << line.c << ")";
    return out;
}

std::pair<Point, bool> intersectionPoint(const Line& first, const Line& second) {
    return first.intersectionPoint(second);
}

class Section: public Line {
private:
    Point point1;
    Point point2;

    bool areIntersectedAsix(int64_t a, int64_t b, int64_t c, int64_t d) const {
        if (a > b)  std::swap (a, b);
        if (c > d)  std::swap (c, d);
        return std::max(a, c) <= std::min(b, d);
    }

public:
    Section() = default;

    Section(const Point& _point1, const Point& _point2): Line(_point1, _point2) {
        if (point1 < point2) {
            point1 = _point1;
            point2 = _point2;
        } else {
            point2 = _point1;
            point1 = _point2;
        }
    }
    Section(Point&& _point1, Point&& _point2): Line(std::move(_point1), std::move(_point2)) {
        if (point1 < point2) {
            point1 = _point1;
            point2 = _point2;
        } else {
            point2 = _point1;
            point1 = _point2;
        }
    }

    bool isBelong(const Point& point) {
        return areIntersectedAsix(point1.x, point2.x, point.x, point.x) &&
               areIntersectedAsix(point1.y, point2.y, point.y, point.y) &&
               Line::isBelong(point);
    }

    bool areIntersected(const Section& second) const {
        return areIntersectedAsix(point1.x, point2.x, second.point1.x, second.point2.x) &&
               areIntersectedAsix(point1.y, point2.y, second.point1.y, second.point2.y) &&
               Vector(point1, second.point1).cross(std::forward<Vector>({point1, second.point2})) *
                               Vector(point2, second.point1).cross(std::forward<Vector>({point2, second.point2})) <= 0 &&
               Vector(second.point1, point1).cross(std::forward<Vector>({second.point1, point2})) *
                               Vector(second.point2, point1).cross(std::forward<Vector>({second.point2, point2})) <= 0;
    }
};

class ConvexHull {
private:
    std::vector<Point> points;
    std::vector<Point> upper_convex;
    std::vector<Point> lower_convex;
    int n;

public:
    ConvexHull(int n): n(n), points(n) {
        for (int i = 0; i < n; ++i) {
            std::cin >> points[i].x >> points[i].y;
        }
    }

    void createConvexHull() {
        std::sort(points.begin(), points.end());
        for (int i = 0; i < n; ++i) {
            while (upper_convex.size() >= 2) {
                if (Vector(upper_convex[upper_convex.size() - 2], upper_convex[upper_convex.size() - 1]).
                    cross(Vector(upper_convex[upper_convex.size() - 1], points[i])) <= 0) {
                    upper_convex.pop_back();
                } else {
                    break;
                }
            }
            upper_convex.push_back(points[i]);

            while (lower_convex.size() >= 2) {
                if (Vector(lower_convex[lower_convex.size() - 2], lower_convex[lower_convex.size() - 1]).
                    cross(Vector(lower_convex[lower_convex.size() - 1], points[i])) >= 0) {
                        lower_convex.pop_back();
                } else {
                    break;
                }
            }
            lower_convex.push_back(points[i]);
        }
    }

    void printConvexHull() {
        std::cout << lower_convex.size() + upper_convex.size() - 2 << '\n';
        for (int i = 0; i + 1 < lower_convex.size(); ++i) {
            std::cout << lower_convex[i] << '\n';
        }
        for (int i = static_cast<int64_t>(upper_convex.size()) - 1; i > 0; --i) {
            std::cout << upper_convex[i] << '\n';
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    ConvexHull points_set = ConvexHull(n);
    points_set.createConvexHull();
    points_set.printConvexHull();
    return 0;
}
