/**
 * @file        Path.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Defines a file system path in a file system context. Header file.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "FileSystem.hpp"
#include <cstdarg>

namespace FS
{

/// @brief A file system path in a file system context.
struct Path : protected AdapterTypes
{

    static constexpr size_t maxLength = lfnMaxLength;   // Maximum allowed path length.

    /// @brief Creates an empty path target.
    Path();

    /// @brief Creates a target file system information from an absolute path string and variadic arguments.
    /// @param args Initialized list of variadic arguments.
    /// @param path Absolute path to the file system entry.
    Path(va_list args, const char* path);

    /// @brief Creates a target file system information from a file system target pointer, relative path string and variadic arguments.
    /// @param args Initialized list of variadic arguments.
    /// @param fs File system pointer.
    /// @param path Relative path to the file system entry.
    Path(va_list args, const FileSystem* fs, const char* path);

    /// @brief Creates a target file system information from an absolute path string and optional arguments.
    /// @param path Absolute path to the file system entry.
    /// @param ... Optional arguments used to format the path.
    Path(const char* path, ...);

    /// @brief Creates a target file system information from a file system target pointer, relative path string and optional arguments.
    /// @param fs File system pointer.
    /// @param path Relative path to the file system entry.
    /// @param ...Optional arguments used to format the path.
    Path(const FileSystem* fs, const char* path, ...);

    /// @returns The file system target pointer.
    inline const FileSystem* fileSystem() const { return m_fileSystem; }

    /// @returns The absolute path (containing the file system root path).
    inline const char* absolutePath() const { return m_absolutePath; }

    /// @returns The relative path (relative to the file system root path).
    inline const char* relativePath() const { return m_relativePath; }

    /// @returns True if the path target is fully configured.
    inline bool isValid() const
    {
        return !!m_fileSystem && !!m_fileSystem->root() && !!m_fileSystem->media() && !!m_absolutePath[0] && !!m_relativePath[0];
    }

protected:

    /// @brief Initializes the structure with the absolute path and variadic arguments to format the path string.
    /// @param path Absolute path to the file system entry.
    /// @param args Variadic arguments list used to format the path string.
    void initializeWithVariadicArgs(const char* path, va_list args);

    /// @brief Initializes the structure with the file system pointer, relative path and variadic arguments to format the path string.
    /// @param fs File system pointer.
    /// @param path Relative path to the file system entry.
    /// @param args Variadic arguments list used to format the path string.
    void initializeWithVariadicArgs(const FileSystem* fs, const char* path, va_list args);

protected:

    const FileSystem* m_fileSystem;       // File system target pointer
    char m_absolutePath[lfnMaxLength];    // Absolute path string.
    char m_relativePath[lfnMaxLength];    // Relative path string.

};

}
