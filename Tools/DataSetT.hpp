/**
 * @file        DataSetT.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An indexable and iterable, fixed size data set class template. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "IndexIterator.hpp"

/// @brief An indexable and iterable, fixed size data set class template.
/// @tparam TPoint Data point type.
/// @tparam Capacity Number of data points in the set.
template<typename TPoint, int Capacity>
class DataSetT
{

public:

    using ValueType = TPoint;
    using Iterator = IndexIterator<DataSetT, int>;

    /// @brief Creates an empty `DataSetT` instance.
    DataSetT() : m_lastIndex(-1), m_points() { }

    /// @brief Tests if both data sets point to the same data array and have the same last index.
    /// @param other The other data set reference.
    /// @returns 1: The same data set. 0: Different data sets.
    bool operator==(DataSetT& other) const
    {
        return m_points == other.m_points && m_lastIndex == other.m_lastIndex;
    }

    /// @brief Tests if the data sets point to different data arrays or have different last indexes.
    /// @param other The other data set reference.
    /// @returns 1: The data sources are different. 0: The data sources are the same.
    bool operator!=(DataSetT& other) const
    {
        return m_points != other.m_points || m_lastIndex != other.m_lastIndex;
    }

    /// @brief Gets the point reference at specified index. No range checking is made.
    /// @param index Data point index.
    /// @returns Data point reference.
    TPoint& operator[](int index) const { return m_points[index]; }

    /// @brief Gets the point reference at specified index. No range checking is made.
    /// @param index Data point index.
    /// @returns Data point reference.
    TPoint& operator[](int index) { return m_points[index]; }

    /// @brief Returns the data set capacity as the maximum number of points that can be stored in this type.
    /// @returns
    inline static int capacity() { return Capacity; }

    /// @returns The number of the data points actually stored in this instance.
    int length() const { return m_lastIndex + 1; }

    /// @returns The first data point reference.
    TPoint& first() const { return m_points[0]; }

    /// @returns The first data point reference.
    TPoint& first() { return m_points[0]; }

    /// @returns The last data point reference.
    TPoint& last() const { return m_points[m_lastIndex]; }

    /// @returns The last data point reference.
    TPoint& last() { return m_points[m_lastIndex]; }

    /// @returns The first point iterator.
    Iterator begin() const { return Iterator(this, 0); }

    /// @returns The first point iterator.
    Iterator begin() { return Iterator(this, 0); }

    /// @returns The point beyond the last point iterator.
    Iterator end() const { return Iterator(this, m_lastIndex + 1); }

    /// @returns The point beyond the last point iterator.
    Iterator end() { return Iterator(this, m_lastIndex + 1); }

    /// @returns A value indicating that the collection is empty.
    bool empty() const { return m_lastIndex < 0; }

    /// @brief Adds a data point to the collection.
    /// @param point A data point to add.
    virtual void add(TPoint point)
    {
        if (m_lastIndex >= Capacity - 1) return;
        ++m_lastIndex;
        m_points[m_lastIndex] = point;
    }

    /// @brief Copies all the data points from another data set of the same type, zeroes the unused points.
    /// @param other The other data set reference.
    virtual void copyFrom(DataSetT& other)
    {
        for (int i = 0; i <= other.m_lastIndex; i++) m_points[i] = other.m_points[i];
        m_lastIndex = other.m_lastIndex;
        for (int i = m_lastIndex + 1; i < Capacity; i++) m_points[i] = 0;
    }

    /// @brief Copies all the data points to the other data set of the same type, zeroes the unused points of the other set.
    /// @param other The other data set reference.
    virtual void copyTo(DataSetT& other)
    {
        for (int i = 0; i <= m_lastIndex; i++) other.m_points[i] = m_points[i];
        other.m_lastIndex = m_lastIndex;
        for (int i = other.m_lastIndex + 1; i < Capacity; i++) other.m_points[i] = 0;
    }

    /// @brief Resets the collection to the empty state.
    virtual void zero()
    {
        m_lastIndex = -1;
        for (int i = 0; i < Capacity; i++) m_points[i] = 0;
    }

    /// @brief Tests if the other data set contains the same data.
    /// @param other The other data source reference.
    /// @returns 1: Both data sources contain the same data. 0: The other data source contains different data.
    bool contentEqual(DataSetT& other)
    {
        if (*this == other) return true;
        if (m_lastIndex != other.m_lastIndex) return false;
        for (int i = 0; i <= m_lastIndex; i++) if (m_points[i] != other.m_points[i]) return false;
        return true;
    }

private:
    int m_lastIndex = -1;           // Zero based last data point index, -1 if there are no datapoints in the set.
    TPoint m_points[Capacity];      // An array to store data points.

};
