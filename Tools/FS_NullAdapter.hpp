#pragma once

#include "FS_AdapterMethods.hpp"
#include <cstring>

#if !defined(USE_FILEX) && !defined(USE_FATFS)

namespace FS
{

/// @brief A dummy adapter that can write anything just discarding the data it gets.
/// @remarks Obviously, all functions that involve reading fill always fail.
/// @remarks Can be used as a template for implementing non trivial adapters for different file system middlewares.
class NullAdapter final : public AdapterMethods
{

public:

    /// @brief Finds the directory entry that matches the path.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param entry Directory entry reference.
    /// @return Status.
    Status find(Media& media, const char* path, DirectoryEntry& entry) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Gets the file or directory creation time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    Status created(Media& media, const char* path, DateTime& dateTime) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Gets the file or directory last modification time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    Status modified(Media& media, const char* path, DateTime& dateTime) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Creates a file.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return Status.
    Status fileCreate(Media& media, const char* path) override
    {
        return OK;
    }

    /// @brief Tests if a file exist on the media.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return True if the file exists, false otherwise.
    Status fileExists(Media& media, const char* path) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Opens a file.
    /// @param media Media structure reference.
    /// @param file File handle reference.
    /// @param path A path to the file relative to the file system root.
    /// @param mode File opening mode. Default opens existing file for reading.
    /// @return Status.
    Status fileOpen(Media& media, FileHandle& file, const char* path, FileMode mode = FileMode::read) override
    {
        if (!(mode & FileMode::write) || file.isUsed) return FS_NEGATIVE;
        file.isUsed = true;
    }

    /// @brief Moves the file pointer to the specified offset.
    /// @param file File handle reference.
    /// @param offset Position within the file.
    /// @return Status.
    Status fileSeek(FileHandle& file, FileOffset offset) override
    {
        return file.isUsed ? OK : FS_NEGATIVE;
    }

    /// @brief Reads data from a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesRead Number of bytes read variable reference.
    /// @return Status.
    Status fileRead(FileHandle& file, void* buffer, size_t size, size_t& bytesRead) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Writes data to a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesWritten Number of bytes written variable reference.
    /// @return Status.
    virtual Status fileWrite(FileHandle& file, void* buffer, size_t size) override
    {
        if (!file.isUsed || !buffer || !size) return FS_NEGATIVE;
        return OK;
    }

    /// @brief Closes a file.
    /// @param file File handle reference.
    /// @return Status.
    Status fileClose(FileHandle& file) override
    {
        if (!file.isUsed) return FS_NEGATIVE;
        file.isUsed = false;
        return OK;
    }

    /// @brief Renames a file.
    /// @param media Media structure reference.
    /// @param oldName Old file name.
    /// @param newName New file name.
    /// @return Status.
    Status fileRename(Media& media, const char* oldName, const char* newName) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Deletes a file.
    /// @param media Media structure reference.
    /// @param path File name.
    /// @return Status.
    Status fileDelete(Media& media, const char* path) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Creates a directory on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    Status directoryCreate(Media& media, const char* path) override
    {
        return OK;
    }

    /// @brief Tests if a directory exists on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    Status directoryExists(Media& media, const char* path) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Renames a directory on the media.
    /// @param media Media structure reference.
    /// @param oldName Old directory name.
    /// @param newName New directory name.
    /// @return Status.
    Status directoryRename(Media& media, const char* oldName, const char* newName) override
    {
        return FS_NEGATIVE;
    }

    /// @brief Deletes a directory from the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    Status directoryDelete(Media& media, const char* path) override
    {
        return FS_NEGATIVE;
    }

};

}

#endif