/**
 * @file        HMI.hpp
 * @author      CodeDog
 * @brief       -
 * @remarks     -
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "hmi.h"
#include "StaticClass.hpp"
#include "OS/Semaphore.hpp"

class HMI final
{
public:

    STATIC(HMI)

    /// @brief Starts the main application thread.
    static void start();

    /// @brief Passes initialization flags to the HMI startup. The initialization will complete when all flags are passed.
    /// @param flags Initialization flags.
    static void init(uint32_t flags);

    /// @brief Called when the USB media is mounted at "1:/".
    static void USBMediaMounted();

    /// @brief Called when the USB media is unmounted from "1:/".
    static void USBMediaUnmounted();

private:

    inline static OS::Semaphore initSemaphore = {}; // Initialization semaphore.

};
