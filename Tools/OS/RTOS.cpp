/**
 * @file        RTOS.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Target RTOS bindings. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "Crash.hpp"
#include "RTOS.hpp"
#include "CurrentThread.hpp"

#if defined(USE_AZURE_RTOS)

void OS::yield(void)
{
    tx_thread_relinquish();
}

void OS::delay(TickCount ticks)
{
    if (tx_thread_sleep(ticks) != TX_SUCCESS) Crash::here();
}

OS::TickCount OS::getTick(void)
{
    return tx_time_get();
}

#elif defined(USE_FREE_RTOS)

void OS::yield(void)
{
    if (CurrentThread::isISRContext())
    {
        portYIELD_FROM_ISR(1);
    }
    else
    {
        portYIELD();
    }
}

void OS::delay(TickCount ticks)
{
    if (CurrentThread::isISRContext()) Crash::here();
    vTaskDelay(ticks);
}

OS::TickCount OS::getTick(void)
{
    return CurrentThread::isISRContext() ? xTaskGetTickCountFromISR() : xTaskGetTickCount();
}

#endif
