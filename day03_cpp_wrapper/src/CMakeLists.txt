cmake_minimum_required(VERSION 3.16)
project(day03_cpp_wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O2)

# Wrapper + logger
add_executable(mpu_logger
    mpu6050_wrapper.cpp
    main_mpu_logger.cpp
)

# Benchmarks
add_executable(benchmark_loops benchmark_loops.cpp)
add_executable(benchmark_arrays benchmark_arrays.cpp)
add_executable(benchmark_vectors benchmark_vectors.cpp)
add_executable(inline_vs_noinline inline_vs_noinline.cpp)
