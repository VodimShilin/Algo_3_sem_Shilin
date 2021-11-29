#include <iostream>
#include <string>
#include <vector>

struct SuffixArray {
private:
    size_t max_c;
    int64_t n;
    std::vector<size_t> P;
    std::vector<size_t> C;
    std::vector<int> powersnk;
    std::vector<int64_t> powerskn;
    std::string data;

public:
    SuffixArray(std::string&& s): n(s.size()), P(n), C(n), powersnk(n + 1), data(std::move(s)) {
        powersnk[0] = 1;
        for (int64_t i = 1; i <= n; ++i) {
            powersnk[i] = powersnk[i - 1];
            if ((i & (i - 1)) == i - 1) {
                powersnk[i] += 1;
            }
        }
        powerskn = std::vector<int64_t>(powersnk[n] + 1);
        powerskn[0] = 1;
        for (int i = 1; i <= powersnk[n]; ++i) {
            powerskn[i] = powerskn[i - 1] * 2;
        }
        size_t cnts[128];
        for (int i = 0; i < 128; ++i) {
            cnts[i] = 0;
        }
        for (int64_t i = 0; i < n; ++i) {
            ++cnts[data[i]];
        }
        for (int i = 1; i < 128; ++i) {
            cnts[i] += cnts[i - 1];
        }
        for (int64_t i = n - 1; i >= 0; --i) {
            P[--cnts[data[i]]] = i;
        }
        C[0] = 0;
        for (int64_t i = 1; i < n; ++i) {
            if (data[P[i]] == data[P[i - 1]]) {
                C[P[i]] = C[P[i - 1]];
            } else {
                C[P[i]] = C[P[i - 1]] + 1;
            }
        }
        std::vector<size_t> temp_P(n);
        for (int i = 0; powerskn[i] < n; ++i) {
            size_t cnt[n];
            for (int j = 0; j < n; ++j) {
                cnt[j] = 0;
            }
            for (int64_t j = 0; j < n; ++j) {
                ++cnt[C[(P[j] + powerskn[i]) % n]];
            }
            for (int64_t j = 1; j < n; ++j) {
                cnt[j] += cnt[j - 1];
            }

            for (int64_t j = n - 1; j >= 0; --j) {
                temp_P[--cnt[C[(P[j] + powerskn[i]) % n]]] = P[j];
            }
            P = temp_P;
            std::vector<std::vector<size_t>> P_temp(n);
            std::vector<size_t> C_temp(n);
            for (int64_t j = 0; j < n; ++j) {
                P_temp[C[P[j]]].push_back(P[j]);
            }
            int64_t count = 0;
            for (int j = 0; j < n; ++j) {
                for (int64_t k = 0; k < P_temp[j].size(); ++k) {
                    P[count++] = P_temp[j][k];
                }
            }
            C_temp[P[0]] = 0;
            for (int64_t j = 1; j < n; ++j) {
                if (C[P[j]] == C[P[j - 1]] && C[(P[j] + powerskn[i]) % n] == C[(P[j - 1] + powerskn[i]) % n]) {
                    C_temp[P[j]] = C_temp[P[j - 1]];
                } else {
                    C_temp[P[j]] = C_temp[P[j - 1]] + 1;
                }
            }
            C = C_temp;
        }
    }

    void print_P() {
        for (int i = 1; i < n; ++i) {
            std::cout << P[i] + 1 << " ";
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string s;
    std::cin >> s;
    s += "#";
    SuffixArray suf = SuffixArray(std::move(s));
    suf.print_P();
    return 0;
}

