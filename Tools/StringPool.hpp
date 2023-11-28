/**
 * @file        StringPool.hpp
 * @author      CodeDog
 * @brief       Provides pre-allocated short string pool. Header file.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "TextBuffer.hpp"
#include "Pool.hpp"

/// @brief A short static C string buffer.
using CString64 = TextBuffer<64>;

/// @brief Provides pre-allocated short string pool.
class StringPool final : public Pool<16, CString64>
{

public:

    /// @returns pool instance singleton.
    static StringPool& getInstance()
    {
        static StringPool instance;
        return instance;
    }

    /// @param item Item pointer.
    /// @returns True if the item is available. False otherwise.
    bool isAvailable(CString64* e) const override
    {
        return !e->isTaken();
    }

    /// @brief Sets the item as available.
    /// @param item Item pointer.
    /// @param value 1: Available (default), 0: Taken.
    void setAvailable(CString64* e, bool value = 1) override
    {
        e->clear();
        if (!value) e->markTaken();
    }

};

/// @brief A RAII wrapper over the `CString64` from the `StringPool`.
class PoolString final : public PoolItem<CString64>
{

public:

    /// @brief Takes a new empty string from the pool.
    PoolString() : PoolItem<CString64>(StringPool::getInstance()) { }

    /// @brief Takes a new string from the pool and sets it to a specific value.
    /// @tparam ...va Variadic arguments type.
    /// @param value C string to set or the format for variadic arguments.
    /// @param ...args Optional variadic arguments to interpolate the string.
    template<class ...va>
    PoolString(const char* value, va ...args) : PoolItem<CString64>(StringPool::getInstance()) { ref().printf(value, args...); }

    /// @brief Takes a new string from the pool and copies another `CString64` value into it.
    /// @param value `CString64` to copy the value from.
    PoolString(const CString64& value) : PoolItem<CString64>(StringPool::getInstance()) { ref().printf(value.c_str()); }

    /// @brief Takes a new string from the pool and copies another `PoolString` value into it.
    /// @param value `PoolString` to copy the value from.
    PoolString(const PoolString& value) : PoolItem<CString64>(StringPool::getInstance()) { ref().printf(value.ref().c_str()); }

};
