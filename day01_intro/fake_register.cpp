// fake_register.cpp
#include <cstdio>
#include <cstdint>
#include <chrono>
#include <thread>

// Simulate a hardware register mapped at some fixed address.
// In real embedded, you'd have something like:
//   #define GPIO_PORTA (*(volatile uint32_t*)0x40020000)
volatile uint32_t FAKE_REGISTER = 0;

int main() {
    printf("=== Fake hardware register demo ===\n");
    printf("Initial FAKE_REGISTER = %u\n\n", FAKE_REGISTER);

    for (int i = 0; i < 10; ++i) {
        // Write a new "value" to the register
        FAKE_REGISTER = i;

        // Read it back
        uint32_t value = FAKE_REGISTER;

        printf("Loop %d: wrote %d, read back %u\n", i, i, value);

        // Sleep a bit so you can see it change
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    printf("\nDone.\n");
    return 0;
}