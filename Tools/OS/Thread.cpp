/**
 * @file        Thread.cpp
 * @author      Adam Łyskawa
 *
 * @brief       RTOS thread class. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "Thread.hpp"
#include "CurrentThread.hpp"
#include "Crash.hpp"

#if defined(USE_AZURE_RTOS)

///
// Azure RTOS implementation
//

OS::Thread::Thread() : ThreadBase(), m_controlBlock() { }

OS::Thread::~Thread()
{
    if (m_handle)
    {
        terminate();
        m_handle = nullptr;
    }
}

void OS::Thread::start(void *arg, ThreadEntry entry, const char *name, Priority priority)
{
    if (m_handle) Crash::here(); // The thread is already started!
    auto result = tx_thread_create(
        &m_controlBlock,
        (char*)name,
        entry,
        reinterpret_cast<UINT>(arg),
        m_stack,
        sizeof(m_stack),
        priority,
        priority + 1, // Preemtion threshold.
        0, // Do not use time slices.
        1 // Auto start thread.
    );
    m_handle = result == TX_SUCCESS ? &m_controlBlock : nullptr;
    if (!m_handle) Crash::here(); // Thread creation failed!
}

#elif defined(USE_FREE_RTOS)

///
// FreeRTOS implementation
//

OS::Thread::Thread() : ThreadBase(), m_buffer() { }

OS::Thread::~Thread()
{
    if (m_handle) terminate();
}

void OS::Thread::start(void *arg, ThreadEntry entry, const char *name, Priority priority)
{
    if (m_handle) Crash::here(); // The thread is already started!
    m_handle = xTaskCreateStatic(entry, name, sizeof(m_stack) >> 2, arg, priority, m_stack, &m_buffer);
    if (!m_handle) Crash::here(); // Thread creation failed!
}

#endif
