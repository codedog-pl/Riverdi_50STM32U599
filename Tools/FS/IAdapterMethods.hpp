/**
 * @file        IAdapterMethods.hpp
 * @author      Adam Łyskawa
 *
 * @brief       File system adapter methods API.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "DateTimeEx.hpp"
#include "AdapterTypes.hpp"
#include "FileSystem.hpp"

#define FS_MOUNT_MTAB_FULL          ((FS::AdapterTypes::Status)0xfff0)  // The file system table is full.
#define FS_MOUNT_CONFLICT           ((FS::AdapterTypes::Status)0xfff1)  // A file system already mounted for a different media.
#define FS_MOUNT_ROOT_NOT_FOUND     ((FS::AdapterTypes::Status)0xfff2)  // Cannot find the file system root in the file system table.
#define FS_MOUNT_MEDIA_NOT_FOUND    ((FS::AdapterTypes::Status)0xfff3)  // Cannot find the media in the file system table.
#define FS_NEGATIVE                 ((FS::AdapterTypes::Status)0xfffe)  // The file system backend refuses to perform the action.
#define FS_ERROR                    ((FS::AdapterTypes::Status)0xffff)  // Unspecifed error occurred.

namespace FS
{

/// @brief Defines methods for a file system adapter class.
class IAdapterMethods : public AdapterTypes
{

public:

    /// @brief Finds the directory entry that matches the path.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param entry Directory entry reference.
    /// @returns Status.
    virtual Status find(Media& media, const char* path, DirectoryEntry& entry) const = 0;

    /// @brief Gets the file or directory creation time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @returns Status.
    virtual Status created(Media& media, const char* path, DateTime& dateTime) const = 0;

    /// @brief Gets the file or directory last modification time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @returns Status.
    virtual Status modified(Media& media, const char* path, DateTime& dateTime) const = 0;

    /// @brief Creates a file.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @returns Status.
    virtual Status fileCreate(Media& media, const char* path) const = 0;

    /// @brief Tests if a file exist on the media.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @returns Status.
    virtual Status fileExists(Media& media, const char* path) const = 0;

    /// @brief Opens a file.
    /// @param media Media structure reference.
    /// @param file File handle reference.
    /// @param path A path to the file relative to the file system root.
    /// @param mode File opening flags.
    /// @returns Status.
    virtual Status fileOpen(Media& media, FileControlBlock& file, const char* path, FileMode mode = FileMode::read) const = 0;

    /// @brief Moves the file pointer to the specified offset.
    /// @param file File handle reference.
    /// @param offset Position within the file.
    /// @returns Status.
    virtual Status fileSeek(FileControlBlock& file, FileOffset offset) const = 0;

    /// @brief Reads data from a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesRead Number of bytes read variable reference.
    /// @returns Status.
    virtual Status fileRead(FileControlBlock& file, void* buffer, size_t size, size_t& bytesRead) const = 0;

    /// @brief Writes data to a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @returns Status.
    virtual Status fileWrite(FileControlBlock& file, const void* buffer, size_t size) const = 0;

    /// @brief Closes a file.
    /// @param file File handle reference.
    /// @returns Status.
    virtual Status fileClose(FileControlBlock& file) const = 0;

    /// @brief Renames a file on the media.
    /// @param media Media structure reference.
    /// @param oldName Old file name.
    /// @param newName New file name.
    /// @returns Status.
    virtual Status fileRename(Media& media, const char* oldName, const char* newName) const = 0;

    /// @brief Deletes a file from the media.
    /// @param media Media structure reference.
    /// @param path File name.
    /// @returns Status.
    virtual Status fileDelete(Media& media, const char* path) const = 0;

    /// @brief Creates a directory on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @returns Status.
    virtual Status directoryCreate(Media& media, const char* path) const = 0;

    /// @brief Tests if a directory exists on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @returns Status.
    virtual Status directoryExists(Media& media, const char* path) const = 0;

    /// @brief Renames a directory on the media.
    /// @param media Media structure reference.
    /// @param oldName Old directory name.
    /// @param newName New directory name.
    /// @returns Status.
    virtual Status directoryRename(Media& media, const char* oldName, const char* newName) const = 0;

    /// @brief Deletes a directory from the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @returns Status.
    virtual Status directoryDelete(Media& media, const char* path) const = 0;

};

}
