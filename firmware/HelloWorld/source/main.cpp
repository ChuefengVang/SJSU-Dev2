#include <project_config.hpp>

#include <cstdint>
#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/debug_print.hpp"
#include <inttypes.h>

#include "L0_LowLevel/startup.hpp"
#include "L1_Drivers/i2c.hpp"

constexpr uint8_t kFirstI2cAddress = 0x08;
constexpr uint8_t kLastI2cAddress  = 0x78;

constexpr uint8_t kAccelerometerAddress = 0x48;
uint8_t initialization_sequence[]       = { 0x2A, 0x01 };
uint8_t byte                            = 0x0D;

I2c i2c(I2c::Port::kI2c2);

volatile uint32_t cycles = 500'000;

int main(void)
{
#if SJ2_ENABLE_ANSI_CODES
    // Clears the terminal
    fputs("\e[2J\e[H", stdout);
#endif
    fputs(SJ2_BACKGROUND_GREEN
         "================================== SJTwo Booted! "
         "==================================\n" SJ2_COLOR_RESET, stdout);
    DEBUG_PRINT("Initializing LEDs...");
    LPC_IOCON->P1_1 &= ~(0b111);
    LPC_IOCON->P1_8 &= ~(0b111);
    LPC_GPIO1->DIR |= (1 << 1);
    LPC_GPIO1->PIN &= ~(1 << 1);
    LPC_GPIO1->DIR |= (1 << 8);
    LPC_GPIO1->PIN |= (1 << 8);
    DEBUG_PRINT("LEDs Initialized...");
    fputs("Enter wait cycles for led animation: ", stdout);
    scanf("%lu", &cycles);
    DEBUG_PRINT("Toggling LEDs...");

    // while (1)
    // {
        // for (uint32_t i = 0; i < cycles; i++) { continue; }
        // LPC_GPIO1->PIN ^= 0b0001'0000'0010;
    // }
    DEBUG_PRINT("I2C Application Starting...");
    DEBUG_PRINT(
        "This example will scan I2C Bus 2 for any devices. If the transaction "
        "comes back with I2cInterface::Status::kSuccess, then we know that an "
        "I2C device has acknowledged our call and it exists.");

    DEBUG_PRINT("Initializing I2C Port 2...");
    i2c.Initialize();
    DEBUG_PRINT("Initializing Onboard Accelerometer using I2C.2...");
    i2c.Write(kAccelerometerAddress, initialization_sequence,
              sizeof(initialization_sequence));
    return 0;
}
