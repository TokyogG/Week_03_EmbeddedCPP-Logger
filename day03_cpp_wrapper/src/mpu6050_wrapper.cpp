#include "mpu6050_wrapper.h"

#include <chrono>
#include <iostream>
#include <fcntl.h>      // open
#include <unistd.h>     // read, write, close, usleep
#include <sys/ioctl.h>  // ioctl
#include <linux/i2c-dev.h> // I2C_SLAVE

// MPU6050 I2C address and registers (standard)
static constexpr int MPU_ADDR = 0x68;
static constexpr int PWR_MGMT_1 = 0x6B;
static constexpr int ACCEL_XOUT_H = 0x3B;
static constexpr int TEMP_OUT_H   = 0x41;
static constexpr int GYRO_XOUT_H  = 0x43;

// Sensitivity scale factors (assuming +/- 2g, +/- 250 deg/s)
static constexpr float ACCEL_SCALE = 16384.0f; // LSB/g
static constexpr float GYRO_SCALE  = 131.0f;   // LSB/(deg/s)
static constexpr float TEMP_OFFSET = 36.53f;   // From datasheet
static constexpr float TEMP_SCALE  = 340.0f;   // LSB/degC

Mpu6050::Mpu6050()
    : fd_(-1),
      start_time_(std::chrono::steady_clock::now())
{}

bool Mpu6050::init() {
    // Open I2C bus
    fd_ = open("/dev/i2c-1", O_RDWR);
    if (fd_ < 0) {
        std::cerr << "Failed to open /dev/i2c-1\n";
        return false;
    }

    // Set slave address
    if (ioctl(fd_, I2C_SLAVE, MPU_ADDR) < 0) {
        std::cerr << "Failed to set I2C slave address 0x"
                  << std::hex << MPU_ADDR << std::dec << "\n";
        return false;
    }

    // Wake up the MPU6050 by clearing sleep bit
    unsigned char buf[2];
    buf[0] = PWR_MGMT_1;
    buf[1] = 0x00;
    if (write(fd_, buf, 2) != 2) {
        std::cerr << "Failed to write PWR_MGMT_1\n";
        return false;
    }

    start_time_ = std::chrono::steady_clock::now();
    return true;
}

int16_t Mpu6050::read_word_2c(int reg) {
    unsigned char reg_buf[1];
    reg_buf[0] = static_cast<unsigned char>(reg);

    // Write register address
    if (write(fd_, reg_buf, 1) != 1) {
        std::cerr << "Failed to write register " << reg << "\n";
        return 0;
    }

    // Read two bytes
    unsigned char data[2];
    if (read(fd_, data, 2) != 2) {
        std::cerr << "Failed to read data from reg " << reg << "\n";
        return 0;
    }

    int16_t value = (data[0] << 8) | data[1];
    return value;
}

bool Mpu6050::read_sample(Mpu6050Sample &out) {
    if (fd_ < 0) {
        std::cerr << "MPU6050 not initialized.\n";
        return false;
    }

    // Read raw values
    int16_t accel_x_raw = read_word_2c(ACCEL_XOUT_H);
    int16_t accel_y_raw = read_word_2c(ACCEL_XOUT_H + 2);
    int16_t accel_z_raw = read_word_2c(ACCEL_XOUT_H + 4);

    int16_t temp_raw = read_word_2c(TEMP_OUT_H);

    int16_t gyro_x_raw = read_word_2c(GYRO_XOUT_H);
    int16_t gyro_y_raw = read_word_2c(GYRO_XOUT_H + 2);
    int16_t gyro_z_raw = read_word_2c(GYRO_XOUT_H + 4);

    // Time since logger start
    auto now = std::chrono::steady_clock::now();
    double ts = std::chrono::duration<double>(now - start_time_).count();

    // Convert to physical units
    out.timestamp_s = ts;
    out.accel_x_g   = static_cast<float>(accel_x_raw) / ACCEL_SCALE;
    out.accel_y_g   = static_cast<float>(accel_y_raw) / ACCEL_SCALE;
    out.accel_z_g   = static_cast<float>(accel_z_raw) / ACCEL_SCALE;

    out.gyro_x_dps  = static_cast<float>(gyro_x_raw) / GYRO_SCALE;
    out.gyro_y_dps  = static_cast<float>(gyro_y_raw) / GYRO_SCALE;
    out.gyro_z_dps  = static_cast<float>(gyro_z_raw) / GYRO_SCALE;

    out.temp_c      = (static_cast<float>(temp_raw) / TEMP_SCALE) + TEMP_OFFSET;

    return true;
}
