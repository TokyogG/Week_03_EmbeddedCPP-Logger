#include "logger.hpp"
#include <chrono>
#include <iostream>

void init_logger() {
    std::cout << "Logger initialized\n";
}

int read_sample() {
    // Dummy sensor value
    return 42;
}

void run_benchmark() {
    constexpr int N = 1'000'000;
    auto start = std::chrono::high_resolution_clock::now();

    volatile int x = 0;
    for (int i = 0; i < N; ++i) {
        x += i;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "C++ loop: " << ns / N << " ns / iteration\n";
}