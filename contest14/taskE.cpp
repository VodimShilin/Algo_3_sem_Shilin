#include <cmath>
#include <iostream>
#include <vector>

const long double eps = 0.0000000001;

bool are_equal(const long double& first, const long double& second) {
    if ((first >= second && first - second < eps) || (first < second && second - first < eps)) {
        return true;
    } else {
        return false;
    }
}

struct Line;

struct Point {
    long double x = 0.0;
    long double y = 0.0;

    Point() = default;

    Point(const long double& x, const long double& y) : x(x), y(y) {}

    Point(Point&& point) = default;
    Point(const Point& point) noexcept: x(point.x), y(point.y) {}
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
        return right.x > x + eps || (are_equal(right.x, x) && right.y > y + eps);
    }

    bool operator>(const Point& right) const {
        return right.x + eps < x || (are_equal(right.x, x) && right.y + eps< y);
    }

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

    Point& operator+=(const long double& right) {
        x += right;
        y += right;
        return *this;
    }

    long double modul() const {
        return x * x + y * y;
    }

    long double dot(const Point& right) const {
        return x * right.x + y * right.y;
    }

    long double cross(const Point& right) const {
        return x * right.y - y * right.x;
    }

    long double dist(const Point& second) const {
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
Point operator+(Point left, const long double& right) {
    return left += right;
}

long double dot(Point left, const Point& right) {
    return left.dot(right);
}

long double cross(Point left, const Point& right) {
    return left.cross(right);
}

struct Vector : public Point {
    using Point::Point;

    Vector(const long double& x1, const long double& y1, const long double& x2, const long double& y2): Point(x2 - x1, y2 - y1) {}

    Vector(const Point& p1, const Point& p2): Point(p2 - p1) {}

    Vector& operator*=(const long double& k) {
        x *= k;
        y *= k;
        return *this;
    }

    long double length() const {
        return sqrtl(modul());
    }
};

Vector operator*(Vector vec, const long double& k) {
    return vec *= k;
}

class Line {
protected:
    long double a = 0.0;
    long double b = 0.0;
    long double c = 0.0;

public:
    friend std::ostream& operator<<(std::ostream&, const Line&);
    friend Point;
    Line() = default;
    Line(const double& k, const double& b) : a(k), b(-1), c(b) {}
    Line(const Point& point, const double& k) : a(k), b(-1), c(point.y - point.x * k) {}
    Line(const Point& point1, const Point& point2) : a(point1.y - point2.y), b(point2.x - point1.x), c(point1.x* point2.y - point1.y * point2.x) {}
    Line(Point&& point1, Point&& point2) : a(point1.y - point2.y), b(point2.x - point1.x), c(point1.x * point2.y - point1.y * point2.x) {}
    Line(const long double& a, const long double& b, const long double& c) : a(a), b(b), c(c) {}
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


    bool areIntersectedAsix(long double a, long double b, long double c, long double d) const {
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

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    Point point;
    std::cin >> point.x >> point.y;
    std::vector<Point> rectangle = std::vector<Point>(n);
    std::vector<Section> lines = std::vector<Section>(n);
    int count1 = 0;
    int count2 = 0;
    bool flag = false;
    for (int i = 0; i < n; ++i) {
        std::cin >> rectangle[i].x >> rectangle[i].y;
    }

    for (int i = 0; i + 1 < n; ++i) {
        lines[i] = Section(rectangle[i], rectangle[i + 1]);
    }
    lines[n - 1] = Section(rectangle[n - 1], rectangle[0]);

    Section ray1 = Section(point, {200000, -180200});
    Section ray2 = Section({-200000, 180200}, point);

    for (int i = 0; i < n; ++i) {
        if (lines[i].isBelong(point)) {
            flag = true;
            break;
        }
        if (lines[i].areIntersected(ray1)) {
            ++count1;
        }
        if (lines[i].areIntersected(ray2)) {
            ++count2;
        }
    }

    if (flag || (count1 % 2 == 1 && count2 % 2 == 1)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
