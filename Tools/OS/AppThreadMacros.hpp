/**
 * @file        AppThreadMacros.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Macros used to synchronize calling instance methods from the main application thread.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "AppThread.hpp"

/// @brief If called from ISR, schedules current instance method to be called from the main application thread and exits.
/// @param type The name of the class (this type).
/// @param method The name of the method to call from the main application thread.
#define SYNC_ISR(type, method)\
if (OS::CurrentThread::isISRContext())\
{\
    OS::AppThread::sync(this, [](OS::ThreadArg arg){ reinterpret_cast<type*>(arg)->method(); });\
    return;\
}
