#pragma once

#include "FS_FileSystem.hpp"
#include <cstring>
#include <cstdio>


namespace FS
{

/// @brief A file system path in the file system context.
struct Path : protected AdapterTypes
{
    FileSystem* fileSystem;             // File system target pointer
    char absolutePath[lfnMaxLength];    // Absolute path string.
    char relativePath[lfnMaxLength];    // Relative path string.

    /// @brief Creates an empty path target.
    Path() : fileSystem(), absolutePath(), relativePath() { }

    /// @brief Creates a target file system information from an absolute path string and optional arguments.
    /// @tparam ...va Variadic arguments type.
    /// @param path Absolute path to the file system entry.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> Path(const char* path, va ...args) : fileSystem(), absolutePath(), relativePath()
    {
        fileSystem = FileSystemTable::find(path);
        if (!fileSystem) return; // If the root is not found, the `fileSystem` will be empty.
        std::snprintf(&absolutePath, FS_MAX_PATH_LENGTH, path, args...);
        std::snprintf(&relativePath, FS_MAX_PATH_LENGTH, path + std::strlen(fileSystem->root), args...);
    }

    /// @brief Creates a target file system information from a file system target pointer, relative path string and optional arguments.
    /// @tparam ...va Variadic arguments type.
    /// @param fs File system target reference.
    /// @param path Relative path to the file system entry.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> Path(FileSystem& fs, const char* path, va ...args) : fileSystem(), absolutePath(), relativePath()
    {
        if (!fs.root) return; // If the target's root is empty, the `fileSystem` will be empty.
        fileSystem = &fs;
        auto rootLength = std::strlen(fileSystem->root);
        std::memcpy(&absolutePath, &fileSystem->root, rootLength);
        std::snprintf(&relativePath, FS_MAX_PATH_LENGTH, path, args...);
        std::snprintf(&absolutePath + rootLength, FS_MAX_PATH_LENGTH - rootLength, path, args...);
    }

    /// @returns True if the path target is fully configured.
    bool isValid()
    {
        return !!fileSystem && !!fileSystem->root && !!fileSystem->media && !!absolutePath[0] && !!relativePath[0];
    }

};



}
