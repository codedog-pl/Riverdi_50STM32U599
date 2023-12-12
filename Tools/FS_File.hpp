#pragma once

#include "FS_Path.hpp"
#include "FS_Adapter.hpp"
#include <utility>
#include <cstdarg>

namespace FS
{

USE_ADAPTER

/// @brief Provides RAII file access API.
struct File final : public Path
{

    File(const File&) = delete; // This type should not be copied.
    File(File&&) = delete; // This type should not be moved.
    ~File() { close(); } // The file is closed when this instance is discarded.

    /// @brief Opens a file.
    /// @tparam ...va Variadic arguments type.
    /// @param path Absolute path to the file.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> File(const char* absolutePath, FileMode pMode, va ...args)
        : Path(absolutePath, std::forward<va>(args)...), mode(pMode) { open(); }

    /// @brief Creates a file metadata from a file system target pointer, relative path string and optional arguments.
    /// @tparam ...va Variadic arguments type.
    /// @param fs File system pointer.
    /// @param path Relative path to the file.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> File(FileSystem* fs, const char* relativePath, FileMode pMode, va ...args)
        : Path(fs, relativePath, std::forward<va>(args)...), mode(pMode) { open(); }

    FileHandle file;    // File handle.
    FileMode mode;      // File mode.
    bool isOpen;        // File is open.

    /// @brief Moves the read / write pointer to the specified offset in the file.
    /// @param offset File offset.
    /// @return True if done. False otherwise.
    bool seek(FileOffset offset)
    {
        if (!isOpen) return false;
        return adapter.fileSeek(file, offset) == OK;
    }

    /// @brief Reads the data from the file.
    /// @param buffer Buffer pointer.
    /// @param size Number of bytes requested.
    /// @return Number of bytes read or an empty value if error occurred.
    ReadResult read(void* buffer, size_t size)
    {
        if (!isOpen || !buffer || !size) return ReadResult();
        size_t bytesRead;
        return adapter.fileRead(file, buffer, size, bytesRead) == OK ? ReadResult(bytesRead) : ReadResult();
    }

    /// @brief Writes the data to the file.
    /// @param buffer Buffer pointer.
    /// @param size Number of bytes to write.
    /// @return True if written successfully. False otherwise.
    bool write(void* buffer, size_t size)
    {
        if (!isOpen || !buffer || !size) return false;
        return adapter.fileWrite(file, buffer, size) == OK;
    }

    /// @brief Closes the file if it was opened.
    void close()
    {
        if (!isOpen) return;
        isOpen = adapter.fileClose(file) != OK;
        if (!isOpen) file = {}; // And clears the file handle just in case.
    }

private:

    /// @brief Opens or creates the file on the media if the path is valid and the media is mounted in the `FileSystemTable`.
    void open()
    {
        if (!isValid()) return; // Invalid path or media, obviously file not found.
        isOpen = adapter.fileOpen(*fileSystem->media, file, relativePath, mode) == OK;
    }

};

}
