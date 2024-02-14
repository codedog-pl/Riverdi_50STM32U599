/**
 * @file        target.h
 * @author      Adam Łyskawa
 *
 * @brief       Main target system configuration for the Woof Toolkit.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

// THIS FILE SHOULD BE EDITED TO MATCH THE TARGET SYSTEM DRIVERS AND MIDDLEWARES

#pragma once

#define CORE                    cm33                // Core specific include file name postfix.
#define MCU_PREFIX              stm32u5a9           // MCU specific include file name prefix.
#define HAL_PREFIX              stm32u5xx_hal       // MCU specific HAL include file name prefix.

#define USE_AZURE_RTOS                              // Use AzureRTOS as the Real Time Operating System.
#define USE_FILEX                                   // Use FILEX as the file system access backend.

// #define USE_FREE_RTOS                               // Use FreeRTOS as the Real Time Operationg System.
// #define USE_FATFS                                   // Use FATFS as the file system access backend.

// FOLLOWING VALUES AFFECT BOTH SYSTEM PERFORMANCE AND MEMORY REQUIREMENTS:

#define WTK_ASYNC_RESULTS       32                  // The number of pre-allocated asynchronous operation result handles, default 32.
#define WTK_LOG_Q               64                  // The number of log messages that can be stored in RAM before the first one is committed.
#define WTK_LOG_MSG_SIZE        128                 // The number of bytes allocated for 1 system log message.
#define WTK_OS_TASKS            16                  // The number of pre-allocated scheduled tasks, default 16.
#define WTK_OS_THREAD_STACK     4096                // The number of bytes allocated for `OS::Thread` instance stack.

// SET EXACTLY AS IN THE TARGET RTOS CONFIGURATION:

#define WTK_OS_TICKS_PER_SECOND 1000                // OS ticks per second setting.

#define FS_SD_ROOT              "0:/"
#define FS_USB_ROOT             "1:/"
