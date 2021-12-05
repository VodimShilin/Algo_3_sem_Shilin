#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

struct Line;

inline bool are_equal(int64_t x, int64_t y) {
    return x == y;
}

inline bool are_different_non_zero_signs(int64_t x, int64_t y) {
    return (x > 0 && y > 0) || (x < 0 && y < 0) ? false : true;
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
    bool operator<=(const Point& right) const {
        return *this < right || *this == right;
    }

    bool operator>(const Point& right) const {
        return right.x < x || (are_equal(right.x, x) && right.y < y);
    }
    bool operator>=(const Point& right) const {
        return *this > right || *this == right;
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

    Point& operator+=(int64_t right) {
        x += right;
        y += right;
        return *this;
    }

    Point& operator*=(int64_t right) {
        x *= right;
        y *= right;
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

struct GreaterUpper {
    bool operator()(const Point& p1, const Point& p2) const {
        return p1 < p2;
    }
};

struct GreaterLower {
    bool operator()(const Point& p1, const Point& p2) const {
        return p2.x > p1.x || (are_equal(p2.x, p1.x) && p2.y < p1.y);
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
    std::set<Point> upper_convex;
    std::set<Point> lower_convex;
    int n;

public:
    ConvexHull() = default;

    ConvexHull(int n): n(n) {}

    ConvexHull(std::set<Point>&& upper, std::set<Point>&& lower): upper_convex(std::move(upper)), lower_convex(std::move(lower)) {}

    bool insideSet(const Point& point) {
        auto it_up = upper_convex.lower_bound(point);
        auto it_low = lower_convex.lower_bound(point);
        bool ans = true;
        if (it_up == upper_convex.end() || it_low == lower_convex.end()) {
            ans = false;
        } else if (it_up == upper_convex.begin() || it_low == lower_convex.begin()) {
            if (point >= *it_low && point <= *it_up) {
                ans = true;
            } else {
                ans = false;
            }
        } else if (are_different_non_zero_signs(Vector(point, *(it_up--)).cross(Vector(point, *it_up)),
                                                Vector(point, *(it_low--)).cross(Vector(point, *it_low)))) {
            ans = true;
        } else {
            ans = false;
        }
        return ans;
    }

    bool belowUpper(const Point& point) {
        auto it_up = upper_convex.lower_bound(point);
        bool ans = true;
        if (it_up == upper_convex.end()) {
            ans = false;
        } else if (it_up == upper_convex.begin()) {
            auto tmp_it_up = it_up;
            if (point <= *it_up || (point > *it_up && (++tmp_it_up != upper_convex.end() || GreaterLower()(point, *(tmp_it_up))))) {
                ans = false;
            } else {
                ans = true;
            }
        } else if (Vector(point, *(it_up--)).cross(Vector(point, *it_up)) >= 0) {
            ans = true;
        } else {
            ans = false;
        }
        return ans;
    }

    bool aboveLower(const Point& point) {
        auto it_low = lower_convex.lower_bound(point);
        bool ans = true;
        if (it_low == lower_convex.end()) {
            ans = false;
        } else if (it_low == lower_convex.begin()) {
            if (point >= *it_low) {
                ans = true;
            } else {
                ans = false;
            }
        } else if (Vector(point, *(it_low--)).cross(Vector(point, *it_low)) <= 0) {
            ans = true;
        } else {
            ans = false;
        }
        return ans;
    }

    void createConvexHull() {
        int flag;
        Point point;
        for (int i = 0; i < n; ++i) {
            std::cin >> flag >> point.x >> point.y;
            if (flag == 1) {
                if (!belowUpper(point)) {
                    auto it = upper_convex.lower_bound(point);
                    auto it_p = it;
                    auto it_q = it;
                    bool is_not_begin = false;
                    if (!upper_convex.empty() && it != upper_convex.begin()) {
                        --it;
                        is_not_begin = true;
                    }
                    if (it_q != upper_convex.end()) {
                        ++it_q;
                        while (it_q != upper_convex.end()) {
                            if (Vector(*it_q, *it_p).cross(Vector(*it_p, point)) <= 0) {
                                upper_convex.erase(it_p);
                                it_p = it_q++;
                            } else {
                                break;
                            }
                        }
                    }
                    if (is_not_begin && !upper_convex.empty() && it != upper_convex.begin()) {
                        it_p = it--;
                        it_q = it;
                        while (it_p != upper_convex.begin()) {
                            if (Vector(*it_q, *it_p).cross(Vector(*it_p, point)) >= 0) {
                                upper_convex.erase(it_p);
                                it_p = it_q--;
                            } else {
                                break;
                            }
                        }
                    }
                    if (!(!upper_convex.empty() && point.x == (--upper_convex.end())->x && point.y <= (--upper_convex.end())->y)) {
                        upper_convex.insert(point);
                    }
                }

                if (!aboveLower(point)) {
                    auto it = lower_convex.lower_bound(point);
                    auto it_p = it;
                    auto it_q = it;
                    bool is_not_begin = false;
                    if (!lower_convex.empty() && it != lower_convex.begin()) {
                        --it;
                        is_not_begin = true;
                    }
                    if (it_q != lower_convex.end()) {
                        ++it_q;
                        while (it_q != lower_convex.end()) {
                            if (Vector(*it_q, *it_p).cross(Vector(*it_p, point)) >= 0) {
                                lower_convex.erase(it_p);
                                it_p = it_q++;
                            } else {
                                break;
                            }
                        }
                    }
                    if (is_not_begin && !lower_convex.empty() && it != lower_convex.begin()) {
                        it_p = it--;
                        it_q = it;
                        while (it_p != lower_convex.begin()) {
                            if (Vector(*it_q, *it_p).cross(Vector(*it_p, point)) <= 0) {
                                lower_convex.erase(it_p);
                                it_p = it_q--;
                            } else {
                                break;
                            }
                        }
                    }
                    if (!(!lower_convex.empty() && point.x == lower_convex.begin()->x && point.y >= lower_convex.begin()->y)) {
                        lower_convex.insert(point);
                    }
                }
            } else {
                bool ans = insideSet(point);
                if (ans) {
                    std::cout << "YES\n";
                } else {
                    std::cout << "NO\n";
                }
            }
        }
    }
};

class PointsSet {
private:
    ConvexHull convex_hull;
    std::vector<Point> points;

    int find_min_elem(const std::vector<Point>& points) const {
        int ans = 0;
        for (int i = 1; i < points.size(); ++i) {
            if (points[i] < points[ans]) {
                ans = i;
            }
        }
        return ans;
    }

public:
    PointsSet(const std::vector<Point>& first, const std::vector<Point>& second, const std::vector<Point>& third) {
        std::vector<Point> temp_sum;
        MinkowskiSum(first, second, temp_sum);
        MinkowskiSum(temp_sum, third, points);
        std::set<Point> upper;
        std::set<Point> lower;
        int min = find_min_elem(points);
        int n = points.size();
        lower.insert(points[min]);
        upper.insert(points[min]);
        Vector vertical = Vector({0, 0}, {0, 1});
        int i = 0;
        for (; i + 1 < n; ++i) {
            if (Vector(points[(min + i) % n], points[(min + i + 1) % n]).cross(vertical) >= 0) {
                if (Vector(points[(min + i) % n], points[(min + i + 1) % n]).cross(Vector(points[(min + i + 1) % n], points[(min + i + 2) % n])) > 0) {
                    lower.insert(points[(min + i + 1) % n]);
                }
            } else {
                break;
            }
        }
        upper.insert(points[(min + i) % n]);
        for (; i + 1 < n; ++i) {
            if (Vector(points[(min + i) % n], points[(min + i + 1) % n]).cross(Vector(points[(min + i + 1) % n], points[(min + i + 2) % n])) > 0) {
                upper.insert(points[(min + i + 1) % n]);
            }
        }
        convex_hull = ConvexHull(std::move(upper), std::move(lower));
    }

    void MinkowskiSum(const std::vector<Point>& first, const std::vector<Point>& second, std::vector<Point>& ans) {
        int n = first.size();
        int m = second.size();
        int min1 = find_min_elem(first);
        int min2 = find_min_elem(second);
        Point last = first[min1] + second[min2];
        ans.push_back(last);
        int i1 = 1;
        int i2 = 1;
        Vector u = Vector(first[min1], first[(min1 + 1) % n]);
        Vector v = Vector(second[min2], second[(min2 + 1) % m]);
        while (i1 <= n && i2 <= m && (i1 + i2 <= n + m)) {
            if (u.cross(v) > 0) {
                last += u;
                ans.push_back(last);
                u = Vector(first[(min1 + i1) % n], first[(min1 + i1 + 1) % n]);
                ++i1;
            } else {
                last += v;
                ans.push_back(last);
                v = Vector(second[(min2 + i2) % m], second[(min2 + i2 + 1) % m]);
                ++i2;
            }
        }
        while (i1 <= n && (i1 + i2 <= n + m)) {
            last += u;
            ans.push_back(last);
            u = Vector(first[(min1 + i1) % n], first[(min1 + i1 + 1) % n]);
            ++i1;
        }
        while (i2 <= m && (i1 + i2 <= n + m)) {
            last += v;
            ans.push_back(last);
            v = Vector(second[(min2 + i2) % m], second[(min2 + i2 + 1) % m]);
            ++i2;
        }
    }

    bool isInside(const Point& point) {
        return convex_hull.insideSet(point);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n, m, k, q;
    std::cin >> n;
    std::vector<Point> first(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> first[i].x >> first[i].y;
    }
    std::cin >> m;
    std::vector<Point> second(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> second[i].x >> second[i].y;
    }
    std::cin >> k;
    std::vector<Point> third(k);
    for (int i = 0; i < k; ++i) {
        std::cin >> third[i].x >> third[i].y;
    }
    PointsSet set = PointsSet(first, second, third);

    std::cin >> q;
    Point cur_point;
    for (int i = 0; i < q; ++i) {
        std::cin >> cur_point.x >> cur_point.y;
        cur_point *= 3;
        if (set.isInside(cur_point)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    return 0;
}
