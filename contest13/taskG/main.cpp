#include <iostream>
#include <vector>

class Finite {
private:
    int64_t _value = 0;

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
        int64_t cur_power_value = _value;
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
    const static int64_t N = 998244353;
    friend std::ostream& operator<<(std::ostream&, Finite&);
    friend std::istream& operator>>(std::istream&, Finite&);
    Finite() = default;
    Finite(int64_t x): _value(x % N) {
        if (_value < 0) {
            _value += N;
        }
    }
    Finite(Finite&& x): _value(x._value) {}
    Finite(const Finite& x): _value(x._value) {}
    Finite& operator=(const Finite& s) noexcept {
        _value = s._value;
        return *this;
    }
    Finite& operator=(Finite&& s) noexcept {
        _value = s._value;
        s._value = 0;
        return *this;
    }

    Finite& operator+=(const Finite& another) {
        _value += another._value;
        if (_value >= N) {
            _value -= N;
        }
        return *this;
    }

    Finite& operator-=(const Finite& another) {
        _value -= another._value;
        if (_value < 0) {
            _value += N;
        }
        return *this;
    }

    Finite& operator*=(const Finite& another) {
        _value *= another._value;
        _value %= N;
        return *this;
    }

    Finite& operator/=(const Finite& another) {
        //        int64_t x = 0;
        //        int64_t y = 0;
        //        extend_gcd(another.value, N, x, y);
        //        value *= x;
        //        value %= N;
        _value *= another.powerTo(N - 2);
        _value %= N;
        return *this;
    }

    Finite& operator%=(const Finite& another) {
        _value %= another._value;
        return *this;
    }

    Finite power(int64_t pow) const {
        return powerTo(pow);
    }

    int64_t value() const {
        return _value;
    }
};
Finite power(const Finite& x, int64_t pow) {
    return x.power(pow);
}
std::ostream& operator<<(std::ostream& out, Finite& write) {
    out << write._value;
    return out;
}
std::istream& operator>>(std::istream& in, Finite& read) {
    in >> read._value;
    read._value %= Finite::N;
    return in;
}
Finite operator+(Finite first, const Finite& second) { return first += second; }
Finite operator-(Finite first, const Finite& second) { return first -= second; }
Finite operator*(Finite first, const Finite& second) { return first *= second; }
Finite operator/(Finite first, const Finite& second) { return first /= second; }
Finite operator%(Finite first, const Finite& second) { return first %= second; }

class FFT {
private:
    int64_t k;
    int64_t n;
    int64_t p = Finite::N;
    int64_t deg = 23;
    int64_t w1 = 31;
    /*std::vector<int64_t> p = {7340033, 257};
    std::vector<int64_t> deg = {20, 8};
    std::vector<int64_t> w1 = {5, 3};*/
    std::vector<Finite> a;
    std::vector<Finite> b;
    std::vector<Finite> w_first;
    Finite rev_n = 0;
    Finite rev_m = 0;
    std::vector<int64_t> rev;

    template <bool is_reverse>
    std::conditional_t<is_reverse, std::vector<int64_t>, std::vector<Finite>> my_fft(const std::vector<Finite>& arr) {
        std::vector<Finite> answer = arr;
        for (int j = 0; j < k; ++j) {
            std::vector<Finite> w((1 << (j + 1)) + 1ll);
            w[0] = 1;
            w[1 << (j + 1)] = 1;
            w[1] = w_first[j];
            for (int64_t i = 2; i < (1 << (j + 1)); ++i) {
                w[i] = w[i - 1] * w[1];
            }
            for (int64_t i = 0; i < n; i += (1 << (j + 1))) {
                for (int64_t s = 0; s < (1 << j); ++s) {
                    Finite x = answer[i + s];
                    Finite y = answer[i + s + (1 << j)] * w[s];
                    answer[i + s] = x + y;
                    answer[i + s + (1 << j)] = x - y;
                }
            }
        }
        if constexpr (is_reverse) {
            for (int64_t i = 0; i < n; ++i) {
                answer[i] *= rev_n;
            }
            std::vector<int64_t> ans = std::vector<int64_t>(n);
            for (int64_t i = 0; i < n; ++i) {
                ans[i] = answer[i].value();
                if (ans[i] > p / 2ll) {
                    ans[i] -= p;
                }
            }
            return ans;
        } else {
            return answer;
        }
    }

    void extend_gcd(Finite A, Finite B, Finite& x, Finite& y) {
        if (A.value() == 0) {
            x = 0;
            y = 1;
            return;
        }
        Finite x1, y1;

        extend_gcd(B % A, A, x1, y1);
        x = (y1 - (B / A) * x1);
        y = x1;
    }

public:
    FFT(std::vector<int> _a, int64_t a_size, std::vector<int> _b, int64_t b_size, int k): k(k), n(1 << k), a(n), b(n), w_first(std::vector<Finite>(deg + 1)), rev(n) {
        rev[0] = 0;
        int oldest = -1;
        for (int64_t mask = 1; mask < n; ++mask) {
            if ((mask & (mask - 1)) == 0) {
                ++oldest;
            }
            rev[mask] = rev[mask ^ (1 << oldest)] | (1 << (k - oldest - 1));
        }

        for (int64_t i = 0; i < n; ++i) {
            a[i] = (rev[i] > a_size) ? 0 : _a[rev[i]];
        }
        for (int64_t i = 0; i < n; ++i) {
            b[i] = (rev[i] > b_size) ? 0 : _b[rev[i]];
        }

        w_first[deg - 1] = w1;
        for (int64_t j = 2; j <= deg; ++j) {
            w_first[deg - j] = (w_first[deg - j + 1] * w_first[deg - j + 1]);
        }
        rev_n = 1 / Finite(n);
    }

    std::vector<int64_t> fft() {
        std::vector<Finite> a_ans = my_fft<false>(a);
        std::vector<Finite> b_ans = my_fft<false>(b);
        std::vector<Finite> r = std::vector<Finite>(n);
        //recovery(first_a, second_a);
        for (int64_t i = 0; i < n; ++i) {
            r[i] = a_ans[i] * b_ans[i];
        }
        for (int i = 0; i < k; ++i) {
            w_first[i] = 1 / w_first[i];
        }
        std::vector<Finite> r_ans = std::vector<Finite>(n);
        for (int64_t i = 0; i < n; ++i) {
            r_ans[i] = r[rev[i]];
        }
        std::vector r_second = my_fft<true>(r_ans);
        return r_second;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int64_t n, m;
    std::cin >> n;
    std::vector<int> a(n + 1ll);
    for (int64_t i = 0; i <= n; ++i) {
        std::cin >> a[n - i];
    }
    std::cin >> m;
    std::vector<int> b(m + 1ll);
    for (int64_t i = 0; i <= m; ++i) {
        std::cin >> b[m - i];
    }

    int k;
    for (k = 0; (n + m + 1ll) > (1 << k); ++k);
    FFT for_solve = FFT(a, n, b, m, k   );
    std::cout << (n + m) << " ";
    std::vector<int64_t> ans = for_solve.fft();
    for (int64_t i = 0; i < n + m + 1ll; ++i) {
        std::cout << ans[n + m - i] << " ";
    }
    return 0;
}
