/**
 * @file        Mutex.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An object that provides mutually exclusive access to a resource. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "RTOS.hpp"

namespace OS
{

/// @brief Defines an object that provides mutually exclusive access to a resource.
class  Mutex
{

public:

    /// @brief Creates a new mutex with lazy initialization.
    Mutex();

    /// @brief Additional cleanup.
    ~Mutex();

    /// @brief If not called from ISR waits for a mutex to become available. When called from ISR the call is ignored.
    /// @param timeout The time to wait for the mutex to be released expressed in RTOS ticks.
    /// @returns True if the system call completed successfully. False if error occurred or called from ISR.
    bool acquire(TickCount timeout = waitForever);

    /// @brief If not called from ISR releases the mutex. When called from ISR the call is ignored.
    /// @returns True if the system call completed successfully. False if error occurred or called from ISR.
    bool release();

private:

    /// @brief Performs the lazy initialization of the control block if required.
    void init(void);

#if defined(USE_AZURE_RTOS)
    TX_MUTEX m_controlBlock;
    bool m_isCreated;
#elif defined(USE_FREE_RTOS)
    StaticSemaphore_t m_buffer; // A statically allocated buffer for the data.
    SemaphoreHandle_t m_handle; // A pointer used to access the data.
#endif

};

}
