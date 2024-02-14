/**
 * @file        Semaphore.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Binary semaphore. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "Semaphore.hpp"
#include "CurrentThread.hpp"
#include "Crash.hpp"

#if defined(USE_AZURE_RTOS)

OS::Semaphore::Semaphore() : m_controlBlock(), m_isCreated(false), m_isTaken(false) { }

OS::Semaphore::~Semaphore()
{
    if (m_isCreated)
    {
        tx_semaphore_delete(&m_controlBlock);
        m_isCreated = false;
    }
}

bool OS::Semaphore::wait(TickCount timeout)
{
    if (m_isTaken || CurrentThread::isISRContext()) Crash::here();
    init();
    m_isTaken = true;
    bool ok = tx_semaphore_get(&m_controlBlock, timeout) == TX_SUCCESS;
    m_isTaken = false;
    return ok;
}

bool OS::Semaphore::release(void)
{
    if (!m_isCreated || !m_isTaken) return false;
    return tx_semaphore_put(&m_controlBlock) == TX_SUCCESS;
}

void OS::Semaphore::init(void)
{
    if (m_isCreated) return;
    auto result = tx_semaphore_create(&m_controlBlock, nullptr, 0);
    m_isCreated = result == TX_SUCCESS;
    if (!m_isCreated) Crash::here(); // Semaphore creation failed!
}

#elif defined(USE_FREE_RTOS)

OS::Semaphore::Semaphore() : m_buffer(), m_handle(), m_isTaken(false) { }

OS::Semaphore::~Semaphore()
{
    if (m_handle) m_handle = nullptr;
}

bool OS::Semaphore::wait(TickCount timeout)
{
    if (m_isTaken || CurrentThread::isISRContext()) Crash::here();
    init();
    m_isTaken = true;
    bool ok = xSemaphoreTake(m_handle, timeout) == pdTRUE;
    m_isTaken = false;
    return ok;
}

bool OS::Semaphore::release(void)
{
    if (!m_isTaken) return false;
    if (CurrentThread::isISRContext())
    {
        BaseType_t pxHigherPriorityTaskWoken = 0;
        if (xSemaphoreGiveFromISR(m_handle, &pxHigherPriorityTaskWoken) == pdTRUE)
        {
            if (pxHigherPriorityTaskWoken) portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
            return true;
        }
        return false;
    }
    return xSemaphoreGive(m_handle) == pdTRUE;
}

void OS::Semaphore::init(void)
{
    if (m_handle) return;
    m_handle = xSemaphoreCreateBinaryStatic(&m_buffer);
    if (!m_handle) Crash::here(); // Semaphore creation failed!
}

#endif
