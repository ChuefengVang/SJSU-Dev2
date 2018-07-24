// This file contains helper and utility functions, objects and types for
// FreeRTOS.
#pragma once

namespace Rtos
{

enum Priority
{
    kIdle = tskIDLE_PRIORITY,
    kLow,
    kMedium,
    kHigh,
    kCritical
};

constexpr void * kNoParameter = nullptr;
constexpr void ** kNoHandle = nullptr;

// Calculates and returns the stack size of a task. This will add in the minimum
// needed stack size for a task.
constexpr size_t StackSize(size_t stack_size_bytes)
{
    return configMINIMAL_STACK_SIZE + (stack_size_bytes / sizeof(StackType_t));
}

template<typename T>
constexpr void * PassParameter(T t)
{
    return reinterpret_cast<void *>(t);
}

template<typename T = int32_t>
constexpr T RetrieveParameter(void * parameter)
{
    return reinterpret_cast<T>(parameter);
}

}  // namespace Rtos
