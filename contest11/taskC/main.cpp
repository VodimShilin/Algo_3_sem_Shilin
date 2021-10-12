#include <iostream>
#include <string>
#include <vector>
#include <list>

class Trie {
private:
    struct Node {
        std::vector<int64_t> to = std::vector<int64_t>(26, 0);
        bool term = false;
        std::list<int64_t> dna_ended_in_curr; // pos_in_d
    };
    std::vector<Node> trie;
    std::vector<std::pair<const std::string, int64_t>>& D;
public:
    Trie(std::vector<std::pair<const std::string, int64_t>>& D): D(D) {
        trie.emplace_back(Node());
    }

    std::vector<int64_t> add(std::string&& s) {
        int64_t v = 0;
        for (int64_t i = 0; i < s.size(); ++i) {
            if (!trie[v].to[s[i] - 'a']) {
                trie.emplace_back(Node());
                trie[v].to[s[i] - 'a'] = static_cast<int64_t>(trie.size()) - 1;
                update_v(v, s[i]);
            }
            v = trie[v].to[s[i] - 'a'];
        }
        trie[v].term = true;
        std::vector<int64_t> ended_after_this_g;
        for (auto it = trie[v].dna_ended_in_curr.begin(); it != trie[v].dna_ended_in_curr.end(); ++it) {
            if (find(*it, v)) {
                ended_after_this_g.push_back(*it);
            }
        }
        return ended_after_this_g;
    }

    void update_v(int64_t pos, char c) {
        for (auto it = trie[pos].dna_ended_in_curr.begin(); it != trie[pos].dna_ended_in_curr.end();) {
            auto newit = it;
            it = ++it;
            ++D[*newit].second;
            trie[trie[pos].to[c - 'a']].dna_ended_in_curr.emplace_back(*newit);
            trie[pos].dna_ended_in_curr.erase(newit);
        }
    }

    bool find(int64_t pos_in_D, int64_t v = 0) {
        while (D[pos_in_D].second != D[pos_in_D].first.size()) {
            if (trie[v].term) {
                v = 0;
            } else if (trie[v].to[D[pos_in_D].first[D[pos_in_D].second] - 'a']) {
                v = trie[v].to[D[pos_in_D].first[D[pos_in_D].second] - 'a'];
                ++D[pos_in_D].second;
            } else {
                break;
            }
        }
        if (trie[v].term) {
            v = 0;
        }
        if (D[pos_in_D].second == D[pos_in_D].first.size()) {
            if (!v) {
                return true;
            } else {
                return false;
            }
        } else {
            trie[v].dna_ended_in_curr.emplace_back(pos_in_D);
            return false;
        }
    }
};

class Decoder {
private:
    std::vector<std::pair<const std::string, int64_t>> D; //+last_watched_symbol
    Trie G;
    int64_t k = 0;

    std::string make_s(std::string&& x) {
        std::string s = std::string(" ", x.size());
        for (int64_t i = 0; i < x.size(); ++i) {
            s[i] = x[(k + i) % x.size()];
        }
        return s;
    }

public:
    Decoder(): D(), G(D) {}

    void add_g(std::string&& x) {
        std::string g = make_s(std::move(x));
        std::vector<int64_t> ended_after_this_g = G.add(std::move(g));
        k = ended_after_this_g.size();
        std::cout << k << ' ';
        for (auto x: ended_after_this_g) {
            std::cout << x + 1 << ' ';
        }
        std::cout << '\n';
    }


    void add_d(std::string&& x) {
        int64_t size_D = D.size();
        D.emplace_back(make_s(std::move(x)), 0);
        if (G.find(size_D)) {
            k = 1;
            std::cout << 1 << ' ' << size_D + 1 << '\n';
        } else {
            k = 0;
            std::cout << 0 << '\n';
        }
    }
};

int main() {
    Decoder decoder;
    int64_t n;
    std::cin >> n;
    for (int64_t i = 0; i < n; ++i) {
        std::getchar();
        char c = std::getchar();
        std::getchar();
        std::string s;
        std::cin >> s;
        if (c == '?') {
            decoder.add_d(std::move(s));
        } else {
            decoder.add_g(std::move(s));
        }
    }

}
