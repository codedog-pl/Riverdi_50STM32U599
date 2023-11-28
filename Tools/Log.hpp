#pragma once

#include "LogBase.hpp"
#include "DebugUART.hpp"

/// @brief System-wide UART log implementation.
class Log final : public LogBase<256>
{

public:

    /// @brief Initializes the default log level.
    /// @param isRelase 1: RELEASE build, fewer messages. 0: DEBUG build, more messages.
    static inline void init(bool isRelase)
    {
        level(isRelase ? LogMessage::info : LogMessage::detail);
    }

    /// @brief Initializes the logger with the UART output.
    /// @param huart UART handle pointer.
    static inline void init(UART_HandleTypeDef* huart)
    {
        m_output = DebugUART::getInstance(huart, m_pool);
    }

};
