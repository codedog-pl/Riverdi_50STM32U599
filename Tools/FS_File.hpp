/**
 * @file        FS_File.hpp
 * @author      CodeDog
 *
 * @brief       RAII file access API. Header only, but requires some implementation files compiled.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

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
    /// @param absolutePath Absolute path to the file.
    /// @param pMode One or more flags from the `FileMode` enumeration.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> File(const char* absolutePath, FileMode pMode, va ...args)
        : Path(absolutePath, std::forward<va>(args)...), m_mode(pMode) { open(); }

    /// @brief Opens a file.
    /// @tparam ...va Variadic arguments type.
    /// @param fs Target file system pointer.
    /// @param relativePath Relative path to the file. (Relative to the root path.)
    /// @param pMode One or more flags from the `FileMode` enumeration.
    /// @param ...args Optional arguments used to format the path.
    template<class ...va> File(FileSystem* fs, const char* relativePath, FileMode pMode, va ...args)
        : Path(fs, relativePath, std::forward<va>(args)...), m_mode(pMode) { open(); }

    /// @returns True if the file is actually successfully open.
    inline bool isOpen() const { return m_isOpen; }

    /// @brief Moves the read / write pointer to the specified offset in the file.
    /// @param offset File offset.
    /// @return True if done. False otherwise.
    bool seek(FileOffset offset)
    {
        if (!m_isOpen) return false;
        return adapter.fileSeek(m_file, offset) == OK;
    }

    /// @brief Reads the data from the file.
    /// @param buffer Buffer pointer.
    /// @param size Number of bytes requested.
    /// @return Number of bytes read or an empty value if error occurred.
    ReadResult read(void* buffer, size_t size)
    {
        if (!m_isOpen || !buffer || !size) return ReadResult();
        size_t bytesRead;
        return adapter.fileRead(m_file, buffer, size, bytesRead) == OK ? ReadResult(bytesRead) : ReadResult();
    }

    /// @brief Reads a structure or a primitive type from the file.
    /// @tparam T The type of the structure, can also be a primitive type.
    /// @param data The data reference.
    /// @return True if read successfully, false if not read at all or less than required length read.
    template<typename T> bool read(T& data)
    {
        constexpr size_t size = sizeof(data);
        ReadResult result = read(&data, size);
        return result.has_value() && result.value() == size;
    }

    /// @brief Writes the data to the file.
    /// @param buffer Buffer pointer.
    /// @param size Number of bytes to write.
    /// @return True if written successfully. False otherwise.
    bool write(void* buffer, size_t size)
    {
        if (!m_isOpen || !buffer || !size) return false;
        return adapter.fileWrite(m_file, buffer, size) == OK;
    }

    /// @brief Writes a structure or a primitive type to the file.
    /// @tparam T  The type of the structure, can also be a primitive type.
    /// @param data The data reference.
    /// @return True if written successfully. False otherwise.
    template<typename T> bool write(T& data) { return write(&data, sizeof(data)); }

    /// @brief Closes the file if it was opened.
    void close()
    {
        if (!m_isOpen) return;
        m_isOpen = adapter.fileClose(m_file) != OK;
        if (!m_isOpen) m_file = {}; // And clears the file handle just in case.
    }

private:

    /// @brief Opens or creates the file on the media if the path is valid and the media is mounted in the `FileSystemTable`.
    void open()
    {
        if (!isValid()) return; // Invalid path or media, obviously file not found.
        m_isOpen = adapter.fileOpen(*m_fileSystem->media(), m_file, m_relativePath, m_mode) == OK;
    }

    FileHandle m_file;    // File handle.
    FileMode m_mode;      // File mode.
    bool m_isOpen;        // File is open.

};

}
