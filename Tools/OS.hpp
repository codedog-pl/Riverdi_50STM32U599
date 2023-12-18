/**
 * @file        OS.hpp
 * @author      CodeDog
 * @brief       Unified RTOS API.
 *
 * @remarks     Includes the target specific RTOS calls adapter.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "target.h"
#if defined(USE_AZURE_RTOS)
#include "OSAzureRTOS.hpp"
#elif defined(USE_FREE_RTOS)
#include "OSFreeRTOS.hpp"
#endif