---

# **Week 03 — Embedded C & Modern C++ Logger (Raspberry Pi 5)**

*Part of the 16-Week Edge AI Engineering Bootcamp*

This week transitions from Python-based data pipelines to **embedded-grade C and C++ development**.
You will build a complete real-time logger for the MPU6050 IMU sensor, benchmark it against Python, and prepare for STM32 work later in the Bootcamp.

---

## 🚀 **Weekly Objectives**

By the end of Week 03, you should be able to:

### **Embedded C**

* Understand stack vs heap, alignment, pointers, and structs
* Use `volatile` correctly for memory-mapped I/O
* Organize multi-file C projects (`.h` and `.c`)
* Communicate with sensors using raw Linux I²C (`/dev/i2c-1`)

### **Modern C++**

* Wrap C drivers using RAII principles
* Build modular class interfaces (`.h`/`.cpp`)
* Use CMake for multi-target builds
* Understand abstraction cost vs. performance

### **Performance Engineering**

* Compare Python ↔ C ↔ C++ latency
* Write nano-scale microbenchmarks
* Interpret compiler optimizations (`-O0`, `-O2`, `-O3`)

---

## 📁 **Folder Structure (Updated)**

```
Week_03_EmbeddedCPP-Logger/
│
├── day01_intro/
│   ├── pointer_lab.cpp
│   ├── struct_lab.cpp
│   ├── fake_register.cpp
│   └── Notes_Day01.md
│
├── day02_c_logger/
│   ├── mpu6050.c
│   ├── mpu6050.h
│   ├── logger.c
│   ├── Makefile
│
├── day03_cpp_wrapper/
│   └── src/
│       ├── main_mpu_logger.cpp
│       ├── mpu6050_wrapper.cpp
│       ├── mpu6050_wrapper.h
│       ├── benchmark_loops.cpp
│       ├── benchmark_arrays.cpp
│       ├── benchmark_vectors.cpp
│       ├── inline_vs_noinline.cpp
│       └── CMakeLists.txt
│
├── day04_cmsis_nn/    (placeholder)
│
└── day05_summary/     (placeholder)
```

---

# 🟦 **Day 01 — C Foundations**

### Learning Goals

* Stack vs heap memory
* Struct layout and alignment
* Pointer arithmetic
* `volatile` and simulated hardware registers

### Labs

* `pointer_lab.cpp`
* `struct_lab.cpp`
* `fake_register.cpp`

By the end of Day 01 you should be able to explain how C variables map directly into memory — the mental model required for embedded systems.

---

# 🟧 **Day 02 — Build the MPU6050 Logger in C**

### What you build

* A raw I²C MPU6050 driver using:

  * `open("/dev/i2c-1")`
  * `ioctl(fd, I2C_SLAVE, 0x68)`
  * manual register reads/writes
* A 50 Hz real-time CSV logger
* Baseline performance measurement

### Performance Comparison (Pi 5)

| Version          | Latency / Loop | Notes                 |
| ---------------- | -------------- | --------------------- |
| Python 3.11      | 2–3 ms         | interpreter overhead  |
| **C**            | **0.8 ms**     | ~3× faster            |
| **C++** (Day 03) | ~0.8–1.0 ms    | tiny wrapper overhead |

---

# 🟩 **Day 03 — C++ Wrapper + Microbenchmarks**

This day converts the C driver into a reusable C++ class and introduces nano-scale performance benchmarking.

### Key Concepts

* RAII for managing `/dev/i2c-1`
* Clean class interface vs global functions
* `std::chrono` for timing
* Understanding abstraction cost
* C++ as a thin layer over C for embedded systems

### Benchmark Outputs (Your Pi 5 Results)

```
Empty loop:       3.457 ns/iter
Raw array sum:    3.427 ns/iter
Vector sum:       3.472 ns/iter
Inline add:       3 ns/iter
No-inline add:    2 ns/iter
```

### Takeaways

* Modern compilers aggressively optimize; theoretical “slower” paths can sometimes measure faster.
* You must **measure**, not assume.
* At this scale, the difference between C and C++ abstractions is negligible — but readability improves drastically.

### Deliverables

* `mpu_logger` → writes `cpp_mpu_wrapper_log.csv`
* `benchmark_loops`
* `benchmark_arrays`
* `benchmark_vectors`
* `inline_vs_noinline`

---

# 🟪 **Day 04 — CMSIS-NN / DSP (Preview)**

You will explore an optimized convolution kernel and compare:

* naive C implementation
* manually unrolled version
* CMSIS-NN or optimized DSP version

This prepares you for MCU-level ML in Week 13.

## Day 04 – CMSIS-NN Convolution

**Setup**
- int8 3×3 convolution (HWC)
- Cin = 8, Cout = 8
- Proper per-channel requantization (Q31 multiplier)

**Results**
| Kernel | Time / iter | Speedup |
|------|-------------|---------|
| Naive C++ | ~794 µs | 1.0× |
| CMSIS-NN (arm_convolve_wrapper_s8) | ~424 µs | ~1.87× |

**Notes**
- CMSIS-NN always applies requantization; identity scaling requires Q31 multiplier (`0x7fffffff`)
- After aligning quant pipelines, CMSIS and naive outputs match exactly
- Even on Cortex-A (Pi), CMSIS-NN delivers meaningful speedups


---

# 🟥 **Day 05 — Summary & Benchmark Report**

* Create `BENCHMARKS.md`
* Compare Python → C → C++ with tables
* Final documentation polish
* Push final version to GitHub

---