/**
 * @file        ThreadBase.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Common methods for RTOS threads. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "IThread.hpp"

namespace OS
{

/// @brief Provides common methods for RTOS threads.
/// @remarks Stores just the system handle, without the control block data and stack.
class ThreadBase : public IThread
{

public:

    /// @brief Creates an empty / inactive thread base.
    ThreadBase() : m_handle() { }

    /// @brief Creates an active thread base for a RTOS thread handle.
    /// @param handle RTOS thread handle.
    ThreadBase(ThreadHandle handle) : m_handle(handle) { }

    /// @returns A value indicating that the thread is active / started.
    inline bool active() const override { return m_handle != nullptr; };

    /// @returns A handle that can be used to locate the tread control block in the system.
    inline ThreadHandle handle() const override { return m_handle; }

    /// @brief Changes the priority of the thread. DO NOT CALL FROM ISR!
    /// @param newPriority New priority to set.
    /// @returns The old priority before this call.
    Priority changePriority(Priority newPriority) override;

    /// @brief Terminates the RTOS thread. Do not terminate inactive threads. DO NOT CALL FROM ISR!
    void terminate(void) override;

protected:
    ThreadHandle m_handle;
};

}
