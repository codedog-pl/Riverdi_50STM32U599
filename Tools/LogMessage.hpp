/**
 * @file        LogMessage.hpp
 * @author      CodeDog
 *
 * @brief       Log message class. Header only.
 *
 * @copyright   (c)2022 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <utility>
#include "bindings.h"
#include "target.h"
#include "DateTimeEx.hpp"
EXTERN_C_BEGIN
#include "datetime.h"
EXTERN_C_END

/// @brief System log message class.
class LogMessage final
{

public:

    /// @brief Message severity level.
    enum Severity : uint8_t { error, warning, info, debug, detail, spam };

    /// @brief Contains packed buffer pointer and size.
    using Buffer = std::pair<const uint8_t*, size_t>;

public:

    /// @brief Creates a new log message with the default (debug) severity.
    LogMessage()
        : m_severity(debug), m_length(0), m_offset(0), m_buffer() { memset(m_buffer, 0, size); }

    /// @brief Creates a new log message with specified severity.
    LogMessage(Severity s)
        : m_severity(s), m_length(0), m_offset(0), m_buffer() { memset(m_buffer, 0, size); }

    /// @brief Copies another message.
    /// @param other The other message reference.
    LogMessage(const LogMessage& other)
        : m_severity(other.m_severity), m_length(other.m_length), m_offset(other.m_offset), m_buffer()
    {
        memcpy(m_buffer, other.m_buffer, m_length);
    }

    LogMessage(LogMessage&& other) = delete; // Instances should not be moved.

    /// @brief Clears the message.
    void clear()
    {
        m_offset = 0;
        m_length = 0;
        memset(m_buffer, 0, size);
    }

    /// @returns True if the message is empty / unset.
    bool empty() { return !m_offset || !m_length; }


    /// @brief Prints a formatted message into the message buffer.
    /// @tparam ...va Variadic arguments type.
    /// @param format Text format.
    /// @param ...args Variadic arguments.
    /// @return A pointer to the message.
    template<class ...va> LogMessage* printf(const char* format, va ...args)
    {
        int l = snprintf((char*)(&m_buffer[m_offset]), size - m_length, format, args...);
        m_offset += l;
        m_length += l;
        return this;
    }

    /// @brief Appends a character to the message.
    /// @param c The character to append.
    /// @param count Number of characters to append. Default 1.
    /// @return A poiner to the message.
    LogMessage* add(char c, int count = 1)
    {
        if (m_length + count > size) return this;
        for (int i = 0; i < count; i++)
        {
            m_buffer[m_offset] = c;
            m_offset++;
            m_length++;
        }
        return this;
    }

    /// @brief Appeds a string to the message.
    /// @param s String to append.
    /// @return A pointer to the message.
    LogMessage* add(const char* s)
    {
        size_t l = strlen(s);
        if (m_length + l > size) return this;
        memcpy(&m_buffer[m_offset], s, l);
        m_offset += l;
        m_length += l;
        return this;
    }

    /// @brief Adds an ISO8601 timestamp to the message.
    /// @returns A pointer to the message.
    LogMessage* addTimestamp()
    {
        DateTimeEx timestamp;
        if (timestamp.getRTC())
            return printf(
                dateTimeFormat,
                timestamp.year, timestamp.month, timestamp.day,
                timestamp.hour, timestamp.minute, timestamp.second + timestamp.fraction
            );
        else return this->add('*');
    }

    /// @returns Message's buffer pointer and length in bytes as pair.
    Buffer buffer() { return { (const uint8_t*)&m_buffer, m_length }; }

    /// @returns Message buffer pointer.
    uint8_t* ptr() { return m_buffer; }

    /// @returns Message length in bytes.
    size_t length() { return m_length; }

    /// @brief Gets the message character at the specified index.
    /// @param index Character index.
    /// @return A pointer to the specific character in the message buffer or null pointer if out of bounds.
    uint8_t* operator[](size_t index) { return index < m_length ? &m_buffer[index] : nullptr; }

private:
    static constexpr int size = WTK_LOG_MSG_SIZE;                       // Pre-configured message size in bytes.
    static constexpr const char* dateTimeFormat = ISO_DATE_TIME_MS_F;   // Date format for messages.
    Severity m_severity = debug;                                        // Message severity level.
    size_t m_length = 0;                                                // Current buffer length.
    int m_offset = 0;                                                   // Current message offset.
    uint8_t m_buffer[size]{};                                           // Message buffer.

};
