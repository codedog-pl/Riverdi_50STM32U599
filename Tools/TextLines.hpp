/**
 * @file        TextLines.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides a fixed number of fixed length text lines that can be stored in a text file. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "TextBuffer.hpp"
#include "IndexIterator.hpp"
#include <new>

/// @brief Provides a fixed number of fixed length text lines that can be stored in a text file.
/// @tparam TNum Number of lines.
/// @tparam TLength Maximum length of a line.
template<size_t TNum, size_t TLength>
class TextLines
{

public:

    using Line = TextBuffer<TLength>; // Single line buffer type.
    using ThisType = TextLines<TNum, TLength>;
    using ValueType = Line;
    using Iterator = IndexIterator<ThisType>;

    static constexpr size_t lineCapacity = TNum;    // The number of lines this type can store.
    static constexpr size_t lineSize = TLength;     // The number of characters in an individual line.
    static constexpr size_t size = TNum * TLength;  // The number of bytes this elements require to store its text.

    TextLines() : m_data(), m_count(0) { }

    /// @brief Returns the line address at specified index (or `nullptr` when `index` is out of bounds).
    /// @param index Line index.
    /// @returns Line address or `nullptr`.
    inline Line& operator[](size_t index) { return index < TNum ? m_data[index] : m_sentinel; }

    /// @brief Returns the line address at specified index (or `nullptr` when `index` is out of bounds).
    /// @param index Line index.
    /// @returns Line address or `nullptr`.
    inline const Line& operator[](size_t index) const { return index < TNum ? m_data[index] : m_sentinel; }

    /// @returns The starting iterator for the forward only range of elements.
    inline Iterator begin() { return Iterator(this, 0); }

    /// @returns Excluded end interator for the forward only range of elements.
    inline Iterator end() { return Iterator(this, m_count); }

    /// @returns The number of lines set.
    inline size_t count() const { return m_count; }

    /// @brief Clears the data.
    virtual void clear()
    {
        for (size_t i = 0; i < TNum; i++) new(&m_data[i]) Line();
        m_count = 0;
    }

protected:

    /// @brief Sets the lines from a text buffer using line end characters as delimiters.
    /// @param text The text buffer containg some lines of text.
    void fromText(TextBufferBase& text)
    {
        size_t lineIndex = 0, lineOffset = 0;
        m_count = 0;
        for (size_t i = 0, n = text.length(); i < n; ++i)
        {
            if (!text[i] || !*text[i]) break;
            if (*text[i] == '\r') continue;
            if (*text[i] == '\n')
            {
                if (++lineIndex < TNum)
                {
                    lineOffset = 0;
                    continue;
                }
                else break;
            }
            if (!lineOffset) ++m_count; // First character that is not a line end character increases the total line count.
            if (lineOffset < TLength)
            {
                char c = *text[i];
                if (c >= ' ' && c <= '~') // printable ASCII only
                    *m_data[lineIndex][lineOffset++] = c;
            }
        }
    }

    /// @brief Places the lines into a text buffer using line end characters as delimiters.
    /// @param text The text buffer the lines will be placed into.
    void toText(TextBufferBase& text) const
    {
        if (!m_count) return;
        for (size_t i = 0; i < m_count; ++i) text.printf(i < m_count - 1 ? "%s\r\n" : "%s", m_data[i]);
    }

private:

    Line m_data[TNum];  // Lines data array.
    inline static Line m_sentinel = {};
    size_t m_count;     // The number of lines set.

};
