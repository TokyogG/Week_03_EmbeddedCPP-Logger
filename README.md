# **Week 03 — Embedded C & Modern C++ Logger (Raspberry Pi 5)**

*Part of the 16-Week Edge AI Engineering Bootcamp*

This week transitions from Python-based data pipelines to **embedded-grade C and modern C++ development**.
You build a complete real-time logger for the MPU6050 IMU sensor, benchmark it against Python, and establish the Python ↔ C++ integration pattern used by real-world edge AI systems.

---

## 🚀 **Weekly Objectives**

By the end of Week 03, you should be able to:

### **Embedded C**
- Understand stack vs heap, alignment, pointers, and structs
- Use `volatile` correctly for memory-mapped I/O
- Organize multi-file C projects (`.h` / `.c`)
- Communicate with sensors using raw Linux I²C (`/dev/i2c-1`)

### **Modern C++**
- Wrap C drivers using RAII principles
- Build modular class interfaces (`.h` / `.cpp`)
- Use CMake for multi-target builds
- Reason about abstraction cost vs performance

### **Performance Engineering**
- Compare Python ↔ C ↔ C++ latency
- Write nano-scale microbenchmarks
- Interpret compiler optimizations (`-O0`, `-O2`, `-O3`)
- Measure instead of assuming performance characteristics

---

## 📁 **Folder Structure**

Week_03_EmbeddedCPP-Logger/
│
├── day01_intro/
│ ├── pointer_lab.cpp
│ ├── struct_lab.cpp
│ ├── fake_register.cpp
│
├── day02_c_logger/
│ ├── mpu6050.c
│ ├── mpu6050.h
│ ├── logger.c
│ ├── Makefile
│
├── day03_cpp_wrapper/
│ └── src/
│ ├── main_mpu_logger.cpp
│ ├── mpu6050_wrapper.cpp
│ ├── mpu6050_wrapper.h
│ ├── benchmark_loops.cpp
│ ├── benchmark_arrays.cpp
│ ├── benchmark_vectors.cpp
│ ├── inline_vs_noinline.cpp
│ └── CMakeLists.txt
│
├── day04_cmsis_nn/
│
└── day05_pybind11/


---

## 🟦 **Day 01 — C Foundations**

### Focus
- Stack vs heap memory
- Struct layout and alignment
- Pointer arithmetic
- `volatile` and simulated hardware registers

### Labs
- `pointer_lab.cpp`
- `struct_lab.cpp`
- `fake_register.cpp`

**Outcome:**  
A concrete mental model of how C variables map directly into memory — the foundation required for embedded systems.

---

## 🟧 **Day 02 — MPU6050 Logger in C**

### What was built
- Raw I²C MPU6050 driver using:
  - `open("/dev/i2c-1")`
  - `ioctl(fd, I2C_SLAVE, 0x68)`
  - Manual register reads/writes
- 50 Hz real-time CSV logger
- Baseline performance measurement

### Performance Comparison (Raspberry Pi 5)

| Version     | Latency / Loop | Notes                |
|------------|----------------|----------------------|
| Python 3.11 | 2–3 ms         | Interpreter overhead |
| **C**       | **~0.8 ms**    | ~3× faster           |

---

## 🟩 **Day 03 — C++ Wrapper & Microbenchmarks**

Converted the C driver into a reusable C++ class and introduced nano-scale benchmarking.

### Key Concepts
- RAII for managing `/dev/i2c-1`
- Clean class interfaces vs global functions
- `std::chrono` for high-resolution timing
- Measuring abstraction cost

### Benchmark Results (Pi 5)

Empty loop: ~3.4 ns / iter
Raw array sum: ~3.4 ns / iter
Vector sum: ~3.4 ns / iter
Inline add: ~3 ns / iter
No-inline add: ~2 ns / iter


### Takeaways
- Modern compilers aggressively optimize
- Intuition is unreliable at nanosecond scale
- C++ abstractions add negligible overhead when designed correctly

---

## 🟪 **Day 04 — CMSIS-NN & DSP Acceleration**

Explored optimized convolution kernels relevant to MCU-level ML.

### Setup
- int8 3×3 convolution (HWC)
- Cin = 8, Cout = 8
- Proper per-channel requantization (Q31)

### Results

| Kernel | Time / iter | Speedup |
|------|-------------|---------|
| Naive C++ | ~794 µs | 1.0× |
| CMSIS-NN (`arm_convolve_wrapper_s8`) | ~424 µs | ~1.87× |

### Notes
- CMSIS-NN always applies requantization
- Identity scaling requires Q31 multiplier (`0x7fffffff`)
- After alignment, CMSIS-NN and naive outputs match exactly
- Even on Cortex-A (Pi), CMSIS-NN delivers meaningful speedups

---

## 🟥 **Day 05 — pybind11 (Python ↔ C++ Integration)**

Integrated the optimized C++ logger into Python using **pybind11**, establishing the same architectural boundary used by:

- NumPy
- PyTorch
- ONNX Runtime
- TensorRT / HailoRT Python APIs

### What was achieved
- Built a native C++ shared library callable from Python
- Validated end-to-end execution on Raspberry Pi 5
- Demonstrated Python orchestration with C++ execution

### Benchmark
- C++ loop benchmark via pybind11: **~3 ns / iteration**
- Python import required `PYTHONPATH=.` unless installed or copied

**Outcome:**  
A production-realistic hybrid Python/C++ workflow suitable for edge AI systems.

---

## ✅ Week 03 Outcome

By the end of this week, you can:
- Write and reason about embedded C
- Design modern, efficient C++ interfaces
- Benchmark at nanosecond scale
- Integrate Python and C++ cleanly
- Prepare for MCU deployment and TinyML workflows later in the bootcamp
