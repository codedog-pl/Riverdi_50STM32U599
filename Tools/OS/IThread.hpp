/**
 * @file        IThread.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Declares an API for the common system thread properties and methods.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "RTOS.hpp"
#include "ThreadPriority.hpp"

namespace OS
{

/// @brief System thread interface.
class IThread
{

public:

    using Priority = ThreadPriority; // RTOS thread priority.

    /// @returns A value indicating that the thread is active / started.
    virtual bool active() const = 0;

    /// @returns A handle that can be used to locate the tread control block in the system.
    virtual ThreadHandle handle() const = 0;

    /// @brief Changes the priority of the thread. DO NOT CALL FROM ISR!
    /// @param newPriority New priority to set.
    /// @returns The old priority before this call.
    virtual Priority changePriority(Priority newPriority) = 0;

    /// @brief Terminates the RTOS thread. Do not terminate inactive threads. DO NOT CALL FROM ISR!
    virtual void terminate(void) = 0;

};

}
