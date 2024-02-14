/**
 * @file        Timeout.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A scheduled task wrapper to be used for resettable and cancellable timeouts. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "RTOS.hpp"
#include "Action.hpp"

namespace OS
{

/// @brief A scheduled task wrapper to be used for resettable and cancellable timeouts.
class Timeout
{

public:

    /// @brief Defines a timeout. Doesn't start the timer.
    /// @param seconds A time amount in seconds before the action is called. Accepts fractional values.
    /// @param action An action to be called when the time elapses.
    Timeout(double seconds, Action action);

    /// @brief Defines a timeout. Doesn't start the timer.
    /// @param seconds A time amount in seconds before the action is called. Accepts fractional values.
    /// @param arg A pointer that will be passed as the action argument.
    /// @param action An action to be called when the time elapses.
    Timeout(double seconds, void* arg, BindingAction action);

    /// @brief This type cannot be copied.
    Timeout(const Timeout&) = delete;

    /// @brief This type cannot be moved.
    Timeout(Timeout&&) = delete;

    /// @brief Releases the scheduled task instance and resets the data.
    ~Timeout();

    /// @brief Sets the timeout. The action assigned in the constructor will be called after specified number of secods.
    void set();

    /// @brief Sets the timeout. The action assigned in the constructor will be called after specified number of secods.
    /// @param seconds New time interval value in seconds. Accepts fractional values.
    void set(double seconds);

    /// @brief Resets the time interval value, so the full interval time must be elapsed again since now.
    void reset();

    /// @brief Clears the timeout, so the action will not be called again until `set` or `reset` method is called.
    void clear();

protected:
    TaskId m_taskId;                // Timeout task identifier.
    TickCount m_ticks;              // A time interval in RTOS ticks to call the associated action.
    OptionalBindingAction m_action; // An action to call after the timeout elapses.
    void* m_binding;                // An optional pointer passed to the action.

};

}
