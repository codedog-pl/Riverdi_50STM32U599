/**
 * @file        Log.hpp
 * @author      Adam Łyskawa
 *
 * @brief       System debug information logger. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "ILogOutput.hpp"
#include "LogMessagePool.hpp"
#include "StaticClass.hpp"
#include <cstdarg>

/// @brief Provides methods of sending messages to a static system log.
/// @tparam TSize Message pool size.
class Log final
{

    STATIC(Log)

public:

    /// @brief Initializes the default log level.
    /// @param isRelase 1: RELEASE build, fewer messages. 0: DEBUG build, more messages.
    static void init(bool isRelase = false);

    /// @brief Initializes the logger with the UART output.
    /// @param huart UART handle pointer.
    static void initUART(UART_HandleTypeDef* huart);

    /// @brief Starts asynchronous operation as soon as the RTOS is started.
    /// @remarks If not defined in the current output, it does nothing.
    static void startAsync(void);

    /// @returns The current severity level. Messages above this level will be discarded.
    static inline LogMessage::Severity level() { return m_level; }

    /// @brief Sets the current severity level. Messages above this level will be discarded.
    /// @param value New value.
    static inline void level(LogMessage::Severity value) { m_level = value; }

    /// @brief Formats and sends a message.
    /// @param format Text format.
    /// @param ... Variadic arguments.
    static void printf(const char* format, ...);

    /// @brief Formats and sends a message with a time stamp.
    /// @param format Text format.
    /// @param ... Variadic arguments.
    static void tsprintf(const char* format, ...);

    /// @brief Formats and sends a messge for the `detail` severity with indentation.
    /// @param format Text format.
    /// @param ... Variadic arguments.
    static void dump(const char* format, ...);

    /// @brief Formats and sends a message with a timestamp.
    /// @param format Text format.
    /// @param ... Variadic arguments.
    static void msg(const char* format, ...);

    /// @brief Formats and sends a message with a timestamp.
    /// @param severity Message severity.
    /// @param format Text format.
    /// @param ... Variadic arguments.
    static void msg(LogMessage::Severity severity, const char* format, ...);

    /// @returns Current dump indentation value.
    static inline size_t dumpIndentation() { return m_dumpIndentation; }

    /// @brief Sets the current dump indentation.
    /// @param value The number of text columns to indent the dump lines.
    static inline void dumpIndentation(size_t value) { m_dumpIndentation = value; }

protected:

    /// @returns A tuple of:
    ///             - An empty message from the pool, or `nullptr` when the pool is exhausted.
    ///             - Message pool offset.
    static inline std::tuple<LogMessage*, int> getMessage(LogMessage::Severity severity = LogMessage::debug)
    {
        if (severity > m_level) return { 0, 0 }; // Don't produce messages above defined severity.
        return m_pool.get(severity);
    }

    static constexpr size_t dumpIndentationDefault = 24; // Default text indentation for the `dump` method.

    static inline LogMessage::Severity m_level = LogMessage::detail;    // Default log level. Messages above this level will be discarded.
    static inline LogMessagePool<WTK_LOG_Q> m_pool = {};                // Static message pool.
    static inline ILogOutput* m_output = {};                            // Message output implementation.
    static inline size_t m_dumpIndentation = dumpIndentationDefault;    // Current dump line indentation.

};
