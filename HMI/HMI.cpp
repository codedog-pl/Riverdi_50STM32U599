/**
 * @file        HMI.cpp
 * @author      CodeDog
 * @brief       Main HMI application start.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "HMI.hpp"
#include "Log.hpp"
#include "Thread.hpp"

#include <iostream>

void HMI::start()
{
    Log::msg("HMI: Initializing...");
    initSemaphore = OS::semaphoreCreate("HMI_Initialization");
    while ((HMI_SysInit & HMI_ALL) != HMI_ALL) OS::semaphoreWait(initSemaphore);
    OS::semaphoreDelete(initSemaphore);
    initSemaphore = 0;
    Log::msg("HMI: Initialization complete.");
    Thread::dispatchLoop(); // This will wait indefinitely for thread synchronization events.
}

void HMI::init(uint32_t flags)
{
    HMI_SysInit |= flags;
    if (initSemaphore) OS::semaphoreRelease(initSemaphore);
}

void HMI::USBMediaMounted()
{
    Log::msg("HMI: USB media available.");
}

void HMI::USBMediaUnmounted()
{
    Log::msg("HMI: USB media disconnected.");
}

// C bindings

extern "C"
{

uint32_t HMI_SysInit = HMI_NONE;

void HMI_Start()
{
   HMI::start();
}

void HMI_Init(uint32_t flags)
{
    HMI::init(flags);
}

void HMI_TriggerUSBMediaMounted()
{
    Thread::sync(HMI::USBMediaMounted);
}

void HMI_TriggerUSBMediaUnmounted()
{
    Thread::sync(HMI::USBMediaUnmounted);
}

}
