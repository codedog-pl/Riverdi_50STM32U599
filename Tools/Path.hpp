#pragma once

#include <cstring>
#include <cstdio>
#include <tuple>

/// @brief File system path container / manipulation class.
class Path final
{

public:
    static constexpr int maxLength = 64;    // The maximal length of the path.
    static constexpr char separator = '/';  // The separator character.

    /// @brief Default constructor.
    Path()
    {
        memset(m_data, 0, maxLength);
    }

    /// @brief Initializing constructor.
    /// @param c_str Initial value.
    Path(const char* c_str)
    {
        memset(m_data, 0, maxLength);
        snprintf(m_data, maxLength, c_str);
    }

    /// @brief Copy constructor.
    /// @param other Other path.
    Path(const Path& other)
    {
        copy(other);
    }

    /// @brief Move constructor.
    /// @param other Other path.
    Path(Path&& other) noexcept
    {
        size_t length = strlen(other.m_data);
        memcpy(m_data, other.m_data, length);
        if (maxLength - length > 0) memset(&m_data[length], 0, maxLength - length);
        memset(other.m_data, 0, maxLength);
    }

    /// @brief Clears the data.
    ~Path()
    {
        memset(m_data, 0, maxLength);
    }

    /// @brief Copies the content of the other path into this path.
    /// @param other A path instance to copy the content from.
    /// @return Self pointer.
    Path& copy(const Path& other)
    {
        size_t length = strlen(other.m_data);
        memcpy(m_data, other.m_data, length);
        if (maxLength - length > 0) memset(&m_data[length], 0, maxLength - length);
        return *this;
    }

    /// @brief Adds a part to the path.
    /// @tparam ...va Variadic arguments.
    /// @param part Path part that will be added after a path separator.
    /// @param ...args Optional arguments for format strings in the `part` value.
    /// @returns Self pointer.
    template<class ...va>
    Path& add(const char* part, va ...args)
    {
        size_t offset = length();
        if (offset >= maxLength - 1) return *this;
        // Add separator if not empty:
        if (offset)
        {
            m_data[offset] = separator;
            ++offset;
        }
        // Add part:
        size_t space = maxLength - offset;
        if (space <= 0) return *this;
        snprintf(&m_data[offset], space, part, args...);
        deleteTrailingSeparator();
        return *this;
    }

    /// @brief Prefixes the last part of the path.
    /// @param prefix A prefix string, default just a dot.
    /// @return Self pointer.
    Path& prefix(const char* prefix = ".")
    {
        auto [offset, space] = locateLastPart();
        size_t prefixLength = strlen(prefix);
        if (space < prefixLength) return *this;
        memmove(&m_data[offset + prefixLength], &m_data[offset], length() - offset);
        memcpy(&m_data[offset], prefix, prefixLength);
        return *this;
    }

    /// @brief Replaces the last part of the path.
    /// @tparam ...va Variadic arguments.
    /// @param part The replacement path part.
    /// @param ...args Optional arguments for format strings in the `part` value.
    /// @return Self pointer.
    template<class ...va>
    Path& replaceLast(const char* part, va ...args)
    {
        auto [offset, space] = locateLastPart();
        if (space <= 0) return *this;
        snprintf(&m_data[offset], space, part, args...);
        deleteTrailingSeparator();
        return *this;
    }

    /// @returns Current value as a C (zero terminated) string.
    const char* c_str() const
    {
        return const_cast<const char*>(&m_data[0]);
    }

    /// @returns 1: Path is empty. 0: Path is not empty.
    bool isEmpty() const
    {
        return !m_data[0];
    }

    /// @return The length of the path string to the zero termination.
    size_t length() const
    {
        return strlen(m_data);
    }

    /// @brief Reset the path data to all zeros.
    void reset()
    {
        memset(m_data, 0, maxLength);
    }

friend class PathPool;
private:

    /// @brief Marks the empty path as taken.
    void markTaken()
    {
        if (!m_data[0]) m_data[1] = 1;
    }

    /// @brief Checks if the path is marked as taken.
    /// @return 1: Taken. 0: Available.
    bool isTaken()
    {
        return m_data[0] || m_data[1];
    }

    /// @returns Last part's offset and space left.
    std::tuple<int, size_t> locateLastPart()
    {
        int last = length() - 1;
        int offset = last;
        for (; offset >= 0; --offset) if (m_data[offset] == separator) break;
        if (offset) ++offset;
        size_t space = maxLength - offset;
        return std::tuple<int, size_t>(offset, space);
    }

    /// @brief Deletes the trailing path separator.
    void deleteTrailingSeparator()
    {
        int offset = length();
        if (offset && m_data[offset - 1] == separator) m_data[offset - 1] = 0;
    }

private:
    char m_data[maxLength]; // Path characters data. (The only data this class contains.)

};
