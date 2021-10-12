#include <iostream>
#include <string>
#include <vector>

class KMP {
private:
    std::string data;
    std::vector<int32_t> prefixes;
    size_t patternLength;

public:
    explicit KMP(const std::string &data, size_t patternLength) : data(data), prefixes(data.size()),
                                                                  patternLength(patternLength) {}

    void prefix_function() {
        for (size_t i = 1; i < prefixes.size(); ++i) {
            int32_t j = prefixes[i - 1];
            while (j > 0 && data[j] != data[i]) {
                j = prefixes[j - 1];
            }
            if (data[j] == data[i]) {
                ++j;
            }
            prefixes[i] = j;
        }
    }

    void kmp() {
        for (size_t i = patternLength + 1; i < prefixes.size(); ++i) {
            if (prefixes[i] == patternLength) {
                std::cout << i - patternLength - patternLength << ' ';
            }
        }
    }
};

int main() {
    std::string s;
    std::string t;
    std::cin >> s >> t;
    KMP kmp = KMP(t + '#' + s, t.size());
    kmp.prefix_function();
    kmp.kmp();
    return 0;
}
