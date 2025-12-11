#pragma once

#include <cstdint>
#include <chrono>

// Simple IMU sample container
struct Mpu6050Sample {
    double timestamp_s;    // seconds since logger start
    float accel_x_g;
    float accel_y_g;
    float accel_z_g;
    float gyro_x_dps;
    float gyro_y_dps;
    float gyro_z_dps;
    float temp_c;
};

class Mpu6050 {
public:
    // Create but do not init hardware yet
    Mpu6050();

    // Initialize I2C + sensor.
    // Returns true on success.
    bool init();

    // Read a single sample into 'out'.
    // Returns true on success.
    bool read_sample(Mpu6050Sample &out);

private:
    int fd_;  // I2C file descriptor
    std::chrono::steady_clock::time_point start_time_;

    int16_t read_word_2c(int reg);
};