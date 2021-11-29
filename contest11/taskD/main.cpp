#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
/*
class Trie {
private:
    struct Node {
        std::vector<bool> to = std::vector<bool>(26);
        bool term = false;
    };
    std::vector<Node> trie;

public:
    Trie() {
        trie.emplace_back(Node());
    }

    void add(const std::string& s) {
        int v = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (!trie[v].to[s[i] - 'a']) {
                trie.emplace_back(Node());
                trie[v].to[s[i] - 'a'] = static_cast<int64_t>(trie.size()) - 1;
            }
            v = trie[v].to[s[i] - 'a'];
        }
        trie[v].term = true;
    }
};
*/
int main() {

    std::vector<std::string> points;
    std::vector<std::string> words;
    std::string s;
    bool points_first = false;
    char c = getchar();
    while (c != '\n') {
        if (c != '.') {
            s += c;
        } else {
            if (!s.empty()) {
                words.push_back(s);
            } else {
                points_first = true;
            }
            s = "";
            while (c == '.') {
                s += c;
                c = getchar();
            }
            points.push_back(s);
            s = "";
            continue;
        }
        c = getchar();
        if (c == '\n') {
            words.push_back(s);
        }
    }
    std::sort(words.begin(), words.end());
    size_t i = 0;
    if (points_first) {
        ++i;
        std::cout << points[0];
    }

    for (const auto& word: words) {
        std::cout << word;
        if (i < points.size()) {
            std::cout << points[i];
            ++i;
        }
    }
    return 0;
}
