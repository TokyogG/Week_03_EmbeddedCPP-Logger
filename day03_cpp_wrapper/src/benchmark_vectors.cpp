#include <vector>
#include <chrono>
#include <iostream>

int main() {
    const int N = 2048;
    std::vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i;

    volatile long sum = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        sum += v[i];
    }
    auto end = std::chrono::high_resolution_clock::now();

    double ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double per_iter = ns / N;

    std::cout << "Vector sum: " << per_iter << " ns/iter\n";
    return 0;
}
