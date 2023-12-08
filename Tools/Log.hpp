#pragma once

#include "LogBase.hpp"
#include "DebugITM.hpp"
#include "DebugUART.hpp"

/// @brief System-wide UART log implementation.
class Log final : public LogBase<256>
{

public:

    /// @brief Initializes the default log level.
    /// @param isRelase 1: RELEASE build, fewer messages. 0: DEBUG build, more messages.
    static inline void init(bool isRelase = false)
    {
        level(isRelase ? LogMessage::info : LogMessage::detail);
        m_output = DebugITM::getInstance(m_pool);
    }

    /// @brief Initializes the logger with the UART output.
    /// @param huart UART handle pointer.
    static inline void initUART(UART_HandleTypeDef* huart)
    {
        m_output = DebugUART::getInstance(huart, m_pool);
    }

    /// @brief Starts asynchronous operation as soon as the RTOS is started.
    /// @remarks If not defined in the current output, it does nothing.
    static inline void startAsync(void)
    {
        if (m_output) m_output->startAsync();
    }

};
