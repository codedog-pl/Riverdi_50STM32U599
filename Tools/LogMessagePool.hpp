#pragma once

#include "ILogMessagePool.hpp"
#include <new>
#include <tuple>

/// @brief Provides a preallocated log message pool that works as a queue.
/// @tparam TNum Pool capacity.
template<int TSize>
class LogMessagePool final : public ILogMessagePool
{

public:

    LogMessagePool() : m_messages(), m_lastIndex(-1) { }

    /// @returns The message pool capacity.
    size_t size() override { return TSize; }

    /// @brief Gets an empty, initialized message from the pool if available.
    /// @param severity Message severity to request.
    /// @returns A tuple of:
    ///             - Message pointer or `nullptr` if the pool is exhausted.
    ///             - The index of the message in the pool.
    std::tuple<LogMessage*, int> get(LogMessage::Severity severity = LogMessage::debug) override
    {
        int offset = m_lastIndex + 1;
        if (offset >= TSize) return { 0, offset }; // Return `nullptr` with the capacity value when none available.
        return { new(&m_messages[m_lastIndex = offset]) LogMessage(severity), offset };
    }

    /// @brief Resets the pool with setting the last index to -1, to start adding new messages to the beginning of the pool.
    inline void clear() override
    {
        m_lastIndex = -1;
    }

    /// @returns The index of the last message taken from the pool.
    inline int lastIndex() override
    {
        return m_lastIndex;
    }

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    inline const LogMessage* operator[](size_t index) const override { return index < TSize ? &m_messages[index] : nullptr; }

    /// @returns The element pointer at index, or nullptr on index out of bounds.
    inline LogMessage* operator[](size_t index) override { return index < TSize ? &m_messages[index] : nullptr; }

private:

    LogMessage m_messages[TSize];   // Messages array.
    int m_lastIndex;                // Last index used, -1 if no message was taken.

};
