// struct_lab.cpp
#include <cstdio>   // printf
#include <cstddef>  // offsetof
#include <cstdint>  // int16_t, uint8_t, etc.

// Simulated sensor reading (like a tiny MPU packet)
struct SensorReading {
    int16_t accel_x;     // 2 bytes
    int16_t accel_y;     // 2 bytes
    int16_t accel_z;     // 2 bytes
    float   temperature; // 4 bytes
};

int main() {
    printf("=== Struct layout demo ===\n");

    printf("sizeof(SensorReading) = %zu bytes\n", sizeof(SensorReading));
    printf("offset of accel_x     = %zu\n", offsetof(SensorReading, accel_x));
    printf("offset of accel_y     = %zu\n", offsetof(SensorReading, accel_y));
    printf("offset of accel_z     = %zu\n", offsetof(SensorReading, accel_z));
    printf("offset of temperature = %zu\n\n", offsetof(SensorReading, temperature));

    // Create an instance and assign fields
    SensorReading s;
    s.accel_x = 100;
    s.accel_y = -200;
    s.accel_z = 300;
    s.temperature = 23.5f;

    printf("Values:\n");
    printf("  accel_x = %d\n", s.accel_x);
    printf("  accel_y = %d\n", s.accel_y);
    printf("  accel_z = %d\n", s.accel_z);
    printf("  temperature = %.2f\n\n", s.temperature);

    // View memory as raw bytes
    unsigned char* raw = reinterpret_cast<unsigned char*>(&s);
    printf("Raw bytes (hex):\n  ");
    for (size_t i = 0; i < sizeof(SensorReading); ++i) {
        printf("%02X ", raw[i]);
    }
    printf("\n");

    return 0;
}