/**
 * @file        Mutex.cpp
 * @author      Adam Łyskawa
 *
 * @brief       An object that provides mutually exclusive access to a resource.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "Mutex.hpp"
#include "CurrentThread.hpp"
#include "Crash.hpp"

#if defined(USE_AZURE_RTOS)

OS::Mutex::Mutex() : m_controlBlock(), m_isCreated(false) { }

OS::Mutex::~Mutex()
{
    if (m_isCreated)
    {
        if (tx_mutex_delete(&m_controlBlock) == TX_SUCCESS) m_isCreated = false;
    }
}

bool OS::Mutex::acquire(TickCount timeout)
{
    if (CurrentThread::isISRContext()) return false;
    init();
    return tx_mutex_get(&m_controlBlock, timeout) == TX_SUCCESS;
}

bool OS::Mutex::release()
{
    if (!m_isCreated || CurrentThread::isISRContext()) return false;
    return tx_mutex_put(&m_controlBlock) == TX_SUCCESS;
}

void OS::Mutex::init(void)
{
    if (m_isCreated) return;
    m_isCreated = tx_mutex_create(&m_controlBlock, nullptr, 0) == TX_SUCCESS;
    if (!m_isCreated) Crash::here(); // Mutex creation failed!
}

#elif defined(USE_FREE_RTOS)

OS::Mutex::Mutex() : m_buffer(), m_handle() { }

OS::Mutex::~Mutex()
{
    if (m_handle) m_handle = nullptr;
}

bool OS::Mutex::acquire(TickCount timeout)
{
    if (CurrentThread::isISRContext()) return false;
    init();
    return xSemaphoreTake(m_handle, timeout) == pdTRUE;
}

bool OS::Mutex::release()
{
    if (!m_handle || CurrentThread::isISRContext()) return false;
    return xSemaphoreGive(m_handle) == pdTRUE;
}

void OS::Mutex::init(void)
{
    if (m_handle) return;
    m_handle = xSemaphoreCreateMutexStatic(&m_buffer);
    if (!m_handle) Crash::here(); // Mutex creation failed!
}

#endif
