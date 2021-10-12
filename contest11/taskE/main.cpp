#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <queue>

class Trie {
private:
    struct Node {
        int32_t to[26];
        bool term = false;
        int32_t link = -1;

        std::vector<int32_t> indexes_of_words;
        int32_t count_of_letters;

        Node() {
            std::memset(to, -1, sizeof(to));
        }
    };
    std::vector<Node> trie;
    int32_t n;

public:
    Trie(int32_t n): n(n) {
        trie.emplace_back(Node());
    }

    void add(std::string&& s, int32_t index) {
        int32_t v = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (trie[v].to[s[i] - 'a'] == -1) {
                trie.emplace_back(Node());
                trie[v].to[s[i] - 'a'] = static_cast<int32_t>(trie.size()) - 1;
            }
            v = trie[v].to[s[i] - 'a'];
        }
        trie[v].term = true;
        trie[v].indexes_of_words.push_back(index);
        trie[v].count_of_letters = s.size();
    }

    void aho_corasick() {
        trie[0].link = 0;
        for (int32_t c = 0; c < 26; ++c) {
            if (trie[0].to[c] != -1) {
                continue;
            }
            trie[0].to[c] = 0;
        }
        std::queue<int32_t> q;
        q.push(0);
        while (!q.empty()) {
            int32_t v = q.front();
            q.pop();
            for (int32_t c = 0; c < 26; ++c) {
                int32_t u = trie[v].to[c];
                if (trie[u].link != -1) {
                    continue;
                }
                trie[u].link = (v == 0 ? 0 : trie[trie[v].link].to[c]);
                for (int32_t d = 0; d < 26; ++d) {
                    if (trie[u].to[d] != -1) {
                        continue;
                    }
                    trie[u].to[d] = trie[trie[u].link].to[d];
                }
                q.push(u);
            }
        }
    }

    void find_words_in_string(std::string&& s) {
        std::vector<std::vector<int32_t>> entries(n);
        int32_t v = 0;
        for (int32_t i = 0; i < s.size(); ++i) {
            v = trie[v].to[s[i] - 'a'];
            if (v == -1) {
                v = 0;
                continue;
            }
            int32_t newv = v;
            while (newv > 0) {
                if (trie[newv].term) {
                    for (auto index: trie[newv].indexes_of_words) {
                        entries[index].push_back(i - trie[newv].count_of_letters + 2);
                    }
                }
                newv = trie[newv].link;
            }
        }
        for (int32_t i = 0; i < n; ++i) {
            std::cout << entries[i].size() << ' ';
            for (int32_t j = 0; j < entries[i].size(); ++j) {
                std::cout << entries[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }
};

int main() {
    std::string s;
    std::cin >> s;
    int32_t n;
    std::cin >> n;
    Trie trie = Trie(n);
    for (int32_t i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        trie.add(std::move(word), i);
    }
    trie.aho_corasick();
    trie.find_words_in_string(std::move(s));
    return 0;
}
