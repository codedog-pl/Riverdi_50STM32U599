/**
 * @file        hal_target.h
 * @author      CodeDog
 * @brief       A header file for targeting HAL header files.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#define _VALUE(x) x
#define _STRING(x) #x
#define _TO_STRING(x) _STRING(x)
#define _CONCAT_INTERNAL(a, b) a ## b
#define _CONCAT(a, b) _CONCAT_INTERNAL(a, b)

#ifdef CORE
#define CORE_HEADER _TO_STRING(_CONCAT(core_, CORE.h))  // HAL Core header file.
#endif

#ifdef MCU_PREFIX
#define MCU_HEADER _TO_STRING(_CONCAT(MCU_PREFIX, xx.h)) // HAL MCU main header file name.
#else
#error Define MCU_PREFX to locate main HAL MCU header file.
#endif

#ifdef HAL_PREFIX
#define HAL_HEADER _TO_STRING(HAL_PREFIX.h) // HAL main header file name.
#define HAL_HEADER_(suffix) _TO_STRING(_CONCAT(HAL_PREFIX, _CONCAT(_, suffix.h))) // HAL header with specified suffix.
#else
#error Define HAL_PREFX to locate HAL header files.
#endif
