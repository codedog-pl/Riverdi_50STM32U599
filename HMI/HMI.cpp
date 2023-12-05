/**
 * @file        HMI.cpp
 * @author      CodeDog
 * @brief       Main HMI application start.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "HMI.hpp"
#include "Log.hpp"
#include "tx_api.h"
#include "main.h"
#include "GPIO_test.hpp"

#include <iostream>

void HMI::start()
{
    Log::msg("HMI: Thread started.");
    constexpr uint32_t initializationLatency = 100;
    while ((HMI_SysInit & HMI_ALL) != HMI_ALL) OS::delay(initializationLatency);
    Log::msg("HMI: Initialization complete.");
    while (1)
    {
        OS::delay(1000); // for now
    }
}

// C bindings

extern "C"
{

uint32_t HMI_SysInit = HMI_NONE;

void HMI_Start()
{
   HMI::start();
}

}
