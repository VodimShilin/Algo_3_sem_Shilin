#include <iostream>
#include <string>
#include <vector>

class ZFunction {
private:
    std::vector<int64_t> zfunction;
    std::string base;

public:
    ZFunction() {
        std::cin >> base;
        zfunction = std::vector<int64_t>(base.size());
    }
    explicit ZFunction(const std::string& base): zfunction(base.size()), base(base) {}
    explicit ZFunction(std::string&& base): zfunction(base.size()), base(std::move(base)) {}

    void z_function() {
        int64_t n = base.size();
        int64_t l = -1;
        int64_t r = -1;
        std::cout << 0 << ' ';
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
            std::cout << zfunction[i] << ' ';
        }
    }

    void print_z_function() {
        for (size_t i = 0; i < base.size(); ++i) {
            std::cout << zfunction[i] << ' ';
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    ZFunction z = ZFunction();
    z.z_function();
    return 0;
}
