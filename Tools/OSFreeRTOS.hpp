/**
 * @file        OSFreeRTOS.hpp
 * @author      CodeDog
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

/// @brief RTOS compatibility wrapper.
class OS
{

public:

    OS() = delete;
    OS(const OSFreeRTOS&) = delete;
    OS(OS&&) = delete;

    using Status = osStatus;                    // Status code (FreeRTOS).
    using MutexId = osMutexId;                  // Mutex identifier (FreeRTOS).
    using ThreadPriority = osPriority;          // Thread priority (FreeRTOS).
#if osCMSIS < 0x20000U
    using ThreadEntry = void(*)(const void*);   // Thread entry function type (FreeRTOS CMSIS_V1).
#else
    using ThreadEntry = void(*)(void*);         // Thread entry function type (FreeRTOS CMSIS_V2).
#endif
    using ThreadId = osThreadId;                // Thread identifier (FreeRTOS).

    static constexpr const char* defaultMutexName = "OS::Mutex";
    static constexpr MutexId noMutex = nullptr;
    static constexpr ThreadId noThread = nullptr;
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

    /// @brief Creates a new RTOS mutex.
    /// @return Mutex identifier of zero in case of an error.
    inline static MutexId mutexCreate()
    {
#if osCMSIS < 0x20000U
        osMutexDef(thisMutex);
        return osMutexCreate(osMutex(thisMutex));
#else
        const osMutexAttr_t attr = { .name = defaultMutexName };
        return osMutexNew(&attr);
#endif
    }

    /// @brief Waits until a Mutex becomes available.
    /// @param id Mutex identifier.
    /// @param timeout Timeout value in milliseconds or 0 for no timeout.
    /// @return Status code.
    inline static Status mutexWait(MutexId id, uint32_t timeout)
    {
#if osCMSIS < 0x20000U
        return osMutexWait(id, timeout);
#else
        return osMutexAcquire(id, timeout);
#endif
    }

    /// @brief Releases the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return Status code.
    inline static Status mutexRelease(MutexId id)
    {
        return osMutexRelease(id);
    }

    /// @brief Deletes the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return Status code.
    inline static Status mutexDelete(MutexId id)
    {
        return osMutexDelete(id);
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
        return osThreadCreate(&threadDef, nullptr);
#else
        const osThreadAttr_t attr = {
            .name = name,
            .stack_size = threadDefaultStackSize,
            .priority = priority
        };
        return osThreadNew(entry, nullptr, &attr);
#endif
    }

};

#endif
