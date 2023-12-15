#pragma once

#include "FS_FileSystem.hpp"
#include <cstring>
#include <cstdio>

namespace FS
{

/// @brief A file system path in the file system context.
struct Path : protected AdapterTypes
{

    static constexpr size_t maxLength = lfnMaxLength;   // Maximum allowed path length.

    /// @brief Creates an empty path target.
    Path() : m_fileSystem(), m_absolutePath(), m_relativePath() { }

    /// @brief Creates a target file system information from an absolute path string and optional arguments.
    /// @tparam ...va Variadic arguments type.
    /// @param path Absolute path to the file system entry.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> Path(const char* path, va ...args) : m_fileSystem(), m_absolutePath(), m_relativePath()
    {
        m_fileSystem = FileSystemTable::find(path);
        if (!m_fileSystem || !m_fileSystem->root) return;
        std::snprintf(&m_absolutePath[0], maxLength, path, args...);
        std::snprintf(&m_relativePath[0], maxLength, path + std::strlen(m_fileSystem->root), args...);
    }

    /// @brief Creates a target file system information from a file system target pointer, relative path string and optional arguments.
    /// @tparam ...va Variadic arguments type.
    /// @param fs File system pointer.
    /// @param path Relative path to the file system entry.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> Path(FileSystem* fs, const char* path, va ...args) : m_fileSystem(), m_absolutePath(), m_relativePath()
    {
        if (!fs || !fs->root) return;
        m_fileSystem = fs;
        auto rootLength = std::strlen(m_fileSystem->root);
        std::memcpy(&m_absolutePath, &m_fileSystem->root, rootLength);
        std::snprintf(&m_relativePath[0], maxLength, path, args...);
        std::snprintf(&m_absolutePath[0] + rootLength, maxLength - rootLength, path, args...);
    }

    /// @returns The file system target pointer.
    inline const FileSystem* fileSystem() const { return m_fileSystem; }

    /// @returns The absolute path (containing the file system root path).
    inline const char* absolutePath() const { return m_absolutePath; }

    /// @returns The relative path (relative to the file system root path).
    inline const char* relativePath() const { return m_relativePath; }

    /// @returns True if the path target is fully configured.
    bool isValid() const
    {
        return !!m_fileSystem && !!m_fileSystem->root && !!m_fileSystem->media && !!m_absolutePath[0] && !!m_relativePath[0];
    }

protected:

    FileSystem* m_fileSystem;             // File system target pointer
    char m_absolutePath[lfnMaxLength];    // Absolute path string.
    char m_relativePath[lfnMaxLength];    // Relative path string.

};

}
