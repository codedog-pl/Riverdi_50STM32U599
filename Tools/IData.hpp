/**
 * @file        IData.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Declares an interface to use virtual data array.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include <cstring>
#include <type_traits>

/// @brief Provides an interface to use virtual data array.
/// @tparam TElement Element type.
template<typename TElement>
class IData
{

public:

    /// @returns The number of elements the backing array contains.
    virtual const size_t size() const = 0;

protected:

    /// @returns The pointer to the backing array.
    virtual TElement* data() = 0;

    /// @returns The read only pointer to the backing array.
    virtual const TElement* data() const = 0;

    /// @returns The element pointer at index, or nullptr on invalid index.
    virtual TElement* operator[](size_t index) = 0;

    /// @returns The element pointer at index, or nullptr on invalid index.
    virtual const TElement* operator[](size_t index) const = 0;

    /// @returns The element reference at index, or a default instance (sentinel) reference on invalid index.
    virtual TElement& at(size_t index) = 0;

    /// @returns The element reference at index, or a default instance (sentinel) reference on invalid index.
    virtual const TElement& at(size_t index) const = 0;

};

/// @brief Provides static element data storage for a derived class.
/// @tparam TSize Number of elements.
/// @tparam TElement Element type.
template<size_t TSize, typename TElement>
class StaticData : public virtual IData<TElement>
{

public:

    /// @returns The number of elements the backing array contains.
    const size_t size() const override { return TSize; }

    /// @returns The pointer to the backing array.
    inline TElement* data() override { return m_data; }

    /// @returns The read only pointer to the backing array.
    inline const TElement* data() const override { return m_data; }

protected:

    /// @brief Initializes the backing array.
    StaticData() : m_data() { }

    /// @brief Initializes the backing array with a copy of the other storage.
    /// @param other The other data storage.
    StaticData(const StaticData& other) : m_data()
    {
        static_assert(std::is_trivially_copyable_v<TElement>, "TElement must be trivially copyable");
        memcpy(m_data, other.m_data, TSize * sizeof(TElement));
    }

    /// @brief This type is not moveable.
    StaticData(StaticData&&) = delete;

    /// @returns The element pointer at index, or nullptr if index out of bounds.
    inline TElement* operator[](size_t index) override { return index < TSize ? &m_data[index] : nullptr; }

    /// @returns The element pointer at index, or nullptr if index out of bounds.
    inline const TElement* operator[](size_t index) const override { return index < TSize ? &m_data[index] : nullptr; }

    /// @returns The element reference at index, or a default instance (sentinel) reference on invalid index.
    inline TElement& at(size_t index) override
    {
        static TElement sentinel{};
        return index < TSize ? m_data[index] : sentinel;
    }

    /// @returns The element reference at index, or a default instance (sentinel) reference on invalid index.
    inline const TElement& at(size_t index) const override {
        static TElement sentinel{};
        return index < TSize ? m_data[index] : sentinel;
    }

    /// @brief Elements' storage backing array.
    TElement m_data[TSize];

};
