#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class ZFunction {
private:
    std::vector<int64_t> zfunction;
    std::string base;

public:

    explicit ZFunction(const std::string& base): zfunction(base.size()), base(base) {}
    explicit ZFunction(std::string&& base): zfunction(base.size()), base(std::move(base)) {}

    int64_t z_function() {
        int64_t n = base.size();
        int64_t l = -1;
        int64_t r = -1;
        int64_t max = 0;
        for (int64_t i = 1; i < n; ++i) {
            if (l <= i && i <= r) {
                zfunction[i] = std::min(zfunction[i - l], r - i + 1);
            }
            while (i + zfunction[i] < n && base[zfunction[i]] == base[i + zfunction[i]]) {
                ++zfunction[i];
            }
            if (i + zfunction[i] - 1 > r) {
                l = i;
                r = i + zfunction[i] - 1;
            }
            if (zfunction[i] > zfunction[max]) {
                max = i;
            }
        }
        return zfunction[max];
    }

    void print_z_function() {

    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string s;
    std::cin >> s;
    int64_t sum = 0;
    std::string string;
    for (int i = 0; i < s.size(); ++i) {
        string += s[i];
        std::string nstring = string;
        std::reverse(nstring.begin(), nstring.end());
        ZFunction z = ZFunction(std::move(nstring));
        sum += (i + 1) * (i + 2) / 2 - z.z_function() * (z.z_function() + 1) / 2;
    }
    std::cout << sum;
    return 0;
}

