/**
 * @file        OSFreeRTOS.hpp
 * @author      CodeDog
 *
 * @brief       Unified FreeRTOS API.
 *
 * @remarks     Used to abstract a subset of basic RTOS functions with a common API.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#ifdef USE_FREE_RTOS

#include <cstdint>
#include "cmsis_os.h"
#include "FreeRTOSConfig.h"

/// @brief RTOS compatibility wrapper, maps to FreeRTOS CMSIS (current version).
class OS
{

public:

    OS() = delete;
    OS(const OS&) = delete;
    OS(OS&&) = delete;

    using Status = osStatus;                            // RTOS status code.
    using EventGroupId = osEventFlagsId_t;              // Event group identifier.
    using MutexId = osMutexId;                          // Mutex identifier.
    using SemaphoreId = osSemaphoreId_t;                // Semaphore identifier.
    using ThreadId = osThreadId;                        // Thread identifier.
    using EventFlags = uint32_t;                        // Event flags container type.
    using Timeout = uint32_t;                           // Timeout value in RTOS ticks.
    using ThreadPriority = osPriority;                  // Thread priority.
#if osCMSIS < 0x20000U
    using ThreadEntryArgType = const void*;             // Thread entry argument type.
#else
    using ThreadEntryArgType = void*;                   // Thread entry argument type.
#endif
    using ThreadEntry = void(*)(ThreadEntryArgType);    // Thread entry function type.

    static constexpr EventGroupId noEventGroup = nullptr;
    static constexpr MutexId noMutex = nullptr;
    static constexpr SemaphoreId noSemaphore = nullptr;
    static constexpr ThreadId noThread = nullptr;
    static constexpr Timeout noWait = 0U;
    static constexpr Timeout waitForever = osWaitForever;
    static constexpr ThreadPriority threadPriorityAboveNormal = osPriorityAboveNormal;
    static constexpr ThreadPriority threadPriorityNormal = osPriorityNormal;
    static constexpr ThreadPriority threadPriorityBelowNormal = osPriorityBelowNormal;
    static constexpr ThreadPriority threadPriorityHighest = osPriorityRealtime;
    static constexpr ThreadPriority threadPriorityLowest = osPriorityIdle;
    static constexpr uint32_t threadDefaultStackSize = configMINIMAL_STACK_SIZE;

    /// @brief Yields the current thread allowing the other threads to proceed.
    inline static void yield(void)
    {
        taskYIELD();
    }

    /// @brief Puts the current thread to sleep for a specified number of ticks.
    /// @param ticks The number of OS ticks to wait.
    inline static void delay(uint32_t ticks)
    {
        osDelay(ticks);
    }

    /// @brief Creates an event group.
    /// @param name Optional event group name.
    /// @return Group identifier or zero if error occurred.
    inline static EventGroupId eventGroupCreate(const char* name)
    {
        const osEventFlagsAttr_t attr = { .name = name };
        EventGroupId id = osEventFlagsNew(&attr);
        if (!id) lastError = (Status)osFlagsError;
        return id;
    }

    /// @brief Signals events to an event group.
    /// @param id Event group identifier.
    /// @param flags Flags to signal.
    /// @return False if error occurred, true otherwise.
    inline static bool eventGroupSignal(EventGroupId id, EventFlags flags)
    {
        lastError = (Status)osEventFlagsSet(id, flags);
        return lastError == osOK;
    }

    /// @brief Options type for `eventGroupWait` method.
    enum EventOption : uint32_t
    {
        waitAny = osFlagsWaitAny,   // Wait for any flag.
        waitAll = osFlagsWaitAll,   // Wait for all flags.
        noClear = osFlagsNoClear    // Do not clear flags which have been specified to wait for.
    };

    /// @brief Suspends the current thread until one or more events are signalled.
    /// @param id Event group identifier.
    /// @param flags Flags awaited.
    /// @param option Combining and clearing option (default `waitAny`).
    /// @param timeout Time to wait for the event in OS ticks (default forever).
    /// @return Event flags received, if none set - error occurred.
    inline static EventFlags eventGroupWait(EventGroupId id, EventFlags flags = (EventFlags)(-1),
                                            EventOption option = waitAny, Timeout timeout = waitForever)
    {
        lastError = (Status)osEventFlagsWait(id, flags, option, timeout);
        return lastError == osOK;
    }

    /// @brief Deletes the RTOS event group.
    /// @param id Event group indentifier.
    /// @return False if error occurred, true otherwise.
    inline static bool eventGroupDelete(EventGroupId id)
    {
        lastError = osEventFlagsDelete(id);
        return lastError == osOK;
    }

    /// @brief Creates a new RTOS mutex.
    /// @param name Optional mutex name.
    /// @return Mutex identifier of zero in case of an error.
    inline static MutexId mutexCreate(const char* name = nullptr)
    {
#if osCMSIS < 0x20000U
        osMutexDef(name);
        MutexId id = osMutexCreate(osMutex(name));
#else
        const osMutexAttr_t attr = { .name = name };
        MutexId id = osMutexNew(&attr);

#endif
        if (!id) lastError = osError;
        return id;
    }

    /// @brief Waits until a Mutex becomes available.
    /// @param id Mutex identifier.
    /// @param timeout Timeout value in RTOS ticks.
    /// @return False if error occurred, true otherwise.
    inline static bool mutexWait(MutexId id, Timeout timeout = waitForever)
    {
#if osCMSIS < 0x20000U
        return (lastError = osMutexWait(id, timeout)) == osOK;
#else
        return (lastError = osMutexAcquire(id, timeout)) == osOK;
#endif
    }

    /// @brief Releases the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool mutexRelease(MutexId id)
    {
        return (lastError = osMutexRelease(id)) == osOK;
    }

    /// @brief Deletes the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool mutexDelete(MutexId id)
    {
        return (lastError = osMutexDelete(id)) == osOK;
    }

    /// @brief Creates a RTOS semaphore.
    /// @param name Semaphore name.
    /// @param initialCount The number of times it can be acquired before blocking.
    /// @return Semaphore identifier or zero if error occurred.
    inline static SemaphoreId semaphoreCreate(const char* name, int initialCount = 0)
    {
        const osSemaphoreAttr_t attr = { .name = name };
        SemaphoreId id = osSemaphoreNew(initialCount, initialCount, &attr);
        if (!id) lastError = osError;
        return id;
    }

    /// @brief Acquires a semaphore or waits if the count is zero.
    /// @param id Semaphore identifier.
    /// @param timeout Timeout value in RTOS ticks.
    /// @return False if error occurred, true otherwise.
    inline static bool semaphoreWait(SemaphoreId id, Timeout timeout = waitForever)
    {
        return (lastError = (Status)osSemaphoreAcquire(id, timeout)) == osOK;
    }

    /// @brief Increases the RTOS semaphore count allowing the waiting thread to continue.
    /// @param id Semaphore identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool semaphoreRelease(SemaphoreId id)
    {
        return (lastError = osSemaphoreRelease(id)) == osOK;
    }

    /// @brief Deletes a RTOS semaphore.
    /// @param id Semaphore identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool semaphoreDelete(SemaphoreId id)
    {
        return (lastError = osSemaphoreDelete(id)) == osOK;
    }

    /// @brief Creates a new RTOS thread.
    /// @param name Thread name.
    /// @param thread Entry point.
    /// @param priority Thread priority.
    /// @return Thread identifier or nullptr in case of an error.
    inline static ThreadId threadStart(const char* name, ThreadEntry entry, ThreadPriority priority = threadPriorityNormal)
    {
#if osCMSIS < 0x20000U
        const osThreadDef_t threadDef = {
            (char*)name,
            entry,
            priority,
            1, // Instances.
            threadDefaultStackSize >> 2
        };
        ThreadId id = osThreadCreate(&threadDef, nullptr);
#else
        const osThreadAttr_t attr = {
            .name = name,
            .stack_size = threadDefaultStackSize,
            .priority = priority
        };
        ThreadId id = osThreadNew(entry, nullptr, &attr);
#endif
        if (!id) lastError = osError;
        return id;
    }

    /// @brief Terminates the thread and deletes it.
    /// @param id Thread identifier.
    /// @return False if error occurred, true otherwise.
    inline static bool threadDelete(ThreadId id)
    {
        return (lastError = osThreadTerminate(id)) == osOK;
    }

    /// @brief Last operation error code.
    inline static Status lastError = osOK;

};

#endif
