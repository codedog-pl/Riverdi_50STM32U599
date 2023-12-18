/**
 * @file        target.h
 * @author      CodeDog
 *
 * @brief       Main target system configuration for the Woof Toolkit.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#define CORE                    cm33                // Core specific include file name postfix.
#define MCU_PREFIX              stm32u5a9           // MCU specific include file name prefix.
#define HAL_PREFIX              stm32u5xx_hal       // MCU specific HAL include file name prefix.

#define USE_AZURE_RTOS                              // Use AzureRTOS as the Real Time Operating System.
#define USE_FILEX                                   // Use FILEX as the file system access backend.

// #define USE_FREE_RTOS                               // Use FreeRTOS as the Real Time Operationg System.
// #define USE_FATFS                                   // Use FATFS as the file system access backend.

#define OS_EVENT_GROUPS         16                  // The number of pre-allocated event groups for the application, default 16.
#define OS_MUTEXES              16                  // The number of pre-allocated mutexes for the application, default 16.
#define OS_SEMAPHORES           16                  // The number of pre-allocated semaphores for the application, default 16.
#define OS_THREADS              16                  // The number of pre-allocated threads for the application, default 16.

#define WTK_ASYNC_RESULTS       32                  // The number of pre-allocated asynchronous operation result handles, default 32.
#define WTK_LOG_Q               64                  // The number of log messages that can be stored in RAM before the first one is committed.
#define WTK_LOG_MSG_SIZE        128                 // The number of bytes allocated for 1 system log message.
#define WTK_TASKS               32                  // The number of pre-allocated scheduled tasks, default 32.
#define WTK_WAIT_HANDLES        (OS_THREADS / 2)    // The number of pre-allocated wait handles, default `OS_THREADS` / 2.
