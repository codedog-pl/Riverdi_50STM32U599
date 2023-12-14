#pragma once

#include "DateTime.hpp"
#include "FS_AdapterTypes.hpp"
#include "FS_FileSystem.hpp"

#define FS_MOUNT_MTAB_FULL          0xfff0
#define FS_MOUNT_CONFLICT           0xfff1
#define FS_MOUNT_ROOT_NOT_FOUND     0xfff2
#define FS_MOUNT_MEDIA_NOT_FOUND    0xfff3
#define FS_NEGATIVE                 0xfffe
#define FS_ERROR                    0xffff

namespace FS
{

/// @brief Defines methods for a file system adapter class.
class AdapterMethods : public AdapterTypes
{

public:

    /// @brief Finds the directory entry that matches the path.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param entry Directory entry reference.
    /// @return Status.
    virtual Status find(Media& media, const char* path, DirectoryEntry& entry) = 0;

    /// @brief Gets the file or directory creation time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    virtual Status created(Media& media, const char* path, DateTime& dateTime) = 0;

    /// @brief Gets the file or directory last modification time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    virtual Status modified(Media& media, const char* path, DateTime& dateTime) = 0;

    /// @brief Creates a file.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return Status.
    virtual Status fileCreate(Media& media, const char* path) = 0;

    /// @brief Tests if a file exist on the media.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return Status.
    virtual Status fileExists(Media& media, const char* path) = 0;

    /// @brief Opens a file.
    /// @param media Media structure reference.
    /// @param file File handle reference.
    /// @param path A path to the file relative to the file system root.
    /// @param mode File opening flags.
    /// @return Status.
    virtual Status fileOpen(Media& media, FileHandle& file, const char* path, FileMode mode = FileMode::read) = 0;

    /// @brief Moves the file pointer to the specified offset.
    /// @param file File handle reference.
    /// @param offset Position within the file.
    /// @return Status.
    virtual Status fileSeek(FileHandle& file, FileOffset offset) = 0;

    /// @brief Reads data from a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesRead Number of bytes read variable reference.
    /// @return Status.
    virtual Status fileRead(FileHandle& file, void* buffer, size_t size, size_t& bytesRead) = 0;

    /// @brief Writes data to a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @return Status.
    virtual Status fileWrite(FileHandle& file, void* buffer, size_t size) = 0;

    /// @brief Closes a file.
    /// @param file File handle reference.
    /// @return Status.
    virtual Status fileClose(FileHandle& file) = 0;

    /// @brief Renames a file on the media.
    /// @param media Media structure reference.
    /// @param oldName Old file name.
    /// @param newName New file name.
    /// @return Status.
    virtual Status fileRename(Media& media, const char* oldName, const char* newName) = 0;

    /// @brief Deletes a file from the media.
    /// @param media Media structure reference.
    /// @param path File name.
    /// @return Status.
    virtual Status fileDelete(Media& media, const char* path) = 0;

    /// @brief Creates a directory on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    virtual Status directoryCreate(Media& media, const char* path) = 0;

    /// @brief Tests if a directory exists on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    virtual Status directoryExists(Media& media, const char* path) = 0;

    /// @brief Renames a directory on the media.
    /// @param media Media structure reference.
    /// @param oldName Old directory name.
    /// @param newName New directory name.
    /// @return Status.
    virtual Status directoryRename(Media& media, const char* oldName, const char* newName) = 0;

    /// @brief Deletes a directory from the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    virtual Status directoryDelete(Media& media, const char* path) = 0;

};

}
