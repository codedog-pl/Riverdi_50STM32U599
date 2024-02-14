/**
 * @file        File.hpp
 * @author      Adam Łyskawa
 *
 * @brief       RAII file access API. Header file.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "Path.hpp"

namespace FS
{

/// @brief Provides RAII file access API.
struct File final : public Path
{

    File(const File&) = delete; // This type should not be copied.
    File(File&&) = delete; // This type should not be moved.

    /// @brief Opens a file.
    /// @param absolutePath Absolute path to the file.
    /// @param pMode One or more flags from the `FileMode` enumeration.
    /// @param ... Variadic arguments used to format the path string.
    File(const char* absolutePath, FileMode pMode, ...);

    /// @brief Opens a file.
    /// @param path Path reference.
    /// @param pMode One or more flags from the `FileMode` enumeration.
    /// @param ... Variadic arguments used to format the path string.
    File(Path& path, FileMode pMode, ...);

    /// @brief Opens a file.
    /// @param fs File system pointer.
    /// @param relativePath Relative path to the file.
    /// @param pMode One or more flags from the `FileMode` enumeration.
    /// @param ... Variadic arguments used to format the path string.
    File(const FileSystem* fs, const char* relativePath, FileMode pMode, ...);

    /// @brief The file is closed when this instance is discarded.
    ~File();

    /// @returns True if the file is actually successfully open.
    inline bool isOpen() const { return m_isOpen; }

    /// @returns True if the file is actially successfully open.
    inline operator bool() const { return m_isOpen; }

    /// @brief Moves the read / write pointer to the specified offset in the file.
    /// @param offset File offset.
    /// @returns True if done. False otherwise.
    bool seek(FileOffset offset);

    /// @brief Reads the data from the file.
    /// @param buffer Buffer pointer.
    /// @param size Number of bytes requested.
    /// @returns Number of bytes read or an empty value if error occurred.
    ReadResult read(void* buffer, size_t size);

    /// @brief Reads a structure or a primitive type from the file.
    /// @tparam T The type of the structure, can also be a primitive type.
    /// @param data The data reference.
    /// @returns True if read successfully, false if not read at all or less than required length read.
    template<typename T> bool read(T& data)
    {
        constexpr size_t size = sizeof(data);
        ReadResult result = read(&data, size);
        return result.has_value() && result.value() == size;
    }

    /// @brief Writes the data to the file.
    /// @param buffer Buffer pointer.
    /// @param size Number of bytes to write.
    /// @returns True if written successfully. False otherwise.
    bool write(const void* buffer, size_t size);

    /// @brief Writes a structure or a primitive type to the file.
    /// @tparam T  The type of the structure, can also be a primitive type.
    /// @param data The data reference.
    /// @returns True if written successfully. False otherwise.
    template<typename T> bool write(T& data) { return write(&data, sizeof(data)); }

    /// @brief Closes the file if it was opened.
    void close();

private:

    /// @brief Opens or creates the file on the media if the path is valid and the media is mounted in the `FileSystemTable`.
    void open();

    FileControlBlock m_file;  // File handle.
    FileMode m_mode;    // File mode.
    Status m_status;    // File status.
    bool m_isOpen;      // File is open.

};

}
