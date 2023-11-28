/**
 * @file        PathPool.hpp
 * @author      CodeDog
 * @brief       Provides pre-allocated file paths pool. Header file.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "Path.hpp"
#include "Pool.hpp"

/// @brief Provides pre-allocated path pool.
class PathPool final : public Pool<32, Path>
{

public:

    /// @returns pool instance singleton.
    static PathPool& getInstance()
    {
        static PathPool instance;
        return instance;
    }

    /// @param item Item pointer.
    /// @returns True if the item is available. False otherwise.
    bool isAvailable(Path* e) const override
    {
        return e && !e->isTaken();
    }


    /// @brief Sets the item as available.
    /// @param item Item pointer.
    /// @param value 1: Available (default), 0: Taken.
    void setAvailable(Path* e, bool value = 1) override
    {
        if (e) e->reset();
        if (!value) e->markTaken();
    }

};

/// @brief A RAII wrapper over the `Path` from the `PathPool`.
class PoolPath final : public PoolItem<Path>
{

public:

    /// @brief Takes a new empty path from the pool.
    PoolPath() : PoolItem<Path>(PathPool::getInstance()) { }

    /// @brief Takes a new path from the pool and sets it to a specific value.
    /// @param value The initial path value.
    PoolPath(const char* value) : PoolItem<Path>(PathPool::getInstance()) { ref().add(value); }

    /// @brief Takes a new path from the pool and copies another path's value into it.
    /// @param path Path to copy the value from.
    PoolPath(const Path& path) : PoolItem<Path>(PathPool::getInstance()) { ref().copy(path); }

    /// @brief Takes a new path from the pool and copies another path's value into it.
    /// @param path Path to copy the value from.
    PoolPath(const PoolPath& path) : PoolItem<Path>(PathPool::getInstance()) { ref().copy(path.ref()); }

};
