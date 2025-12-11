#include "mpu6050_wrapper.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char** argv) {
    std::string filename = "cpp_mpu_wrapper_log.csv";
    if (argc > 1) {
        filename = argv[1];
    }

    std::cout << "Initializing MPU6050 via wrapper...\n";
    Mpu6050 mpu;
    if (!mpu.init()) {
        std::cerr << "Failed to initialize MPU6050.\n";
        return 1;
    }

    const double duration_s = 10.0;
    const double sample_rate_hz = 50.0;
    const double sample_period_s = 1.0 / sample_rate_hz;
    const int expected_samples = static_cast<int>(duration_s * sample_rate_hz);

    std::vector<Mpu6050Sample> samples;
    samples.reserve(expected_samples + 10);

    std::cout << "Logging for " << duration_s
              << " seconds at ~" << sample_rate_hz << " Hz...\n";

    auto start = std::chrono::steady_clock::now();
    while (true) {
        auto loop_start = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(loop_start - start).count();
        if (elapsed >= duration_s) {
            break;
        }

        Mpu6050Sample s;
        if (mpu.read_sample(s)) {
            samples.push_back(s);
        } else {
            std::cerr << "Warning: Failed to read sample.\n";
        }

        auto loop_end = std::chrono::steady_clock::now();
        double loop_time = std::chrono::duration<double>(loop_end - loop_start).count();
        double sleep_time = sample_period_s - loop_time;
        if (sleep_time > 0) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(sleep_time)
            );
        }
    }

    std::cout << "Done. Collected " << samples.size() << " samples.\n";

    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open " << filename << " for writing.\n";
        return 1;
    }

    out << "timestamp_s,accel_x_g,accel_y_g,accel_z_g,"
           "gyro_x_dps,gyro_y_dps,gyro_z_dps,temp_c\n";
    for (const auto &s : samples) {
        out << s.timestamp_s << ","
            << s.accel_x_g << ","
            << s.accel_y_g << ","
            << s.accel_z_g << ","
            << s.gyro_x_dps << ","
            << s.gyro_y_dps << ","
            << s.gyro_z_dps << ","
            << s.temp_c << "\n";
    }

    std::cout << "Data saved to " << filename << "\n";
    return 0;
}