/**
 * @file        TaskControlBlock.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Task control block structure. Header only.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "Action.hpp"
#include "RTOS.hpp"

namespace OS
{

/// @brief An action binding structure for a scheduled function call.
struct TaskControlBlock final
{
    TaskId id;                      // Identifier.
    void* binding;                  // Optional action binding.
    OptionalBindingAction action;   // Action callback.
    ThreadContext context;          // Thread context.
    TickCount delayTicks;           // RTOS ticks to wait before this task is called.
    TickCount resetTicks;           // RTOS ticks to reset the `delayTicks` to repeat the task.

    /// @brief Creates an empty task control block.
    TaskControlBlock() : id(0), binding(), action(), context(none), delayTicks(0), resetTicks(0) { }

    /// @brief Resets the task control block to an empty state.
    inline void clear(void)
    {
        id = 0;
        binding = nullptr;
        action = nullptr;
        context = none;
        delayTicks = 0;
        resetTicks = 0;
    }

};

}
