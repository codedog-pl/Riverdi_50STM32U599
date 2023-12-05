/**
 * @file        OS.hpp
 * @author      CodeDog
 * @brief       Unified RTOS API.
 *
 * @remarks     Used to abstract a subset of basic RTOS functions with a common API.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#if defined(USE_AZURE_RTOS)
#include "OSAzureRTOS.hpp"
#elif defined(USE_FREE_RTOS)
#include "OSFreeRTOS.hpp"
#endif