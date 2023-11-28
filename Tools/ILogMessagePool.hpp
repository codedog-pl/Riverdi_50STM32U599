#pragma once

#include "LogMessage.hpp"
#include <cstddef>
#include <tuple>

/// @brief An interface for a log message pool.
class ILogMessagePool
{

public:

    /// @returns The message pool capacity.
    virtual size_t size() = 0;

    /// @brief Gets an empty, initialized message from the pool if available.
    /// @param severity Message severity to request.
    /// @returns A tuple of:
    ///             - Message pointer or `nullptr` if the pool is exhausted.
    ///             - The index of the message in the pool.
    virtual std::tuple<LogMessage*, int> get(LogMessage::Severity severity = LogMessage::debug) = 0;

    /// @brief Resets the pool with setting the last index to -1, to start adding new messages to the beginning of the pool.
    virtual void clear() = 0;

    /// @returns The index of the last message taken from the pool.
    virtual int lastIndex() = 0;

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    virtual const LogMessage* operator[](size_t index) const = 0;

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    virtual LogMessage* operator[](size_t index) = 0;

};