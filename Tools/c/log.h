/**
 * @file        c_debug.h
 * @author      CodeDog
 * @brief       Debug module C bindings.
 * @remarks     -
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hal.h"

/// @brief Initializes the default log level.
/// @param isRelease 1: RELEASE build, fewer messages. 0: DEBUG build, more messages.
void log_level(bool isRelease);

/// @brief Initializes the UART debug module by providing the configured UART handle pointer.
void log_init(UART_HandleTypeDef* huart);

/// @brief Sends a debug message.
/// @param severity 0: error, 1: warning, 2: info, 3: debug, 4: detail, 5: spam.
/// @param format Format string.
/// @param ... arguments passed with the format string.
void log_msg(uint8_t severity, const char* format, ...);

/// @brief Starts asynchronous operation as soon as the RTOS is started.
/// @remarks If not defined in the current output, it does nothing.
void log_start_async(void);
