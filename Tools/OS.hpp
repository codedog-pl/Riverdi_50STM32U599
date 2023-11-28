#pragma once

#include <cstdint>

#if defined(USE_FREE_RTOS)
#include "cmsis_os.h"
#elif defined(USE_AZURE_RTOS)
#include "tx_api.h"
#include "OSResourcePool.hpp"
#endif

/// @brief RTOS compatibility wrapper.
class OS
{

public:

    OS() = delete;
    OS(const OS&) = delete;
    OS(OS&&) = delete;

// Thread start

#if defined(USE_FREE_RTOS)

    using Status = osStatus;                    // Status code (FreeRTOS).
    using MutexId = osMutexId;                  // Mutex identifier (FreeRTOS).
    using ThreadPriority = osPriority;          // Thread priority (FreeRTOS).
#if osCMSIS < 0x20000U
    using ThreadEntry = void(*)(const void*);   // Thread entry function type (FreeRTOS CMSIS_V1).
#else
    using ThreadEntry = void(*)(void*);         // Thread entry function type (FreeRTOS CMSIS_V2).
#endif
    using ThreadId = osThreadId;                // Thread identifier (FreeRTOS).

    static constexpr MutexId noMutex = nullptr;
    static constexpr ThreadId noThread = nullptr;
    static constexpr ThreadPriority threadPriorityAboveNormal = osPriorityAboveNormal;
    static constexpr ThreadPriority threadPriorityNormal = osPriorityNormal;
    static constexpr ThreadPriority threadPriorityBelowNormal = osPriorityBelowNormal;
    static constexpr ThreadPriority threadPriorityHighest = osPriorityRealtime;
    static constexpr ThreadPriority threadPriorityLowest = osPriorityIdle;
    static constexpr uint32_t threadDefaultStackSize = 1024;

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

    /// @brief Creates a new RTOS mutex.
    /// @return Mutex identifier of zero in case of an error.
    inline static MutexId mutexCreate()
    {
#if osCMSIS < 0x20000U
        osMutexDef(thisMutex);
        return osMutexCreate(osMutex(thisMutex));
#else
        const osMutexAttr_t attr = { .name = "thisMutex" };
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

#elif defined(USE_AZURE_RTOS)

public:

    using Status = unsigned int;
    using ThreadId = unsigned int;
    using MutexId = unsigned int;
    using ThreadPriority = unsigned int;
    using ThreadEntryArgType = unsigned long;
    using ThreadEntry = void(*)(ThreadEntryArgType);

    static constexpr ThreadPriority threadPriorityAboveNormal = 4;
    static constexpr ThreadPriority threadPriorityNormal = 5;
    static constexpr ThreadPriority threadPriorityBelowNormal = 6;
    static constexpr ThreadPriority threadPriorityHighest = 0;
    static constexpr ThreadPriority threadPriorityLowest = TX_MAX_PRIORITIES - 1;

    static constexpr uint32_t threadDefaultStackSize = 1024;

    /// @brief Creates a new RTOS mutex.
    /// @return Mutex identifier of zero in case of an error.
    inline static MutexId mutexCreate()
    {
        MutexData* instance = m_mutexPool.getInstance();
        if (!instance) return 0;
        Status status = tx_mutex_create(&instance->controlBlock, 0, 0);
        if (status != TX_SUCCESS)
        {
            m_mutexPool.returnInstance(instance->id);
            return 0;
        }
        return instance->id;
    }

    /// @brief Waits until a Mutex becomes available.
    /// @param id Mutex identifier.
    /// @param timeout Timeout value in milliseconds or 0 for no timeout (fail immediately if mutex taken).
    /// @return Status code.
    inline static Status mutexWait(MutexId id, uint32_t timeout)
    {
        if (id < 1) return TX_POOL_ERROR;
        MutexData* instance = m_mutexPool.getInstance(id);
        if (!instance) return TX_POOL_ERROR;
        uint32_t ticks = (timeout * TX_TIMER_TICKS_PER_SECOND) / 1000;
        return tx_mutex_get(&instance->controlBlock, timeout);
    }

    /// @brief Releases the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return Status code.
    inline static Status mutexRelease(MutexId id)
    {
        if (id < 1) return TX_POOL_ERROR;
        MutexData* instance = m_mutexPool.getInstance(id);
        if (!instance) return TX_POOL_ERROR;
        return tx_mutex_put(&instance->controlBlock);
    }

    /// @brief Deletes the RTOS mutex.
    /// @param id Mutex identifier.
    /// @return Status code.
    inline static Status mutexDelete(MutexId id)
    {
        if (id < 1) return TX_POOL_ERROR;
        MutexData* instance = m_mutexPool.getInstance(id);
        if (!instance) return TX_POOL_ERROR;
        Status status = tx_mutex_delete(&instance->controlBlock);
        m_mutexPool.returnInstance(id);
        return status;
    }

    /// @brief Creates a new RTOS thread.
    /// @param name Thread name.
    /// @param thread Entry point.
    /// @param priority Thread priority.
    /// @return Thread identifier or nullptr in case of an error.
    inline static ThreadId threadStart(const char* name, ThreadEntry threadEntry, ThreadPriority priority = threadPriorityNormal)
    {
        ThreadData* instance = m_threadPool.getInstance();
        if (!instance) return 0;
        Status status = tx_thread_create(
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
        if (status != TX_SUCCESS)
        {
            m_threadPool.returnInstance(instance->id);
            return 0;
        }
        return instance->id;
    }

    /// @brief Terminates the thread and deletes it.
    /// @param id Thread identifier.
    /// @return Status.
    inline static Status threadTerminate(ThreadId id)
    {
        if (id < 1) return TX_POOL_ERROR;
        ThreadData* instance = m_threadPool.getInstance(id);
        if (!instance) return TX_POOL_ERROR;
        Status status = tx_thread_delete(&instance->controlBlock);
        if (status != TX_SUCCESS) return status;
        m_threadPool.returnInstance(id);
        return TX_SUCCESS;
    }

private:

    /// @brief Thread instance metadata structure.
    struct ThreadData : public OSResource
    {
        ThreadData() : controlBlock() { }
        TX_THREAD controlBlock;                         // Thread control block.
        unsigned char stack[threadDefaultStackSize];    // Thread stack.
    };

    /// @brief Mutex instance metadata structure.
    struct  MutexData : public OSResource
    {
        TX_MUTEX controlBlock;  // Mutex control block.
    };

#ifdef TX_MUTEXES
    inline static OSResourcePool<MutexData, TX_MUTEXES> m_mutexPool;
#else
    inline static OSResourcePool<MutexData, 16> m_mutexPool;
#endif

#ifdef TX_THREADS
    inline static OSResourcePool<ThreadData, TX_THREADS> m_threadPool;
#else
    inline static OSResourcePool<ThreadData, 16> m_threadPool;
#endif


#endif

};
