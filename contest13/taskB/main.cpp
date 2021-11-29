#include <iostream>
#include <vector>

int main() {
    int N;
    int64_t sum = 0;
    std::cin >> N;
    std::vector<int> primes;
    int mind[N + 1];
    for (int i = 2; i < N + 1; ++i) {
        mind[i] = i;
    }
    for (int k = 2; k < N + 1; ++k) {
        if (mind[k] == k) {
            primes.push_back(k);
        }
        for (int i = 0; i < primes.size() && primes[i] * k <= N && primes[i] <= mind[k]; ++i) {
            mind[primes[i] * k] = primes[i];
            sum += primes[i];
        }
    }
    std::cout << sum;
    return 0;
}
