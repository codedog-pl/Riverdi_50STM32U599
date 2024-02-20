/**
 * @file        HMI.cpp
 * @author      CodeDog
 * @brief       Main HMI application start.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "HMI.hpp"
#include "Log.hpp"
#include "OS/AppThread.hpp"
#include "OS/CurrentThread.hpp"
#include "FS/Test.hpp"
#include "DACTest.hpp"

void HMI::start()
{
    Log::msg("HMI: Initializing...");
    while ((HMI_SysInit & HMI_ALL) != HMI_ALL) initSemaphore.wait();
    Log::msg("HMI: Initialization complete.");
    FS::Test::fileAPI(FS::SD(), "fs-test.dat");
    ADC_01.registerCallback(ADC1_readingChanged);
    ADC_01.start();
    ADC_02.registerCallback(ADC2_readingChanged);
    ADC_02.start();
    DACTest dacTest;
    dacTest.start();
    OS::AppThread::start(); // This will wait indefinitely for thread synchronization events.
    dacTest.stop();
}

void HMI::init(uint32_t flags)
{
    HMI_SysInit |= flags;
    initSemaphore.release();
}

void HMI::ADC1_readingChanged(double value, double change)
{
    Log::msg("ADC1: Value: %.3f", value);
}

void HMI::ADC2_readingChanged(double value, double change)
{
    Log::msg("ADC2: Value: %.3f", value);
}

void HMI::USBMediaMounted()
{
    Log::msg("HMI: USB media available.");
    FS::Test::fileAPI(FS::USB(), "fs-test.dat");
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
    OS::AppThread::sync(HMI::USBMediaMounted);
}

void HMI_TriggerUSBMediaUnmounted()
{
    OS::AppThread::sync(HMI::USBMediaUnmounted);
}

}
