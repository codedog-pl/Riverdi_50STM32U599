/**
 * @file        Counter.hpp
 * @author      CodeDog
 * @brief       High precision clock counter, header file.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "main.h"
#include <cstdint>

#ifndef COUNTER_TIM
#error COUNTER_TIM must be defined.
#else
extern TIM_HandleTypeDef COUNTER_TIM;
#endif
#ifndef COUNTER_1S
#error COUNTER_1S must be defined.
#endif

/// @brief High precision clock counter.
class Counter
{
public:

    Counter() = delete;
    Counter(const Counter& other) = delete;
    Counter(Counter&& other) = delete;

    /// @brief Starts the hardware timer, must be run at least a second before precise measurement can be done.
    static inline void init() { HAL_TIM_Base_Start(&COUNTER_TIM); }

    /// @returns The current number of the hardware timer ticks as a reference for the getTime() method.
    static inline uint32_t getTicks() { return COUNTER_TIM.Instance->CNT; }

    /// @param t0 Reference tick.
    /// @returns The time in seconds that elapsed since the reference tick.
    static inline double getTime(uint32_t t0) { return static_cast<uint32_t>(COUNTER_TIM.Instance->CNT - t0) / static_cast<double>(COUNTER_1S); }

    /// @brief Gets the time slices for subsequent calls.
    /// @param t0 Reference tick reference. Gets reset on each call.
    /// @returns Time elapsed (in seconds) since the `t0` or the last call.
    static double getTimeSlice(uint32_t& t0)
    {
        uint32_t tick = COUNTER_TIM.Instance->CNT;
        double time = static_cast<uint32_t>(tick - t0) / static_cast<double>(COUNTER_1S);
        t0 = tick;
        return time;
    }

};
