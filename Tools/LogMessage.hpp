/**
 * @file        LogMessage.hpp
 * @author      CodeDog
 * @brief       Log message class. Header file.
 *
 * @copyright   (c)2022 CodeDog, All rights reserved.
 */

#pragma once

#define LOG_MESSAGE_SIZE 128

#include <utility>
#include <stdio.h>
#include <string.h>
#include "bindings.h"
EXTERN_C_BEGIN
#include "datetime.h"
EXTERN_C_END

/**
 * @class DebugMessage
 * @brief A tool for creating and storing debug messages.
 */
class LogMessage final
{
public:
    /**
     * @enum Severity
     * @brief Message severity level.
     */
    enum Severity : uint8_t
    {
        error, warning, info, debug, detail, spam
    };

    using Buffer = std::pair<const uint8_t*, int>;

public:

    /// @brief Creates a new log message with the default (debug) severity.
    LogMessage();

    /// @brief Creates a new log message with specified severity.
    LogMessage(Severity s);

    LogMessage(const LogMessage& other);

    LogMessage(LogMessage&& other);

    /// @brief Clears the message.
    void clear();

    /// @returns True if the message is empty / unset.
    bool empty();

    /**
     * @brief Prints formatted text with the argument list.
     *
     * @param format Text format.
     * @param args Variadic arguments.
     * @return Pointer to the message.
     */
    template<class ...va> LogMessage* printf(const char* format, va ...args)
    {
        int l = snprintf((char*)(&m_buffer[m_offset]), size - m_length, format, args...);
        m_offset += l;
        m_length += l;
        return this;
    }

    /**
     * @brief Appends a character to the message.
     *
     * @param c Character to append.
     * @param count Number of characters to append. Default 1.
     * @return Pointer to the message.
     */
    LogMessage* add(char c, int count = 1);

    /**
     * @brief Appeds a string to the message.
     *
     * @param s String to append.
     * @return Pointer to the message.
     */
    LogMessage* add(const char* s);

    /**
     * @brief Adds an ISO8601 timestamp to the message.
     *
     * @return Pointer to the message.
     */
    LogMessage* addTimestamp();

    /// @returns Message's buffer pointer and length in bytes as pair.
    std::pair<const uint8_t*, int> buffer();

    /// @returns Message buffer pointer.
    uint8_t* ptr() { return m_buffer; }

    /// @returns Message length in bytes.
    size_t length() { return m_length; }

    /// @brief Gets the message character at the specified index.
    /// @param index Character index.
    /// @return A pointer to the specific character in the message buffer or null pointer if out of bounds.
    uint8_t* operator[](size_t index) { return index < m_length ? &m_buffer[index] : nullptr; }

private:
    static constexpr int size = LOG_MESSAGE_SIZE;                       // Pre-configured message size in bytes.
    static constexpr const char* dateTimeFormat = ISO_DATE_TIME_MS_F;   // Date format for messages.
    Severity m_severity = debug;                                        // Message severity level.
    size_t m_length = 0;                                                // Current buffer length.
    int m_offset = 0;                                                   // Current message offset.
    uint8_t m_buffer[size]{};                                           // Message buffer.

};
