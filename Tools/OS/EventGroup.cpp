/**
 * @file        EventGroup.cpp
 * @author      Adam Łyskawa
 *
 * @brief       An object that allows to signal events across the RTOS threads. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "BitFlags.hpp"
#include "Crash.hpp"
#include "CurrentThread.hpp"
#include "EventGroup.hpp"

#if defined(USE_AZURE_RTOS)

OS::EventGroup::EventGroup() : m_controlBlock(), m_isCreated(false) { }

OS::EventGroup::~EventGroup()
{
    tx_event_flags_delete(&m_controlBlock);
    m_isCreated = false;
}

bool OS::EventGroup::signal(EventFlags bits)
{
    init();
    auto result = tx_event_flags_set(&m_controlBlock, bits, TX_OR);
    return result == TX_SUCCESS;
}

OS::EventFlags OS::EventGroup::wait(EventFlags bits, WaitOptions options, TickCount timeout)
{
    init();
    if (CurrentThread::isISRContext()) Crash::here(); // Can't wait in ISR!
    EventFlags actualFlags;
    UINT txOption = (options & noClear)
        ? ((options & waitAny) ? TX_OR : (options & waitAll) ? TX_AND : 0)
        : ((options & waitAny) ? TX_OR_CLEAR : (options & waitAll) ? TX_AND_CLEAR : 0);
    auto result = tx_event_flags_get(&m_controlBlock, bits, txOption, &actualFlags, timeout);
    return result == TX_SUCCESS ? actualFlags : 0;
}

void OS::EventGroup::init(void)
{
    if (m_isCreated) return;
    auto result = tx_event_flags_create(&m_controlBlock, nullptr);
    m_isCreated = result == TX_SUCCESS;
    if (!m_isCreated) Crash::here(); // Event group creation failed!
}

#elif defined(USE_FREE_RTOS)

OS::EventGroup::EventGroup() : m_buffer(), m_handle() { }

OS::EventGroup::~EventGroup()
{
    if (m_handle) m_handle = nullptr;
}

bool OS::EventGroup::signal(EventFlags bits)
{
    init();
    if (CurrentThread::isISRContext())
    {
        BaseType_t pxHigherPriorityTaskWoken = 0;
        if (xEventGroupSetBitsFromISR(m_handle, bits, &pxHigherPriorityTaskWoken) == pdPASS)
        {
            if (pxHigherPriorityTaskWoken) portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
            return true;
        }
        return false;
    }
    return xEventGroupSetBits(m_handle, bits) == pdPASS;
}

OS::EventFlags OS::EventGroup::wait(EventFlags bits, WaitOptions options, TickCount timeout)
{
    init();
    if (CurrentThread::isISRContext()) Crash::here(); // Can't wait in ISR!
    return xEventGroupWaitBits(m_handle, bits, !(options & noClear), !!(options & waitAll), timeout);
}

void OS::EventGroup::init(void)
{
    if (m_handle) return;
    m_handle = xEventGroupCreateStatic(&m_buffer);
    if (!m_handle) Crash::here(); // Event group creation failed!
}

#endif
