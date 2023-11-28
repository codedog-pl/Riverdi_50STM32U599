/**
 * @file        TextBuffer.hpp
 * @author      CodeDog
 * @brief       Provides statically allocated fixed size text buffer.
 * @remarks     Use if required buffer size is known at compile time / for stack allocations.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include "IData.hpp"

/**
 * @brief Provides basic text buffer manipulation functions.
 */
class TextBufferBase : public virtual IData<char>
{
public:
    using IData::operator[];

    /// @brief Creates empty text buffer.
    TextBufferBase() : m_length(0) { };

    /// @brief Creates a text buffer with formatted text.
    /// @tparam ...va Variadic arguments type.
    /// @param format Format string.
    /// @param ...args Variadic arguments.
    template<class ...va> TextBufferBase(const char* format, va ...args) : m_length(0) { printf(format, args...); }

    TextBufferBase(const TextBufferBase& other) : m_length(other.m_length) { }

    TextBufferBase(TextBufferBase&&) = delete;

    TextBufferBase& operator=(const TextBufferBase& other)
    {
        if (&other == this) return *this;
        clear();
        m_length = other.m_length;
        memcpy(data(), other.data(), other.m_length);
    }

    // /// @returns Character at the specified index reference.
    // inline char& at(int index)
    // {
    //     return (index >= 0 && index < static_cast<int>(m_length)) ? data()[index] : nullChar;
    // }

    /// @returns The pointer to a null-terminated character array.
    inline const char* c_str() const
    {
        return const_cast<char*>(data());
    }

    /// @brief @returns The pointer to a null-terminated `T` array.
    template<typename T>
    inline const T* as() const
    {
        static_assert(std::is_trivially_copyable<T>::value, "T must be a trivially copyable type");

        // Additionally, you might check the alignment:
        if (alignof(T) > alignof(char)) {
            static_assert(alignof(T) <= alignof(char), "T has stricter alignment requirements than char");
        }

        return reinterpret_cast<const T*>(this->data());
    }

    /// @brief Clears the buffer by zeroing all of its memory.
    inline void clear()
    {
        m_length = 0;
        memset(data(), 0, size());
    }

    /// @returns The C-string length without the null terminator.
    inline size_t length() const
    {
        return m_length;
    }

    // /// @returns Character at the specified index reference.
    // inline char& operator[](int index)
    // {
    //     return at(index);
    // }

    /**
     * @brief Appends formatted string. Overflowing text will be discarded.
     *
     * @tparam va Variadic argumets type.
     * @param format String format.
     * @param args Variadic arguments.
     */
    template<class ...va> void printf(const char* format, va ...args)
    {
        if (m_length < size() && data()) {
            int ret = snprintf(&(data()[m_length]), size() - m_length, format, args...);
            if (ret > 0) m_length += std::min(static_cast<size_t>(ret), size() - m_length - 1);
            data()[m_length] = '\0'; // Ensure null-termination
        }
    }

    /// @brief Appends a byte character.
    inline void poke(char value)
    {
        if (m_length < size()) data()[m_length++] = value;
    }

    /// @brief Appends 2 byte characters.
    inline void poke(char v1, char v2)
    {
        if (m_length < size()) data()[m_length++] = v1;
        if (m_length < size()) data()[m_length++] = v2;
    }

    /// @brief Appends 3 byte characters.
    inline void poke(char v1, char v2, char v3)
    {
        if (m_length < size()) data()[m_length++] = v1;
        if (m_length < size()) data()[m_length++] = v2;
        if (m_length < size()) data()[m_length++] = v3;
    }

    /// @brief Appends 4 byte characters.
    inline void poke(char v1, char v2, char v3, char v4)
    {
        if (m_length < size()) data()[m_length++] = v1;
        if (m_length < size()) data()[m_length++] = v2;
        if (m_length < size()) data()[m_length++] = v3;
        if (m_length < size()) data()[m_length++] = v4;
    }

    /// @brief Appends 5 byte characters.
    inline void poke(char v1, char v2, char v3, char v4, char v5)
    {
        if (m_length < size()) data()[m_length++] = v1;
        if (m_length < size()) data()[m_length++] = v2;
        if (m_length < size()) data()[m_length++] = v3;
        if (m_length < size()) data()[m_length++] = v4;
        if (m_length < size()) data()[m_length++] = v5;
    }

public:
    size_t m_length = 0;

friend class StringPool;
protected:

    /// @returns 1: The buffer is taken, as either non empty, or marked at offset 1. 0: Empty and free to take.
    bool isTaken() { return data()[0] || data()[1]; }

    /// @brief Marks the buffer as taken if it appears empty having the first character zero.
    void markTaken() { if (!data()[0]) data()[1] = 1; }

protected:
    inline static char nullChar = '\0'; // A null character pointer to return for index out of bounds values.
};

/**
 * @brief Statically allocated buffer.
 * @tparam TSize Fixed size in bytes.
 *
 * @remarks Copyable, not moveable.
 */
template<size_t TSize>
class TextBuffer : public StaticData<TSize, char>, public TextBufferBase
{
protected:
    using Data = StaticData<TSize, char>;
    using Base = TextBufferBase;

public:
    using Data::operator[];

    /// @brief Creates an empty buffer.
    TextBuffer() : Data(), Base() { }

   /// @brief Creates a text buffer with formatted text.
   /// @tparam ...va Variadic arguments type.
   /// @param format Format string.
   /// @param ...args Variadic arguments.
   template<class ...va> TextBuffer(const char* format, va...args) : Data(), Base() { printf(format, args...); }

    /// @brief Copies a buffer from other instance.
    /// @param other The instance to copy the data from.
    TextBuffer(const TextBuffer& other) : Data(other) { m_length = other.m_length; }

    /// @brief This data type is not moveable, since it's stack allocated.
    TextBuffer(TextBuffer&&) = delete;

};
