/**
 * @file        Mutex.hpp
 * @author      CodeDog
 *
 * @brief       Mutex C++ RAII implementation using the target RTOS mutex.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "OS.hpp"

/// @brief An interface for std::Mutex implementation.
class IStdMutex
{
public:

    IStdMutex() = default;
    virtual ~IStdMutex() = default;

    IStdMutex(const IStdMutex&) = delete;
    IStdMutex(IStdMutex&&) = delete;
    IStdMutex& operator=(const IStdMutex&) = delete;
    IStdMutex& operator=(IStdMutex&&) = delete;

    virtual void lock() = 0;    // Locks the mutex, blocks if the mutex is not available.
    virtual void unlock() = 0;  // Unlocks the mutex.

};

namespace std
{

/// @brief RTOS implemented RAII mutex.
class Mutex : IStdMutex
{
public:

    Mutex(OS::Timeout timeout = OS::waitForever) : m_mutex(), m_timeout(timeout) { }

    Mutex(const Mutex&) = delete;
    Mutex(Mutex&&) = delete;

    ~Mutex() { if (m_mutex) OS::mutexDelete(m_mutex); }

    /// @brief Locks the mutex, blocks if the mutex is not available.
    virtual void lock()
    {
        if (!m_mutex)
        {
            m_mutex = OS::mutexCreate(); // Created here to avoid crash when the type is constructed before kernel starts.
        }
        OS::mutexWait(m_mutex, m_timeout);
    }

    /// @brief Unlocks the mutex.
    virtual void unlock()
    {
        if (!m_mutex) return;
        OS::mutexRelease(m_mutex);
    }

private:
    OS::MutexId m_mutex;    // OS mutex.
    OS::Timeout m_timeout;  // Default timeout.

};

}
