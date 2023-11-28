#pragma once

#include "Pool.hpp"

/// @brief AsyncResult instance generic placeholder for the pool.
class AsyncResultGeneric
{
public:

    /// @brief Creates an empty, available instance.
    AsyncResultGeneric() : m_success(), m_failure() { }

    /// @returns A value indicating that the instance is empty and available.
    inline bool isAvailable()
    {
        return !m_success && !m_failure;
    }

    /// @brief Resets the instance to be available.
    inline void setAvailable(bool value = 1)
    {
        if (value)
        {
            m_success = 0;
            m_failure = 0;
        }
        else
        {
            m_success = (void*)sentinel;
            m_failure = (void*)sentinel;
        }
    }

private:

    void* m_success;    // Successful completion function pointer.
    void* m_failure;    // Error completion function pointer.

public:

    /// @brief  A sentinel function to set freshly taken pointers to.
    ///         It does nothing, but is not optimized out.
    static void sentinel()
    {
        volatile int doNotOptimizeAway = 0;
        (void)doNotOptimizeAway;
    }

};

/// @brief Provides pre-allocated asynchronous results pool.
class AsyncPool final : public Pool<64, AsyncResultGeneric>
{

public:

    /// @returns pool instance singleton.
    static AsyncPool& getInstance()
    {
        static AsyncPool instance;
        return instance;
    }

    /// @param item Item pointer.
    /// @returns True if the item is available. False otherwise.
    bool isAvailable(AsyncResultGeneric* e) const override
    {
        return e->isAvailable();
    }

    /// @brief Sets the item as available.
    /// @param item Item pointer.
    /// @param value 1: Available (default), 0: Taken.
    void setAvailable(AsyncResultGeneric* e, bool value = 1) override
    {
        e->setAvailable(value);
    }

};
