/**
 * @file        ILogMessagePool.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Declares an interface for the system log message pool.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "LogMessage.hpp"
#include <cstddef>
#include <tuple>

/// @brief An interface for a log message pool.
class ILogMessagePool
{

public:

    ILogMessagePool() : m_lastIndex(-1), m_lastSent(0) { }

    /// @returns The message pool capacity.
    virtual size_t size() = 0;

    /// @brief Gets an empty, initialized message from the pool if available.
    /// @param severity Message severity to request.
    /// @returns A tuple of:
    ///             - Message pointer or `nullptr` if the pool is exhausted.
    ///             - The index of the message in the pool.
    virtual std::tuple<LogMessage*, int> get(LogMessage::Severity severity = LogMessage::debug) = 0;

    /// @brief Resets the pool with setting the last index to -1, to start adding new messages to the beginning of the pool.
    void clear()
    {
        m_lastIndex = -1;
        m_lastSent = -1;
    }

    /// @returns The index of the last message taken from the pool.
    int lastIndex()
    {
        return m_lastIndex;
    }

    /// @returns The last message sent pointer.
    LogMessage* lastSent()
    {
        return m_lastSent < 0 ? nullptr : (*this)[m_lastSent];
    }

    /// @returns The index of the last message that was sent from the pool.
    int lastSentIndex()
    {
        return m_lastSent;
    }

    /// @brief Sets the last sent message index.
    /// @returns The new index of the last message that was sent from the pool.
    int lastSentIndex(int newIndex)
    {
        m_lastSent = newIndex;
        return m_lastSent;
    }

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    virtual const LogMessage* operator[](size_t index) const = 0;

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    virtual LogMessage* operator[](size_t index) = 0;

protected:
    int m_lastIndex;                // Last index used, -1 if no message was taken.
    int m_lastSent;                 // Last sent message index.

};
