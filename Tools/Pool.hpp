/**
 * @file        Pool.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides generic pre-allocated item pool API with RAII item wrapper. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @remarks     Intended to use on embedded systems to avoid dynamic memory allocations.
 *              Pool sizes must be set at compile time.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <functional>
#include "IData.hpp"

/// @brief Pool error codes enumeration.
enum class PoolErrorCode
{
    E_NONE,             // No error occurred.
    E_POOL_EXHAUSTED,   // Pool was exhausted. Too many items requested without putting back.
    E_INVALID_RETURN    // An invalid pointer was about to be returned to the pool.
};

/// @brief A function acceptin a poll error code.
using PoolErrorHandler = std::function<void(PoolErrorCode)>;

/// @brief An interface for an item pool allowing taking items and then putting them back.
/// @tparam TItem Item type.
template<typename TItem>
class IPool
{
public:

    /// @returns A first available item from the pool or `nullptr` if the pool is exhausted.
    virtual TItem* take() = 0;

    /// @brief Returns the item to the pool.
    /// @param item Item pointer.
    virtual void putBack(TItem* item) = 0;

    /// @returns The number of available items.
    virtual int available() = 0;

    /// @brief Resets the pool to the initial state.
    /// @warning Only for testing purposes, do not use when instances are used!
    virtual void reset() = 0;

    /// @brief Register an error handler that receives an error code.
    /// @param errorHandler A function accepting an error code.
    virtual void registerErrorHandler(PoolErrorHandler errorHandler) = 0;

};

/// @brief Generic item pool template.
/// @tparam TSize Number of items.
/// @tparam TItem Item type.
template<int TSize, typename TItem>
class Pool : public StaticData<TSize, TItem>, public IPool<TItem>
{

public:

    // static constexpr int size = TNum; // Maximal number of items that can be taken from the pool.

    /// @returns A first available item from the pool or `nullptr` if the pool is exhausted.
    TItem* take() override
    {
        for (size_t i = 0; i < this->size(); ++i) if (isAvailable((*this)[i]))
        {
            setAvailable((*this)[i], 0);
            return (*this)[i];
        }
        if (m_error) m_error(PoolErrorCode::E_POOL_EXHAUSTED);
        return nullptr;
    }

    /// @brief Returns the item to the pool.
    /// @param item Item pointer.
    void putBack(TItem* item) override
    {
        if (!item)
        {
            if (m_error) m_error(PoolErrorCode::E_INVALID_RETURN);
            return;
        }
        for (size_t i = 0; i < this->size(); ++i) if (item == (*this)[i])
        {
            setAvailable(item);
            return;
        }
        if (m_error) m_error(PoolErrorCode::E_INVALID_RETURN);
    }

    /// @returns The number of available items.
    int available() override
    {
        int count = 0;
        for (size_t i = 0; i < this->size(); ++i) if (isAvailable((*this)[i])) ++count;
        return count;
    }

    /// @brief Resets the pool to the initial state.
    /// @warning Only for testing purposes, do not use when instances are used!
    void reset() override
    {
        for (size_t i = 0; i < this->size(); i++) setAvailable((*this)[i]);
    }

    void registerErrorHandler(PoolErrorHandler errorHandler) override
    {
        m_error = errorHandler;
    }

    /// @param item Item pointer.
    /// @returns True if the item is available. False otherwise.
    virtual bool isAvailable(TItem* item) const = 0;

    /// @brief Sets the item as available.
    /// @param item Item pointer.
    /// @param value 1: Available (default), 0: Taken.
    virtual void setAvailable(TItem* item, bool value = 1) = 0;

protected:
    PoolErrorHandler m_error{}; // Optional error handler function.

};

/// @brief RAII container for pool items. When object of this type goes out of scope, it's returned to the pool.
/// @tparam TItem Item type.
template<typename TItem>
class PoolItem
{
public:

    /// @brief Gets an item from the pool.
    /// @param pool Item pool reference.
    PoolItem(IPool<TItem>& pool) : m_pool(&pool), m_instance(pool.take()) { }

    /// @brief This is move only type.
    PoolItem(const PoolItem&) = delete;

    /// @brief Moves a pool item.
    /// @param other Source pool item rvalue.
    PoolItem(PoolItem&& other) noexcept : m_pool(other.m_pool), m_instance(other.m_instance) { other.m_pool = nullptr; other.m_instance = nullptr; }

    /// @brief Returns the item to the pool.
    virtual ~PoolItem() { if (m_pool) m_pool->putBack(m_instance); }

    /// @returns The pool item pointer.
    TItem* ptr() const { return m_instance; }

    /// @returns The pool item reference.
    TItem& ref() const { return *m_instance; }

    /// @returns The pool item pointer.
    operator TItem* () const { return m_instance; }

    /// @returns The pool item reference.
    operator TItem& () const { return *m_instance; }

protected:
    IPool<TItem>* m_pool;   // Source pool reference.
    TItem* m_instance;      // Item pointer.
};
