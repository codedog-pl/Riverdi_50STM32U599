/**
 * @file        FS_API.hpp
 * @author      CodeDog
 *
 * @brief       Defines the file system adapter functions in the `FS` namespace. Header only.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "FS_File.hpp"
#include "FS_Media.hpp"
#include <utility>
#include <cstdarg>

namespace FS
{

static AdapterTypes::Status ok = AdapterTypes::OK;

/// @returns The SD card file system pointer if it was mounted. Null pointer otherwise.
inline FileSystem* SD() { return FileSystemTable::find(FS_SD_ROOT); }

/// @return The USB disk file system pointer if it was mounted. Null pointer otherwise.
inline FileSystem* USB() { return FileSystemTable::find(FS_USB_ROOT); }

/// @brief Gets the file or directory creation time.
/// @param fs File system pointer.
/// @param path File or directory path.
/// @param dateTime `DateTime` structure reference.
/// @return True if completed successfully, false otherwise.
bool created(FileSystem* fs, const char* path, DateTime& dateTime)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.created(*context.fileSystem()->media(), context.relativePath(), dateTime) == ok;
}

/// @brief Gets the file or directory last modification time.
/// @param fs File system pointer.
/// @param path File or directory path.
/// @param dateTime `DateTime` structure reference.
/// @return True if completed successfully, false otherwise.
bool modified(FileSystem* fs, const char* path, DateTime& dateTime)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.modified(*context.fileSystem()->media(), context.relativePath(), dateTime) == ok;
}

/// @brief Creates a file.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param path File path.
/// @param ...args Optional arguments used to format the target path.
/// @return True if completed successfully, false otherwise.
template<class ...va> bool fileCreate(FileSystem* fs, const char* path, va ...args)
{
    Path context(fs, path, std::forward<va>(args)...);
    if (!context.isValid()) return false;
    return adapter.fileCreate(*context.fileSystem()->media(), context.relativePath()) == ok;
}

/// @brief Tests if a file exist on the media.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param path File path.
/// @param ...args Optional arguments used to format the target path.
/// @return True if the file exists, false otherwise.
template<class ...va> bool fileExists(FileSystem* fs, const char* path, va ...args)
{
    Path context(fs, path, std::forward<va>(args)...);
    if (!context.isValid()) return false;
    return adapter.fileExists(*context.fileSystem()->media(), context.relativePath()) == ok;
}

/// @brief Renames a file on the media.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param oldName Old file name.
/// @param newName New file name.
/// @param ...args Optional arguments used to format the new name.
/// @return True if completed successfully, false otherwise.
template<class ...va> bool fileRename(FileSystem* fs, const char* oldName, const char* newName, va ...args)
{
    Path n1(fs, oldName);
    Path n2(fs, newName, std::forward<va>(args)...);
    if (!n1.isValid() || !n2.isValid()) return false;
    return adapter.fileRename(*n1.fileSystem()->media(), n1.relativePath(), n2.relativePath()) == ok;
}

/// @brief Deletes a file from the media.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param path File name.
/// @param ...args Optional arguments used to format the target path.
/// @return True if completed successfully, false otherwise.
template<class ...va> bool fileDelete(FileSystem* fs, const char* path, va ...args)
{
    Path context(fs, path, std::forward<va>(args)...);
    if (!context.isValid()) return false;
    return adapter.fileDelete(*context.fileSystem()->media(), context.relativePath()) == ok;
}

/// @brief Creates a directory on the media.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param path Directory name.
/// @param ...args Optional arguments used to format the target path.
/// @return True if completed successfully, false otherwise.
template<class ...va> bool directoryCreate(FileSystem* fs, const char* path, va ...args)
{
    Path context(fs, path, std::forward<va>(args)...);
    if (!context.isValid()) return false;
    return adapter.directoryCreate(*context.fileSystem()->media(), context.relativePath()) == ok;
}

/// @brief Tests if a directory exists on the media.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param path Directory name.
/// @param ...args Optional arguments used to format the target path.
/// @return True if completed successfully, false otherwise.
template<class ...va> bool directoryExists(FileSystem* fs, const char* path, va ...args)
{
    Path context(fs, path, std::forward<va>(args)...);
    if (!context.isValid()) return false;
    return adapter.directoryExists(*context.fileSystem()->media(), context.relativePath()) == ok;
}

/// @brief Renames a directory on the media.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param oldName Old directory name.
/// @param newName New directory name.
/// @param ...args Optional arguments used to format the new name.
/// @return True if completed successfully, false otherwise.
template<class ...va> bool directoryRename(FileSystem* fs, const char* oldName, const char* newName, va ...args)
{
    Path n1(fs, oldName);
    Path n2(fs, newName, std::forward<va>(args)...);
    if (!n1.isValid() || !n2.isValid()) return false;
    return adapter.directoryRename(*n1.fileSystem()->media(), n1.relativePath(), n2.relativePath()) == ok;
}

/// @brief Deletes a directory from the media.
/// @tparam ...va Variadic arguments type.
/// @param fs File system pointer.
/// @param path Directory name.
/// @param ...args Optional arguments used to format the target path.
/// @return True if completed successfully, false otherwise.
template<class ...va> bool directoryDelete(FileSystem* fs, const char* path, va ...args)
{
    Path context(fs, path, std::forward<va>(args)...);
    if (!context.isValid()) return false;
    return adapter.directoryDelete(*context.fileSystem()->media(), context.relativePath()) == ok;
}

}