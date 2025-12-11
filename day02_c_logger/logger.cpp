#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "mpu_registers.h"

bool i2c_write_reg(int fd, uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {reg, val};
    return write(fd, buf, 2) == 2;
}

bool i2c_read_bytes(int fd, uint8_t reg, uint8_t *buf, size_t len)
{
    if (write(fd, &reg, 1) != 1) return false;
    return read(fd, buf, len) == (int)len;
}

int16_t to_int16(uint8_t high, uint8_t low)
{
    return (int16_t)((high << 8) | low);
}

int main()
{
    std::cout << "Opening I2C..." << std::endl;
    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) { std::cerr << "Failed to open I2C\n"; return 1; }

    if (ioctl(fd, I2C_SLAVE, MPU_ADDR) < 0) {
        std::cerr << "Failed to connect to device\n";
        return 1;
    }

    // Wake up MPU6050
    i2c_write_reg(fd, REG_PWR_MGMT_1, 0x00);
    usleep(10000);

    std::ofstream csv("cpp_mpu_log.csv");
    csv << "timestamp,acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z,temp_c\n";

    auto start = std::chrono::steady_clock::now();
    auto end_at = start + std::chrono::seconds(10);

    std::cout << "Logging for 10 seconds at ~50 Hz..." << std::endl;

    while (std::chrono::steady_clock::now() < end_at)
    {
        uint8_t data[14];
        if (!i2c_read_bytes(fd, REG_ACCEL_XOUT_H, data, 14))
        {
            std::cerr << "I2C read failed!\n";
            continue;
        }

        int16_t ax = to_int16(data[0], data[1]);
        int16_t ay = to_int16(data[2], data[3]);
        int16_t az = to_int16(data[4], data[5]);
        int16_t temp = to_int16(data[6], data[7]);
        int16_t gx = to_int16(data[8], data[9]);
        int16_t gy = to_int16(data[10], data[11]);
        int16_t gz = to_int16(data[12], data[13]);

        float temp_c = temp / 340.0f + 36.53f;

        auto ts = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - start).count();

        csv << ts << "," << ax << "," << ay << "," << az << ","
            << gx << "," << gy << "," << gz << "," << temp_c << "\n";

        usleep(20000); // 50 Hz
    }

    csv.close();
    close(fd);

    std::cout << "Done. Data saved to cpp_mpu_log.csv\n";

    // --------------------------------------
    // Benchmark: empty loop timing
    // --------------------------------------
    const int N = 1000000;  // 1 million iterations

    auto b0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        asm volatile(""); // prevents compiler optimization
    }
    auto b1 = std::chrono::high_resolution_clock::now();

    auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(b1 - b0).count();
    double per_iter = (double)total_ns / N;

    std::cout << "---- C++ Empty Loop Benchmark ----\n";
    std::cout << "Total time: " << total_ns << " ns for " << N << " iterations\n";
    std::cout << "Per iteration: " << per_iter << " ns\n";

    return 0;
}