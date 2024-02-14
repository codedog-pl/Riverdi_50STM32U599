/**
 * @file        HistoryList.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A collection of a fixed amount of history elements
 *              that can be added, removed and iterated from the most recent one.
 *              Setting the page size allows the standard iterator to iterate over a subset of elements.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include <new>
#include <type_traits>
#include "IndexIterator.hpp"

 /**
  * @brief A history (stack-like) list providing IndexIterator.
  *
  * @tparam TNum Capacity.
  * @tparam TItem Element type.
  */
template<int TNum, typename TItem>
class HistoryList
{

public: // Type aliases:

    using ThisType = HistoryList<TNum, TItem>;
    using ValueType = TItem;
    using Iterator = IndexIterator<ThisType>;

public: // Static:

    static const unsigned int capacity = TNum; ///< Fixed list capacity.

public: // API:

    /// @brief Creates a new empty history list.
    HistoryList() : m_length(0), m_offset(-1), m_elements() { }

    /// @returns The current length of the list.
    inline unsigned int length() const { return m_length; }

    /// @brief Resets the history list.
    void reset()
    {
        m_length = 0;
        m_offset = -1;
        trim();
    }

    /// @returns A reference to a new element.
    TItem& add(void)
    {
        ++m_length;
        if (m_length > capacity) m_length = capacity;
        m_offset = (m_offset + 1) % capacity;
        return m_elements[m_offset];
    }

    /// @returns True if there are any elements in the list.
    inline bool any() const { return m_length > 0; }

    /// @brief Removes the last added element from the list.
    /// @returns The previously added element's reference or the first (empty) element's reference.
    TItem& back()
    {
        if (m_length > 1)
        {
            resetElement(m_elements[m_offset]);
            --m_length;
            m_offset = m_offset > 0 ? m_offset - 1 : capacity - 1;
        }
        return current();
    }

    /// @returns The last added element's reference or the first (empty) element's reference.
    TItem& current() const { return m_offset >= 0 ? m_elements[m_offset] : m_elements[0]; }

    /// @brief Gets the reference to the element at the specified history level.
    /// @param level A history level, 0 is the most recent (current).
    /// @returns A reference to the element at the specified level.
    TItem& operator[](int level) { return m_elements[(capacity + m_offset - level) % capacity]; }

public: // Iterator API:

    /**
     * @brief   Gets the iterator for the first (most recently added) item.
     *
     * @returns Iterator.
     */
    Iterator begin()
    {
        return Iterator(this, 0);
    }

    /**
     * @brief   Gets the iterator for the one over the last (non existing) item.
     *
     * @returns Iterator.
     */
    Iterator end()
    {
        return Iterator(this, m_length);
    }

protected: // Internal API (for storage implementation):

    /**
     * @brief   Resets the internal array element if the element's destructor can be called.
     *
     * @tparam  U Element type for the template.
     * @param   element Internal array element reference.
     */
    template <typename U>
    typename std::enable_if<!std::is_trivially_destructible<U>::value>::type
        resetElement(U& element) {
        element.~U();
        new (&element) U();
    }

    /**
     * @brief   Resets the internal array element if the element's destructor cannot be called.
     *
     * @tparam  U Element type for the template.
     * @param   element Internal array element reference.
     */
    template <typename U>
    typename std::enable_if<std::is_trivially_destructible<U>::value>::type
        resetElement(U& element) {
        new (&element) U();
    }

    /**
     * @brief   Trims the unset elements by calling the default constructor on them.
     */
    void trim()
    {
        for (unsigned int i = m_length; i < capacity; i++) resetElement(m_elements[i]);
    }

    /// @brief Loads the fixed part of the data ('m_length' and 'm_offset') from buffer.
    /// @returns Elements data pointer.
    void* loadFixedPart(void* buffer)
    {
        m_length = ((unsigned int*)buffer)[0];
        m_offset = ((int*)buffer)[1];
        return m_elements;
    }

    /// @returns The number of bytes the currently added elements take.
    size_t getElementsSize() const
    {
        return m_length * sizeof(TItem);
    }

    /// @returns The number of bytes the currently used portion of this class instance takes.
    constexpr size_t getDataSize() const
    {
        return elementsOffset + getElementsSize();
    }

protected: // Data:

    unsigned int m_length;  // List length.
    int m_offset;           // Current element offset.
    TItem m_elements[TNum]; // Elements array.

    /// @brief The number of bytes the "header" part of the class take (bytes before elements).
    static constexpr size_t elementsOffset = sizeof(m_length) + sizeof(m_offset);

    /// @brief The number of bytes that must be reserved for this class instance.
    static constexpr size_t getMaxDataSize() { return sizeof(ThisType); }

};
