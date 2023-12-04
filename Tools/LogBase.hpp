/**
 * @file        Debug.hpp
 * @author      CodeDog
 * @brief       Provides serial port debugging. Header file.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "IDebugOutput.hpp"
#include "LogMessagePool.hpp"
#include <cstdio>

/// @brief Provides methods of sending message to a static system log.
/// @tparam TSize Message pool size.
template<size_t TSize>
class LogBase
{

public:

    /// @returns The current severity level. Messages above this level will be discarded.
    static inline LogMessage::Severity level()
    {
        return m_level;
    }

    /// @brief Sets the current severity level. Messages above this level will be discarded.
    /// @param value New value.
    static inline void level(LogMessage::Severity value)
    {
        m_level = value;
    }

    /**
     * @brief Prints a formatted debug message.
     *
     * @param format Text format.
     * @param ...args Variadic arguments.
     */
    template<class ...va> static void printf(const char* format, va ...args)
    {
        if (m_output && !m_output->isAvailable()) return;
        auto [message, offset] = getMessage(); if (!message) return;
        message->printf(format, args...);
        if (m_output && !offset) m_output->send(offset);
    }

    /**
     * @brief Prints a formatted debug message with a time stamp.
     *
     * @param format Text format.
     * @param ... Optional arguments.
     */
    template<class ...va> static void tsprintf(const char* format, va ...args)
    {
        if (m_output && !m_output->isAvailable()) return;
        auto [message, offset] = getMessage(); if (!message) return;
        message->addTimestamp()->add(' ')->printf(format, args...);
        if (m_output && !offset) m_output->send(offset);
    }

    /**
     * @brief Adds a formatted debug message to the log, for the detail level and above.
     *
     * @param format Text format.
     * @param args... Variadic arguments.
     */
    template<class ...va> static void dump(const char* format, va ...args)
    {
        if (m_output && !m_output->isAvailable()) return;
        if (m_level < LogMessage::detail) return;
        auto [message, offset] = getMessage(LogMessage::detail); if (!message) return;
        if (m_dumpIndentation) message->add(' ', m_dumpIndentation);
        message->printf(format, args...)->add("\r\n");
        if (m_output && !offset) m_output->send(offset);
    }

    /**
     * @brief Adds a formatted debug message to the log with a timestamp.
     *
     * @param format Text format.
     * @param args... Variadic arguments.
     */
    template<class ...va> static void msg(const char* format, va ...args)
    {
        if (m_output && !m_output->isAvailable()) return;
        auto [message, offset] = getMessage(); if (!message) return;
        message->addTimestamp()->add(' ')->printf(format, args...)->add("\r\n");
        if (m_output && !offset) m_output->send(offset);
    }

    /**
     * @brief Adds a formatted message to the log with a timestamp.
     *
     * @param severity Message severity.
     * @param format Text format.
     * @param args... Variadic arguments.
     */
    template<class ...va> static void msg(LogMessage::Severity severity, const char* format, va ...args)
    {
        if (m_output && !m_output->isAvailable()) return;
        auto [message, offset] = getMessage(severity); if (!message) return;
        message->addTimestamp()->add(' ');
        switch (severity)
        {
        case LogMessage::error:
            message->add("ERROR: ");
            break;
        case LogMessage::warning:
            message->add("WARNING: ");
            break;
        case LogMessage::info:
            message->add("INFO: ");
            break;
        default:
            break;
        }
        message->printf(format, args...)->add("\r\n");
        if (m_output && !offset) m_output->send(offset);
    }

    /// @returns Current dump indentation value.
    size_t dumpIndentation()
    {
        return m_dumpIndentation;
    }

    /// @brief Sets the current dump indentation.
    /// @param value The number of text columns to indent the dump lines.
    void dumpIndentation(size_t value)
    {
        m_dumpIndentation = value;
    }

protected:

    LogBase() = delete;
    LogBase(const LogBase&) = delete;
    LogBase& operator=(const LogBase&) = delete;

    /// @returns A tuple of:
    ///             - An empty message from the pool, or `nullptr` when the pool is exhausted.
    ///             - Message pool offset.
    static std::tuple<LogMessage*, int> getMessage(LogMessage::Severity severity = LogMessage::debug)
    {
        if (severity > m_level) return { 0, 0 }; // Don't produce messages above defined severity.
        return m_pool.get(severity);
    }

    static constexpr size_t dumpIndentationDefault = 24; // Default text indentation for the `dump` method.

    static inline LogMessage::Severity m_level = LogMessage::detail;    // Default log level. Messages above this level will be discarded.
    static inline LogMessagePool<TSize> m_pool = {};                    // Static message pool.
    static inline IDebugOutput* m_output = {};                          // Message output implementation.
    static inline size_t m_dumpIndentation = dumpIndentationDefault;    // Current dump line indentation.

};
