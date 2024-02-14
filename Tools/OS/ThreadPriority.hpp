/**
 * @file        ThreadPriority.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Special thread priority value type that supports various conversions,
 *              RTOS depenedent ordering (comparison) and presets. Header only.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "RTOS.hpp"

namespace OS
{

/// @brief  Special thread priority value type that supports various conversions,
///         RTOS depenedent ordering (comparison) and presets.
struct ThreadPriority final
{

#if defined(USE_AZURE_RTOS)

    /// @brief Azure RTOS priority presets, where the highest (realtime) priority is zero.
    enum Preset : NativePriority
    {
        idle          = TX_MAX_PRIORITIES - 1,
        low           = (TX_MAX_PRIORITIES >> 1) * 5 / 3,
        belowNormal   = (TX_MAX_PRIORITIES >> 1) * 4 / 3,
        normal        = (TX_MAX_PRIORITIES >> 1) - 1,
        aboveNormal   = (TX_MAX_PRIORITIES >> 1) * 2 / 3,
        high          = (TX_MAX_PRIORITIES >> 1) * 1 / 3,
        realtime      = 0
    };

#elif defined(USE_FREE_RTOS)

    /// @brief FreeRTOS priority presets, where the lowest (idle) priority is 1.
    enum Preset : NativePriority
    {
        none          =  0,         // No priority (not initialized).
        idle          =  1,         // Reserved for Idle thread.
        low           =  8,         // Priority: low
        belowNormal   = 16,         // Priority: below normal
        normal        = 24,         // Priority: normal
        aboveNormal   = 32,         // Priority: above normal
        high          = 40,         // Priority: high
        realtime      = 48,         // Priority: realtime
        ISR           = 56,         // Reserved for ISR deferred thread.
        error         = static_cast<NativePriority>(-1), // System cannot determine priority or illegal priority.
        reserved      = 0x7FFFFFFF  // Prevents enum down-size compiler optimization.
    };

#endif

    /// @brief Creates a default thread priority level (normal).
    ThreadPriority() : m_value(normal) { }

    /// @brief Creates a thread priority level from an integer number.
    /// @param value Numeric priority value.
    ThreadPriority(int value) : m_value(value) { }

    /// @brief Creates a thread priority level from a native RTOS integer type.
    /// @param value Native priority value.
    ThreadPriority(NativePriority value) : m_value(static_cast<int>(value)) { }

    /// @brief Creates a thread priority level from a preset value.
    /// @param preset One of the preset enumeration members.
    ThreadPriority(Preset preset) : m_value(static_cast<int>(preset)) { }

    /// @brief Implicitly converts the priority level to an integer number.
    inline operator int() const { return m_value; }

    /// @brief Implicitly converts the priority level to the native RTOS integer type.
    inline operator NativePriority() const { return static_cast<NativePriority>(m_value); }

    /// @brief Implicitly converts the priority level to a preset type (it might not correspond to any enumeration member though).
    inline operator Preset() const { return static_cast<Preset>(m_value); }

#if defined(USE_AZURE_RTOS)

    /// @brief Tests if this priority is lower (closer to idle) than the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is lower (closer to idle).
    inline bool operator <(const ThreadPriority& other) const { return m_value > other.m_value; }

    /// @brief Tests if this priority is lower (closer to idle) or equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is lower (closer to idle) or equal.
    inline bool operator <=(const ThreadPriority& other) const { return m_value >= other.m_value; }

    /// @brief Tests if this priority level is equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority level is equal to the other priority.
    inline bool operator ==(const ThreadPriority& other) const { return m_value == other.m_value; }

    /// @brief Tests if this priority level is NOT equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority level is NOT equal to the other priority.
    inline bool operator !=(const ThreadPriority& other) const { return m_value != other.m_value; }

    /// @brief Tests if this priority is higher (closer to realtime) or equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is higher (closer to realtime) or equal.
    inline bool operator >=(const ThreadPriority& other) const { return m_value <= other.m_value; }

    /// @brief Tests if this priority is higher (closer to realtime) than the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is higher (closer to realtime).
    inline bool operator >(const ThreadPriority& other) const { return m_value < other.m_value; }

    /// @brief Pre-decrements the priority of the thread towards the idle.
    /// @returns This (decreased) priority reference.
    inline ThreadPriority& operator--()
    {
        if (*this < max()) ++m_value;
        return *this;
    }

    /// @brief Post-decrements the priority of the thread towards the idle.
    /// @returns A priority copy taken before it was decreased.
    inline ThreadPriority operator--(int)
    {
        ThreadPriority old = *this;
        if (*this < max()) ++m_value;
        return old;
    }

    /// @brief  Subtracts a value from the priority of the thread,
    ///         towards the idle for positive integers, towards the realtime for negative ones.
    /// @param value A priority level to subtract.
    /// @returns This priority reference.
    inline ThreadPriority& operator-=(int value)
    {
        m_value = cap(m_value + value);
        return *this;
    }

    /// @brief  Subtracts a value from the priority of the thread,
    ///         towards the idle for positive integers, towards the realtime for negative ones.
    /// @param value A priority level to subtract.
    /// @returns A result priority.
    inline ThreadPriority operator-(int value) const
    {
        return cap(m_value + value);
    }

    /// @brief  Adds a value to the priority of the thread,
    ///         towards the realtime for positive integers, towards the idle for negative ones.
    /// @param value A priority level to add.
    /// @returns A result priority.
    inline ThreadPriority operator+(int value) const
    {
        return cap(m_value - value);
    }

    /// @brief  Adds a value to the priority of the thread,
    ///         towards the realtime for positive integers, towards the idle for negative ones.
    /// @param value A priority level to add.
    /// @returns This priority reference.
    inline ThreadPriority& operator+=(int value)
    {
        m_value = cap(m_value - value);
        return *this;
    }

    /// @brief Post-increments the priority of the thread towards the realtime.
    /// @returns A priority copy taken before it was increased.
    inline ThreadPriority operator++(int)
    {
        ThreadPriority old = *this;
        if (*this > min()) --m_value;
        return old;
    }

    /// @brief Pre-increments the priority of the thread towards the realtime.
    /// @returns This (increased) priority reference.
    inline ThreadPriority& operator++()
    {
        if (*this > min()) --m_value;
        return *this;
    }

#elif defined(USE_FREE_RTOS)

    /// @brief Tests if this priority is lower (closer to idle) than the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is lower (closer to idle).
    inline bool operator <(const ThreadPriority& other) const { return m_value < other.m_value; }

    /// @brief Tests if this priority is lower (closer to idle) or equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is lower (closer to idle) or equal.
    inline bool operator <=(const ThreadPriority& other) const { return m_value <= other.m_value; }

    /// @brief Tests if this priority level is equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority level is equal to the other priority.
    inline bool operator ==(const ThreadPriority& other) const { return m_value == other.m_value; }

    /// @brief Tests if this priority level is NOT equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority level is NOT equal to the other priority.
    inline bool operator !=(const ThreadPriority& other) const { return m_value == other.m_value; }

    /// @brief Tests if this priority is higher (closer to realtime) or equal to the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is higher (closer to realtime) or equal.
    inline bool operator >=(const ThreadPriority& other) const { return m_value >= other.m_value; }

    /// @brief Tests if this priority is higher (closer to realtime) than the other priority.
    /// @param other The other priority level to compare with.
    /// @return True if this priority is higher (closer to realtime).
    inline bool operator >(const ThreadPriority& other) const { return m_value > other.m_value; }

    /// @brief Pre-decrements the priority of the thread towards the idle.
    /// @returns This (decreased) priority reference.
    inline ThreadPriority& operator--()
    {
        if (*this > min()) --m_value;
        return *this;
    }

    /// @brief Post-decrements the priority of the thread towards the idle.
    /// @returns A priority copy taken before it was decreased.
    inline ThreadPriority operator--(int)
    {
        ThreadPriority old = *this;
        if (*this > min()) --m_value;
        return old;
    }

    /// @brief  Subtracts a value from the priority of the thread,
    ///         towards the idle for positive integers, towards the realtime for negative ones.
    /// @param value A priority level to subtract.
    /// @returns This priority reference.
    inline ThreadPriority& operator-=(int value)
    {
        m_value = cap(m_value - value);
        return *this;
    }

    /// @brief  Subtracts a value from the priority of the thread,
    ///         towards the idle for positive integers, towards the realtime for negative ones.
    /// @param value A priority level to subtract.
    /// @returns A result priority.
    inline ThreadPriority operator-(int value) const
    {
        return cap(m_value - value);
    }

    /// @brief  Adds a value to the priority of the thread,
    ///         towards the realtime for positive integers, towards the idle for negative ones.
    /// @param value A priority level to add.
    /// @returns A result priority.
    inline ThreadPriority operator+(int value) const
    {
        return cap(m_value + value);
    }

    /// @brief  Adds a value to the priority of the thread,
    ///         towards the realtime for positive integers, towards the idle for negative ones.
    /// @param value A priority level to add.
    /// @returns This priority reference.
    inline ThreadPriority& operator+=(int value)
    {
        m_value = cap(m_value + value);
        return *this;
    }

    /// @brief Post-increments the priority of the thread towards the realtime.
    /// @returns A priority copy taken before it was increased.
    inline ThreadPriority operator++(int)
    {
        ThreadPriority old = *this;
        if (*this < max()) ++m_value;
        return old;
    }

    /// @brief Pre-increments the priority of the thread towards the realtime.
    /// @returns This (increased) priority reference.
    inline ThreadPriority& operator++()
    {
        if (*this < max()) ++m_value;
        return *this;
    }

#endif

private:

    /// @brief Caps the priority from both sides of the `idle` to `realtime` range.
    /// @param value Numeric (integer) thread priority value.
    /// @returns A thread priority within `idle` to `realtime`
    static ThreadPriority cap(int value)
    {
        ThreadPriority priority = value;
        if (priority < min()) priority = min();
        else if (priority > max()) priority = max();
        return priority;
    }

    int m_value; // Numeric (integer) thread priority value.
    static inline ThreadPriority min() { return idle; } // Minimal usable thread priority.
    static inline ThreadPriority max() { return realtime; } // Maximal usable thread priority.

};

}
