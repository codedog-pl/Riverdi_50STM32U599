/**
 * @file        OSAzureRTOS.hpp
 * @author      CodeDog
 * @brief       Unified AzureRTOS API.
 *
 * @remarks     Used to abstract a subset of basic RTOS functions with a common API.
 *
 *              Since AzureRTOS requires preallocated handles, this class preallocates 16 of each type.
 *              If the handles get depleted, the `lastError` is set to `TX_CEILING_EXCEEDED`
 *              and the thread is halted for debugging.
 *              The number of handles can be set in compiler defines as:
 *                  TX_EVENT_GROUPS
 *                  TX_MUTEXES
 *                  TX_SEMAPHORES
 *                  TX_THREADS
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#ifdef USE_AZURE_RTOS

#include <cstdint>
#include "app_threadx.h"
#include "tx_api.h"
#include "OSResourcePool.hpp"

#define TX_RESOURCE_HANLDES_DEPLETED ((UINT) 0xFFFF)

/// @brief RTOS compatibility wrapper.
class OS
{

public:

    OS() = delete;
    OS(const OS&) = delete;
    OS(OS&&) = delete;

public:

    using Status = unsigned int;
    using EventGroupId = unsigned int;
    using MutexId = unsigned int;
    using SemaphoreId = unsigned int;
    using ThreadId = unsigned int;
    using ThreadPriority = unsigned int;
    using ThreadEntryArgType = unsigned long;
    using ThreadEntry = void(*)(ThreadEntryArgType);
    using EventFlags = ULONG;
    using Timeout = ULONG;

    /// @brief Event combining and clearing option value.
    enum EventOption : UINT
    {
        Or = TX_OR, // Signal beside others / wait for any.
        OrClear = TX_OR_CLEAR, // Wait for any and clear.
        And = TX_AND, // Signal discarding others / wait for all.
        AndClear = TX_AND_CLEAR // Wait for all and clear.
    };

    static constexpr Timeout noWait = TX_NO_WAIT;
    static constexpr Timeout waitForever = TX_WAIT_FOREVER;
    static constexpr ThreadPriority threadPriorityAboveNormal = 4;
    static constexpr ThreadPriority threadPriorityNormal = 5;
    static constexpr ThreadPriority threadPriorityBelowNormal = 6;
    static constexpr ThreadPriority threadPriorityHighest = 0;
    static constexpr ThreadPriority threadPriorityLowest = TX_MAX_PRIORITIES - 1;

    static constexpr uint32_t threadDefaultStackSize = TX_APP_STACK_SIZE;

    /// @brief Yields the current thread allowing the other threads to proceed.
    inline static void yield(void)
    {
        tx_thread_relinquish();
    }

    /// @brief Puts the current thread to sleep for a specified number of ticks.
    /// @param ticks The number of OS ticks to wait.
    /// @returns False if error occurred, true otherwise.
    inline static bool delay(uint32_t ticks)
    {
        lastError = tx_thread_sleep(ticks);
        return lastError == TX_SUCCESS;
    }

    /// @brief Creates an event group.
    /// @param name Optional event group name.
    /// @return Group identifier or zero if error occurred.
    inline static EventGroupId eventGroupCreate(const char* name = nullptr)
    {
        EventData* instance = m_eventPool.getInstance();
        if (!instance)
        {
            lastError = TX_RESOURCE_HANLDES_DEPLETED;
            while (1); // Stop here for debugging.
        }
        lastError = tx_event_flags_create(&instance->group, (char*)name);
        return lastError == TX_SUCCESS ? instance->id : 0;
    }

    /// @brief Signals events to an event group.
    /// @param id Event group identifier.
    /// @param flags Flags to signal.
    /// @param option Setting option (default or).
    /// @return False if error occurred, true otherwise.
    inline static bool eventGroupSignal(EventGroupId id, EventFlags flags, EventOption option = Or)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        EventData* instance = m_eventPool.getInstance(id);
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_event_flags_set(&instance->group, flags, option);
        return lastError == TX_SUCCESS;
    }

    /// @brief Suspends the current thread until one or more events are signalled.
    /// @param id Event group identifier.
    /// @param flags Flags awaited (default any / all).
    /// @param option Combining and clearing option (default or, clear).
    /// @param timeout Time to wait for the event in OS ticks (default forever).
    /// @return Event flags received, if none set - error occurred.
    inline static EventFlags eventGroupWait(EventGroupId id, EventFlags flags = (EventFlags)(-1),
                                            EventOption option = OrClear, Timeout timeout = waitForever)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        EventData* instance = m_eventPool.getInstance(id);
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        EventFlags actualFlags;
        lastError = tx_event_flags_get(&instance->group, flags, option, &actualFlags, timeout);
        return lastError == TX_SUCCESS ? actualFlags : 0;
    }

    /// @brief Deletes the RTOS event group.
    /// @param id Event group indentifier.
    /// @return False if error occurred, true otherwise.
    inline static bool eventGroupDelete(EventGroupId id)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        EventData* instance = m_eventPool.getInstance(id);
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_event_flags_delete(&instance->group);
        m_eventPool.returnInstance(instance->id);
        return lastError == TX_SUCCESS;
    }

    /// @brief Creates a new RTOS mutex.
    /// @param name Optional mutex name.
    /// @return Mutex identifier of zero in case of an error.
    inline static MutexId mutexCreate(const char* name = nullptr)
    {
        MutexData* instance = m_mutexPool.getInstance();
        if (!instance)
        {
            lastError = TX_RESOURCE_HANLDES_DEPLETED;
            while (1); // Stop here for debugging.
        }
        lastError = tx_mutex_create(&instance->controlBlock, (CHAR*)name, 0);
        if (lastError != TX_SUCCESS)
        {
            m_mutexPool.returnInstance(instance->id);
            return 0;
        }
        return instance->id;
    }

    /// @brief Waits until a Mutex becomes available.
    /// @param id Mutex identifier.
    /// @param timeout Timeout value in RTOS ticks.
    /// @return False if error occurred, true otherwise.
    inline static bool mutexWait(MutexId id, Timeout timeout = waitForever)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        MutexData* instance = m_mutexPool.getInstance(id);
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_mutex_get(&instance->controlBlock, timeout);
        return lastError == TX_SUCCESS;
    }

    /// @brief Releases the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool mutexRelease(MutexId id)
    {
        if (id < 1) {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        MutexData* instance = m_mutexPool.getInstance(id);
        if (!instance) {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_mutex_put(&instance->controlBlock);
        return lastError == TX_SUCCESS;
    }

    /// @brief Deletes the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool mutexDelete(MutexId id)
    {
        if (id < 1) {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        MutexData* instance = m_mutexPool.getInstance(id);
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_mutex_delete(&instance->controlBlock);
        m_mutexPool.returnInstance(id);
        return lastError == TX_SUCCESS;
    }

    /// @brief Creates a RTOS semaphore.
    /// @param name Semaphore name.
    /// @param initialCount The number of times it can be acquired before blocking.
    /// @return Semaphore identifier or zero if error occurred.
    inline static SemaphoreId semaphoreCreate(const char* name, int initialCount = 0)
    {
        SemaphoreData* instance = m_semaphorePool.getInstance();
        if (!instance)
        {
            lastError = TX_RESOURCE_HANLDES_DEPLETED;
            while (1); // Stop here for debugging.
        }
        lastError = tx_semaphore_create(&instance->controlBlock, nullptr, initialCount = 0);
        return lastError == TX_SUCCESS ? instance->id : 0;
    }

    /// @brief Acquires a semaphore or waits if the count is zero.
    /// @param id Semaphore identifier.
    /// @param timeout Timeout value in RTOS ticks.
    /// @return False if error occurred, true otherwise.
    inline static bool semaphoreWait(SemaphoreId id, Timeout timeout = waitForever)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        SemaphoreData* instance = m_semaphorePool.getInstance();
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_semaphore_get(&instance->controlBlock, timeout);
        return lastError == TX_SUCCESS;
    }

    /// @brief Increases the RTOS semaphore count allowing the waiting thread to continue.
    /// @param id Semaphore identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool semaphoreSignal(SemaphoreId id)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        SemaphoreData* instance = m_semaphorePool.getInstance();
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_semaphore_put(&instance->controlBlock);
        return lastError == TX_SUCCESS;
    }

    /// @brief Deletes a RTOS semaphore.
    /// @param id Semaphore identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool semaphoreDelete(SemaphoreId id)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        SemaphoreData* instance = m_semaphorePool.getInstance();
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_semaphore_delete(&instance->controlBlock);
        m_semaphorePool.returnInstance(instance->id);
        return lastError == TX_SUCCESS;
    }

    /// @brief Creates a new RTOS thread.
    /// @param name Thread name.
    /// @param thread Entry point.
    /// @param priority Thread priority.
    /// @return Thread identifier or nullptr in case of an error.
    inline static ThreadId threadStart(const char* name, ThreadEntry threadEntry, ThreadPriority priority = threadPriorityNormal)
    {
        ThreadData* instance = m_threadPool.getInstance();
        if (!instance)
        {
            lastError = TX_RESOURCE_HANLDES_DEPLETED;
            while (1); // Stop here for debugging.
        }
        lastError = tx_thread_create(
            &instance->controlBlock,
            (char*)name,
            threadEntry,
            0,
            &instance->stack,
            threadDefaultStackSize,
            priority,
            priority > 0 ? priority - 1 : 0, // Preemtion threshold.
            0, // Do not use time slices.
            1 // Auto start thread.
        );
        if (lastError != TX_SUCCESS)
        {
            m_threadPool.returnInstance(instance->id);
            return 0;
        }
        return instance->id;
    }

    /// @brief Terminates the thread and deletes it.
    /// @param id Thread identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool threadDelete(ThreadId id)
    {
        if (id < 1)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        ThreadData* instance = m_threadPool.getInstance(id);
        if (!instance)
        {
            lastError = TX_NOT_AVAILABLE;
            return false;
        }
        lastError = tx_thread_delete(&instance->controlBlock);
        if (lastError != TX_SUCCESS) return false;
        m_threadPool.returnInstance(id);
        return true;
    }

    /// @brief Last operation error code.
    inline static Status lastError = TX_SUCCESS;

private:

    /// @brief Event flags metadata structure.
    struct EventData : public OSResource
    {
        TX_EVENT_FLAGS_GROUP group; // Event flags group.
    };

    /// @brief Mutex instance metadata structure.
    struct  MutexData : public OSResource
    {
        TX_MUTEX controlBlock;  // Mutex control block.
    };

    struct SemaphoreData : public OSResource
    {
        TX_SEMAPHORE controlBlock;  // Semaphore control block.
    };

    /// @brief Thread instance metadata structure.
    struct ThreadData : public OSResource
    {
        ThreadData() : controlBlock() { }
        TX_THREAD controlBlock;                         // Thread control block.
        unsigned char stack[threadDefaultStackSize];    // Thread stack.
    };

#ifdef TX_EVENT_GROUPS
    inline static OSResourcePool<EventData, TX_EVENT_GROUPS> m_eventPool;
#else
    inline static OSResourcePool<EventData, 16> m_eventPool;
#endif

#ifdef TX_MUTEXES
    inline static OSResourcePool<MutexData, TX_MUTEXES> m_mutexPool;
#else
    inline static OSResourcePool<MutexData, 16> m_mutexPool;
#endif

#ifdef TX_SEMAPHORES
    inline static OSResourcePool<SemaphoreData, TX_SEMAPHORES> m_semaphorePool;
#else
    inline static OSResourcePool<SemaphoreData, 16> m_semaphorePool;
#endif

#ifdef TX_THREADS
    inline static OSResourcePool<ThreadData, TX_THREADS> m_threadPool;
#else
    inline static OSResourcePool<ThreadData, 16> m_threadPool;
#endif

};

#endif
