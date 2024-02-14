/**
 * @file        CurrentThread.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides the current thread static methods. Header only.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "target.h"
#include "hal_mcu.h"
#include "ThreadBase.hpp"
#include "StaticClass.hpp"
#include <cstddef>

namespace OS
{

/// @brief Provides the current thread static methods.
class CurrentThread final
{

    STATIC(CurrentThread)

public:

    /// @returns Current thread instance pointer.
    static inline ThreadBase get()
    {
        if (isISRContext()) return nullptr;
#if defined(USE_AZURE_RTOS)
        auto handle = tx_thread_identify();
#elif defined(USE_FREE_RTOS)
        auto handle = xTaskGetCurrentTaskHandle();
#endif
        return ThreadBase(handle);
    }

    /// @returns 1: Called from ISR. 0: Not called from ISR.
    static inline bool isISRContext() { return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0; }

};

}
