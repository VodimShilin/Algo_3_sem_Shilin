#include <iostream>
#include <vector>

class Finite {
private:
    int64_t value = 0;

    void normalize(int64_t& num) {
        num %= N;
        if (num < 0) {
            num += N;
        }
    }

    void extend_gcd(int64_t A, int64_t B, int64_t& x, int64_t& y) {
        if (A == 0) {
            x = 0;
            y = 1;
            return;
        }
        int64_t x1, y1;

        extend_gcd(B % A, A, x1, y1);
        x = (y1 - (B / A) * x1);
        y = x1;
        normalize(x);
        normalize(y);
    }

    [[nodiscard]] int64_t powerTo(int64_t power) const {
        int64_t to_return = 1;
        int64_t cur_power_value = value;
        for (; power > 0; power >>= 1) {
            if (power & 1) {
                to_return *= cur_power_value;
                to_return %= N;
            }
            cur_power_value *= cur_power_value;
            cur_power_value %= N;
        }
        return to_return;
    }

public:
    const static int64_t N = 1'000'000'007;
    friend std::ostream& operator<<(std::ostream&, Finite&);
    friend std::istream& operator>>(std::istream&, Finite&);
    Finite() = default;
    Finite(int64_t x): value(x % N) {
        if (value < 0) {
            value += N;
        }
    }
    Finite(Finite&& x): value(x.value) {}
    Finite(const Finite& x): value(x.value) {}

    Finite& operator+=(const Finite& another) {
        value += another.value;
        value %= N;
        return *this;
    }

    Finite& operator-=(const Finite& another) {
        value -= another.value;
        value += N;
        value %= N;
        return *this;
    }

    Finite& operator*=(const Finite& another) {
        value *= another.value;
        value %= N;
        return *this;
    }

    Finite& operator/=(const Finite& another) {
        int64_t x = 0;
        int64_t y = 0;
        extend_gcd(another.value, N, x, y);
        value *= x;
        value %= N;
//        value *= another.powerTo(N - 2);
        value %= N;
        return *this;
    }

    Finite power(int64_t pow) const {
        return powerTo(pow);
    }
};
Finite power(const Finite& x, int64_t pow) {
    return x.power(pow);
}
std::ostream& operator<<(std::ostream& out, Finite& write) {
    out << write.value;
    return out;
}
std::istream& operator>>(std::istream& in, Finite& read) {
    in >> read.value;
    read.value %= Finite::N;
    return in;
}
Finite operator+(Finite first, const Finite& second) { return first += second; }
Finite operator-(Finite first, const Finite& second) { return first -= second; }
Finite operator*(Finite first, const Finite& second) { return first *= second; }
Finite operator/(Finite first, const Finite& second) { return first /= second; }

class NumbersOfOneMan {
private:
    std::vector<Finite> a;
    int64_t k;
    int64_t n;
    Finite sum = 0;
    int64_t mod = 1'000'000'007;

    void summing() {
        for (int i = 0; i < n; ++i) {
            sum += a[i];
        }
    }

    void normalize(int64_t& num) {
        num %= mod;
        if (num < 0) {
            num += mod;
        }
    }

//    void extend_gcd(int64_t A, int64_t B, int64_t& x, int64_t& y) {
//        if (A == 0) {
//            x = 0;
//            y = 1;
//            return;
//        }
//        int64_t x1, y1;
//
//        extend_gcd(B % A, A, x1, y1);
//        x = (y1 - (B / A) * x1) % mod;
//        y = x1;
//        normalize(x);
//        normalize(y);
//    }

public:
    NumbersOfOneMan(std::vector<Finite>&& _a, int64_t _k): a(std::move(_a)), k(_k), n(a.size()) {}

    void solve() {
        summing();
        Finite factorial = 1;
        Finite ans = 0;
        Finite C = 1;
        Finite sign = 1;
        for (int i = 0; i < k; ++i) {
            Finite cur_ans = power(k - i, n - 2);
            cur_ans *= (k + n - i - 1);
            cur_ans *= C;
            cur_ans *= sign;
            ans += cur_ans;
            C /= i + 1;
            C *= k - i - 1;
            sign *= -1;
        }
        for (int i = 2; i < k; ++i) {
            factorial *= i;
        }
        ans /= factorial;
        ans *= sum;
        std::cout << ans;
    }
};

int main() {
    int64_t n, k;
    std::cin >> n >> k;
    std::vector<Finite> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    NumbersOfOneMan solution = NumbersOfOneMan(std::move(a), k);
    solution.solve();
    return 0;
}
