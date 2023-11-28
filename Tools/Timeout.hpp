/**
 * @file        Timeout.hpp
 * @author      CodeDog
 * @brief       A simple timeout feature implementation using FreeRTOS threads. Header file.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "Delay.hpp"

/**
 * @brief A simple implementation of a timeout feature.
 */
class Timeout
{

public:

    /// @brief Defines a timeout. Doesn't start the timer.
    /// @param seconds A time amount in seconds before the action is called. Accepts fractional values.
    /// @param action An action to be called when the time elapses.
    Timeout(double seconds, Action action);

    Timeout(const Timeout& other) = delete;

    Timeout(Timeout&& other) = delete;

    /// @brief Releases all resources used by the instance.
    ~Timeout();

    /// @brief Sets the timeout. The action assigned in the constructor will be called after specified number of secods.
    void set();

    /// @brief Sets the timeout. The action assigned in the constructor will be called after specified number of secods.
    /// @param seconds New time interval value in seconds. Accepts fractional values.
    void set(double seconds);

    /// @brief Resets the time interval value, so the full interval time must be elapsed again since now.
    void reset();

    /// @brief Clears the timeout, so the action will not be called again.
    void clear();

protected:
    WaitHandle* m_waitHandle;   // Timeout wait handle pointer.
    int m_ticks;                // A time interval in OS ticks to call the associated action.
    Action m_action;            // An action to call after the timeout elapses.

};
