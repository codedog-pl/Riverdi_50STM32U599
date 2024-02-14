/**
 * @file        Semaphore.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Binary semaphore. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "RTOS.hpp"

namespace OS
{

/// @brief Binary semaphore.
class Semaphore
{

public:

    /// @brief Creates a new binary semaphore that is in an empty state, so the `wait` method will block until the `release` method is called.
    Semaphore();

    /// @brief Additional cleanup.
    ~Semaphore();

    /// @brief Waits until the semaphore is released. DO NOT CALL FROM ISR!
    /// @param timeout The time to wait for the semaphore to be released expressed in RTOS ticks.
    /// @returns True if the system call completed successfully.
    bool wait(TickCount timeout = waitForever);

    /// @brief Releases the semaphore so the thread that waits at the `wait` method can proceed.
    /// @returns True if the system call completed successfully.
    bool release(void);

private:

    /// @brief Performs the lazy initialization of the control block if required.
    void init(void);

#if defined(USE_AZURE_RTOS)
    TX_SEMAPHORE m_controlBlock;    // Direct semaphore control block.
    bool m_isCreated;               // A value indicating the semaphore has been created successfully.
#elif defined(USE_FREE_RTOS)
    StaticSemaphore_t m_buffer; // A statically allocated buffer for the data.
    SemaphoreHandle_t m_handle; // A pointer used to access the data.
#endif

    bool m_isTaken; // True if the semaphore is taken.

};

}
