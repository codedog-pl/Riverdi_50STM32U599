/**
 * @file        Range.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Universal floating points numbers iterator. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <cmath>
#include <limits>

/// @brief Iterates between a range of numbers.
class RangeIterator
{

public:

    /// @brief Creates the number iterator instance.
    /// @param start A value that starts the sequence.
    /// @param end A value that ends the sequence.
    /// @param numValues The number of values in the sequence.
    RangeIterator(double start, double end, size_t numValues) :
        m_start(start),
        m_end(end),
        m_numValues(numValues),
        m_step((end - start) / static_cast<double>(numValues - 1ul)),
        m_current(numValues > 1 ? start : end),
        m_currentIndex(0) { }

    /// @brief Creates a zero iterator that doesn't produce anything.
    RangeIterator() :
        m_start(0),
        m_end(0),
        m_numValues(0),
        m_step(0),
        m_current(0),
        m_currentIndex(0) { }

    /// @brief Gets the current index of the iterator.
    /// @return Current index value.
    size_t index() { return m_currentIndex; }

    /// @brief Implements value pre-incrementation.
    /// @return This reference.
    RangeIterator& operator++()
    {
        m_current = (m_currentIndex < m_numValues - 1)
            ? m_start + m_step * ++m_currentIndex
            : m_end;
        return *this;
    }

    /// @brief Implements value post-incrementation.
    /// @return Copy of the iterator before incrementation.
    RangeIterator operator++(int)
    {
        RangeIterator temp = *this;
        ++(*this);
        return temp;
    }

    /// @brief Tests if the instances of the iterator are equal.
    /// @param other Other iterator instance reference.
    /// @return True if the instances are equal.
    bool operator==(const RangeIterator& other) const
    {
        return
            std::fabs(m_current - other.m_current) <= std::numeric_limits<double>::epsilon() * std::fabs(m_current);
    }

    /// @brief Tests if the instances of the iterator are NOT equal.
    /// @param other Other iterator instance reference.
    /// @return True if the instances are NOT equal.
    bool operator!=(const RangeIterator& other) const
    {
        return !(*this == other);
    }

    /// @return The address of the item.
    double operator*() const
    {
        return m_current;
    }

private:

    double m_start;         // A value that starts the sequence.
    double m_end;           // A value that ends the sequence.
    size_t m_numValues;     // The number of values in the sequence.
    double m_step;          // A step value between the sequence elements.
    double m_current;       // Current value.
    size_t m_currentIndex;  // Current value index.

};

/// @brief Generates a sequence of `numValues` equally distributed values from `start` to `end` inclusive.
class Range
{

public:

    /// @brief Generates a sequence of `numValues` equally distributed values from `start` to `end` inclusive.
    /// @param start First generated value.
    /// @param end Last generated value.
    /// @param numValues Number of generated values.
    Range(double start, double end, size_t numValues) :
        m_start(start),
        m_step((end - start) / static_cast<double>(numValues - 1ul)),
        m_end(end),
        m_numValues(numValues) { }

    /// @returns The first term iterator.
    RangeIterator begin() const
    {
        return RangeIterator(m_start, m_end + m_step, m_numValues + 1);
    }

    /// @returns The "last + 1" term iterator.
    RangeIterator end() const
    {
        return (m_numValues > 0)
            ? RangeIterator(m_end + m_step, m_end + m_step, m_numValues + 1)
            : begin();
    }

private:
    double m_start;     // First value.
    double m_step;      // Increment step.
    double m_end;       // Last value.
    size_t m_numValues; // Number of values.

};
