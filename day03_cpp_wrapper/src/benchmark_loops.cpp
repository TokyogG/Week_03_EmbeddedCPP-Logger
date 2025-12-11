#include <chrono>
#include <iostream>

int main() {
    const int ITER = 1'000'000;
    volatile int sink = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITER; ++i) {
        sink++;
    }
    auto end = std::chrono::high_resolution_clock::now();

    double ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double per_iter = ns / ITER;

    std::cout << "Empty loop: " << per_iter << " ns/iter\n";
    return 0;
}
