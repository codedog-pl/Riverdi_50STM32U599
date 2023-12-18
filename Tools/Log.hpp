/**
 * @file        Log.hpp
 * @author      CodeDog
 *
 * @brief       System log implementation using a backend configured in target configuration.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "target.h"
#include "LogBase.hpp"
#include "LogITM.hpp"
#include "LogUART.hpp"

/// @brief Implements the system log on the target.
class Log final : public LogBase<WTK_LOG_Q>
{

    STATIC(Log)

public:

    /// @brief Initializes the default log level.
    /// @param isRelase 1: RELEASE build, fewer messages. 0: DEBUG build, more messages.
    static inline void init(bool isRelase = false)
    {
        level(isRelase ? LogMessage::info : LogMessage::detail);
        m_output = LogITM::getInstance(m_pool);
    }

    /// @brief Initializes the logger with the UART output.
    /// @param huart UART handle pointer.
    static inline void initUART(UART_HandleTypeDef* huart)
    {
        m_output = LogUART::getInstance(huart, m_pool);
    }

    /// @brief Starts asynchronous operation as soon as the RTOS is started.
    /// @remarks If not defined in the current output, it does nothing.
    static inline void startAsync(void)
    {
        if (m_output) m_output->startAsync();
    }

};
