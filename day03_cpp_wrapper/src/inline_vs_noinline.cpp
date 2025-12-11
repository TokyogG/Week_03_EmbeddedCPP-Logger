#include <chrono>
#include <iostream>

inline int add_inline(int a, int b) { return a + b; }
__attribute__((noinline)) int add_noinline(int a, int b) { return a + b; }

int main() {
    const int ITER = 1'000'000;
    volatile int sink = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITER; ++i) {
        sink += add_inline(i, 2);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double inline_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITER;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITER; ++i) {
        sink += add_noinline(i, 2);
    }
    end = std::chrono::high_resolution_clock::now();
    double noinline_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITER;

    std::cout << "Inline add:   " << inline_ns   << " ns/iter\n";
    std::cout << "No-inline add:" << noinline_ns << " ns/iter\n";
    return 0;
}
