/**
 * @file        ThreadBase.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Common methods for RTOS threads. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "ThreadBase.hpp"
#include "Crash.hpp"
#include "CurrentThread.hpp"

#if defined(USE_AZURE_RTOS)

OS::ThreadPriority OS::ThreadBase::changePriority(Priority newPriority)
{
    if (!m_handle || CurrentThread::isISRContext()) Crash::here();
    NativePriority oldPriority;
    tx_thread_priority_change(m_handle, newPriority, &oldPriority);
    return Priority(oldPriority);
}

void OS::ThreadBase::terminate(void)
{
    if (!m_handle || CurrentThread::isISRContext() || tx_thread_delete(m_handle) != TX_SUCCESS) Crash::here();
    m_handle = nullptr;
}

#elif defined(USE_FREE_RTOS)

OS::ThreadPriority OS::ThreadBase::changePriority(Priority newPriority)
{
    Priority old(uxTaskPriorityGet(m_handle));
    vTaskPrioritySet(m_handle, newPriority);
    return old;
}

void OS::ThreadBase::terminate(void)
{
    if (!m_handle || CurrentThread::isISRContext()) Crash::here();
    vTaskDelete(m_handle);
    m_handle = nullptr;
}

#endif
