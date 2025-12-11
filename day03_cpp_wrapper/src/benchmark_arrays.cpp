#include <chrono>
#include <iostream>

int main() {
    const int N = 2048;
    static int arr[N];

    for (int i = 0; i < N; ++i) arr[i] = i;

    volatile long sum = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        sum += arr[i];
    }
    auto end = std::chrono::high_resolution_clock::now();

    double ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double per_iter = ns / N;

    std::cout << "Raw array sum: " << per_iter << " ns/iter\n";
    return 0;
}
