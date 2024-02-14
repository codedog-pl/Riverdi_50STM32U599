/**
 * @file        TextBuffer.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides fixed size text buffer with some text manipulation methods. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @remarks     Use if required buffer size is known at compile time / for stack allocations.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include "IData.hpp"

/// @brief Provides basic text buffer manipulation functions.
class TextBufferBase : public virtual IData<char>
{

public:

    using IData::operator[];

    /// @brief Creates empty text buffer.
    TextBufferBase() : m_length(0) { };

    /// @brief Copies the text buffer.
    /// @param other The text buffer to be copied.
    TextBufferBase(const TextBufferBase& other) : m_length(other.m_length) { }

    /// @brief This type is not moveable.
    TextBufferBase(TextBufferBase&&) = delete;

    /// @brief Copy assignment operator.
    /// @param other The text buffer to be copied.
    /// @returns This reference.
    TextBufferBase& operator=(const TextBufferBase& other)
    {
        if (&other == this) return *this;
        clear();
        memcpy(data(), other.data(), m_length = other.m_length);
    }

    /// @returns Character at the specified index reference.
    inline char& operator[](int index)
    {
        return at(index);
    }

    /// @brief Implicitly converts the text buffer into a C type null terminated string.
    inline operator char*() const { return const_cast<char*>(c_str()); }

    /// @brief Implicitly converts the text buffer into a C type null terminated string.
    inline operator const char*() const { return c_str(); }

    /// @returns Character at the specified index reference.
    inline char& at(int index)
    {
        return (index >= 0 && index < static_cast<int>(m_length)) ? data()[index] : nullChar;
    }

    /// @returns The pointer to a null-terminated character array.
    inline const char* c_str() const
    {
        return const_cast<char*>(data());
    }

    /// @brief @returns The pointer to a null-terminated `T` array.
    template<typename T>
    inline T* as()
    {
        static_assert(std::is_trivially_copyable<T>::value, "T must be a trivially copyable type");

        if (alignof(T) > alignof(char)) {
            static_assert(alignof(T) <= alignof(char), "T has stricter alignment requirements than char");
        }

        return reinterpret_cast<T*>(this->data());
    }

    /// @brief @returns The pointer to a null-terminated `T` array.
    template<typename T>
    inline const T* as() const
    {
        static_assert(std::is_trivially_copyable<T>::value, "T must be a trivially copyable type");

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

    /// @brief Copies the current buffer content to another buffer.
    /// @param target Target buffer address.
    /// @param _size The optional fragment size. Set zero to use the current text length.
    void copy(char* target, size_t _size = 0) const
    {
        if (_size < 1) _size = m_length;
        if (_size > size()) _size = size();
        if (!_size) return;
        memcpy(target, data(), _size);
    }

    /// @brief Appends formatted string. Overflowing text will be discarded.
    /// @param format String format.
    /// @param ... Variadic arguments.
    void printf(const char* format, ...)
    {
        if (m_length >= size()) return;
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }

    /// @brief Appends formatted string. Overflowing text will be discarded.
    /// @param format String format.
    /// @param args Variadic arguments.
    void vprintf(const char* format, va_list args)
    {
        if (m_length >= size()) return;
        int ret = vsnprintf(&(data()[m_length]), size() - m_length, format, args);
        if (ret > 0) m_length += std::min(static_cast<size_t>(ret), size() - m_length - 1);
        data()[m_length] = '\0'; // Ensure null-termination
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

    /// @brief Updates the internal length of the string in the buffer.
    inline void update(void)
    {
        m_length = strlen(c_str());
    }

protected:

    /// @returns 1: The buffer is taken, as either non empty, or marked at offset 1. 0: Empty and free to take.
    bool isTaken() const { return data()[0] || data()[1]; }

    /// @brief Marks the buffer as taken if it appears empty having the first character zero.
    void markTaken() { if (!data()[0]) data()[1] = 1; }

protected:
    size_t m_length = 0;
    inline static char nullChar = '\0'; // A null character pointer to return for index out of bounds values.

};

/// @brief Statically allocated text buffer.
/// @tparam TSize Fixed size in bytes.
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
    /// @param format Format string.
    /// @param ... Variadic arguments.
    TextBuffer(const char* format, ...) : Data(), Base()
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }

    /// @brief Copies a buffer from other instance.
    /// @param other The instance to copy the data from.
    TextBuffer(const TextBuffer& other) : Data(other) { m_length = other.m_length; }

    /// @brief This data type is not moveable, since it's stack allocated.
    TextBuffer(TextBuffer&&) = delete;

};
