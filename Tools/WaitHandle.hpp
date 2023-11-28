#pragma once

#include <cstdint>
#include "Action.hpp"

/**
 * @brief A wait handle for delayed continuation.
 */
struct WaitHandle final
{

    /// @brief Creates an empty (free) wait handle.
    WaitHandle() : action(), ticks(), reset(), binding() { }

    /// @brief Cancels the wait handle making it a free / inactive handle.
    void cancel()
    {
        action.plain = nullptr;
        ticks = 0;
        reset = 0;
        binding = nullptr;
    }

friend class Delay;
private:
    OptionalBindingAction action;   // Continuation action or nullptr for a free / inactive handle.
    uint32_t ticks;                 // Ticks left before calling the action.
    uint32_t reset;                 // A value to reset the `ticks` to after it goes to zero.
    void* binding;                  // An optional binding to pass when calling the action.

};
