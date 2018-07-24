#include <cstdio>
#include <cstdint>

#include "config.hpp"
#include "FreeRTOS.h"
#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/debug_print.hpp"
#include "L2_Utilities/rtos.hpp"
#include "task.h"

namespace
{

void vLedToggle(void * pvParameters)
{
    DEBUG_PRINT("Setting up task...");
    DEBUG_PRINT("Retrieving delay amount from pvParameters...");
    auto delay = Rtos::RetrieveParameter(pvParameters);
    DEBUG_PRINT("Initializing LEDs...");
    LPC_IOCON->P1_1 &= ~(0b111);
    LPC_IOCON->P1_8 &= ~(0b111);
    LPC_GPIO1->DIR |= (1 << 1);
    LPC_GPIO1->PIN &= ~(1 << 1);
    LPC_GPIO1->DIR |= (1 << 8);
    LPC_GPIO1->PIN |= (1 << 8);
    DEBUG_PRINT("LEDs Initialized...");
    DEBUG_PRINT("Toggling LEDs...");
    // Loop blinks the LEDs back and forth at a rate that depends on the
    // pvParameter's value.
    while (true)
    {
        LPC_GPIO1->PIN ^= 0b0001'0000'0010;
        vTaskDelay(delay);
    }
}

constexpr uint32_t kButtonPinNumber = 14;
constexpr uint32_t kLedNumber       = 15;

bool CheckSwitch3()
{
    return (LPC_GPIO1->PIN & (1 << kButtonPinNumber));
}

void vButtonReader(void * pvParameters)
{
    SJ2_USED(pvParameters);
    DEBUG_PRINT("Setting up task...");
    DEBUG_PRINT("Initializing LED3 and SW3...");
    LPC_IOCON->P1_14 &= ~(0b111);
    LPC_IOCON->P1_15 &= ~(0b111);
    LPC_GPIO1->DIR &= ~(1 << kButtonPinNumber);
    LPC_GPIO1->DIR |= (1 << kLedNumber);
    LPC_GPIO1->PIN |= (1 << kLedNumber);
    DEBUG_PRINT("LED3 and SW3 Initialized...");
    DEBUG_PRINT("Press and release SW3 to toggle LED3 state...");
    bool button_pressed = false;
    // Loop detects when the button has been released and changes the LED state
    // accordingly.
    while (true)
    {
        if (CheckSwitch3())
        {
            button_pressed = true;
        }
        else if (!CheckSwitch3() && button_pressed)
        {
            LPC_GPIO1->PIN ^= (1 << kLedNumber);
            button_pressed = false;
        }
        else
        {
            button_pressed = false;
        }
        vTaskDelay(50);
    }
}

}  // namespace

int main(void)
{
    TaskHandle_t xHandle = NULL;
    DEBUG_PRINT("Creating Tasks ...");
    // See https://www.freertos.org/a00125.html for the xTaskCreate API
    // See L2_Utilities/rtos.hpp for the Rtos:: namespace utility functions
    xTaskCreate(
        vLedToggle,                // Make function vLedToggle a task
        "vLedToggle",              // Give this task the name "vLedToggle"
        Rtos::StackSize(256),      // Size of stack allocated to task
        Rtos::PassParameter(100),  // Parameter to be passed to task
        Rtos::Priority::kLow,      // Give this task low priority
        &xHandle);                 // Reference to the task
    xTaskCreate(
        vButtonReader,             // Make function vButtonReader a task
        "vButtonReader",           // Give this task the name "vButtonReader"
        Rtos::StackSize(256),      // Size of stack allocated to task
        Rtos::kNoParameter,        // Pass nothing to this task
        Rtos::Priority::kMedium,   // Give this task medium priority
        Rtos::kNoHandle);          // Do not supply a task handle
    DEBUG_PRINT("Starting Scheduler ...");
    vTaskStartScheduler();
    return 0;
}
