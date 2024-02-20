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
#include "adc.h"
#include "ADC.hpp"

class HMI final
{
public:

    STATIC(HMI)

    /// @brief Starts the main application thread.
    static void start();

    /// @brief Passes initialization flags to the HMI startup. The initialization will complete when all flags are passed.
    /// @param flags Initialization flags.
    static void init(uint32_t flags);

    static void ADC1_readingChanged(double value, double change);

    static void ADC2_readingChanged(double value, double change);

    /// @brief Called when the USB media is mounted at "1:/".
    static void USBMediaMounted();

    /// @brief Called when the USB media is unmounted from "1:/".
    static void USBMediaUnmounted();

private:

    inline static OS::Semaphore initSemaphore = {}; // Initialization semaphore.

    inline static ADCObserver<1024, uint16_t> ADC_01 = { &hadc1, 2.0 };
    inline static ADCObserver<1024, uint16_t> ADC_02 = { &hadc2, 2.0 };

};
