/**
 * @file        Thread.hpp
 * @author      Adam Łyskawa
 *
 * @brief       RTOS thread class. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "RTOS.hpp"
#include "ThreadBase.hpp"

namespace OS
{

/// @brief Represents a RTOS thread, including the control block and the stack memory.
class Thread final : public ThreadBase
{

public:

    /// @brief Creates an empty thread container for the thread to be started later.
    Thread();

    /// @brief Terminates the RTOS thread on going out of scope.
    ~Thread();

    /// @brief Starts the thread with the given entry point.
    ///        Do not call from ISR, or when the thread is already started.
    ///        It can be called again after `terminate` method is called.
    /// @param entry A function that will be called from the thread context. The function will receive `nullptr` as the argument.
    /// @param name Thread name, default `nullptr`.
    /// @param priority Thread priority, default `Priority::normal`.
    inline void start(ThreadEntry entry, const char *name = nullptr, Priority priority = Priority::normal)
    {
        start(nullptr, entry, name, priority);
    }

    /// @brief Starts the thread with the given entry point.
    ///        Do not call from ISR, or when the thread is already started.
    ///        It can be called again after `terminate` method is called.
    /// @param arg A pointer argument to pass to the `entry` function.
    /// @param entry A function that will be called from the thread context.
    /// @param name Thread name, default `nullptr`.
    /// @param priority Thread priority, default `Priority::normal`.
    void start(void *arg, ThreadEntry entry, const char *name = nullptr, Priority priority = Priority::normal);

private:

#if defined(USE_AZURE_RTOS)
    TX_THREAD m_controlBlock;   // Contains the thread control block.
#elif defined(USE_FREE_RTOS)
    StaticTask_t m_buffer;      // A statically allocated buffer for the data.
#endif
    uint32_t m_stack[WTK_OS_THREAD_STACK >> 2]; // Thread stack memory aligned to the MCU word boundary.

};

}
