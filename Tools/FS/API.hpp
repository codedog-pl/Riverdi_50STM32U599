/**
 * @file        API.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Declares the file system adapter functions in the `FS` namespace. Header file.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "DateTime.hpp"
#include "File.hpp"
#include "Media.hpp"

namespace FS
{

/// @returns The eMMC chip file system pointer if it was mounted. Null pointer otherwise.
inline const FileSystem* eMMC() { return FileSystemTable::find(MediaType::eMMC); }

/// @returns The SD card file system pointer if it was mounted. Null pointer otherwise.
inline const FileSystem* SD() { return FileSystemTable::find(MediaType::SD); }

/// @returns The USB disk file system pointer if it was mounted. Null pointer otherwise.
inline const FileSystem* USB() { return FileSystemTable::find(MediaType::USB); }

/// @returns The internal file system pointer if it was mounter. Null pointer otherwise.
const FileSystem* internal();

/// @returns The external file system pointer if it was mounted. Null pointer otherwise.
inline const FileSystem* external() { return FileSystemTable::find(MediaType::USB); }

/// @brief Gets the file or directory creation time.
/// @param fs File system pointer.
/// @param path File or directory path.
/// @param dateTime `DateTime` structure reference.
/// @returns True if completed successfully, false otherwise.
bool created(const FileSystem* fs, const char* path, DateTime& dateTime);

/// @brief Gets the file or directory last modification time.
/// @param fs File system pointer.
/// @param path File or directory path.
/// @param dateTime `DateTime` structure reference.
/// @returns True if completed successfully, false otherwise.
bool modified(const FileSystem* fs, const char* path, DateTime& dateTime);

/// @brief Creates a file.
/// @param fs File system pointer.
/// @param path File path.
/// @param ... Optional arguments used to format the target path.
/// @returns True if completed successfully, false otherwise.
bool fileCreate(const FileSystem* fs, const char* path, ...);

/// @brief Tests if a file exist on the media.
/// @param fs File system pointer.
/// @param path File path.
/// @param ... Optional arguments used to format the target path.
/// @returns True if the file exists, false otherwise.
bool fileExists(const FileSystem* fs, const char* path, ...);

/// @brief Renames a file on the media.
/// @param fs File system pointer.
/// @param oldName Old file name.
/// @param newName New file name.
/// @param ...args Optional arguments used to format the new name.
/// @returns True if completed successfully, false otherwise.
bool fileRename(const FileSystem* fs, const char* oldName, const char* newName, ...);

/// @brief Deletes a file from the media.
/// @param fs File system pointer.
/// @param path File name.
/// @param ... Optional arguments used to format the target path.
/// @returns True if completed successfully, false otherwise.
bool fileDelete(const FileSystem* fs, const char* path, ...);

/// @brief Creates a directory on the media.
/// @param fs File system pointer.
/// @param path Directory name.
/// @param ... Optional arguments used to format the target path.
/// @returns True if completed successfully, false otherwise.
bool directoryCreate(const FileSystem* fs, const char* path, ...);

/// @brief Tests if a directory exists on the media.
/// @param fs File system pointer.
/// @param path Directory name.
/// @param ... Optional arguments used to format the target path.
/// @returns True if completed successfully, false otherwise.
bool directoryExists(const FileSystem* fs, const char* path, ...);

/// @brief Renames a directory on the media.
/// @param fs File system pointer.
/// @param oldName Old directory name.
/// @param newName New directory name.
/// @param ... Optional arguments used to format the new name.
/// @returns True if completed successfully, false otherwise.
bool directoryRename(const FileSystem* fs, const char* oldName, const char* newName, ...);

/// @brief Deletes a directory from the media.
/// @param fs File system pointer.
/// @param path Directory name.
/// @param ... Optional arguments used to format the target path.
/// @returns True if completed successfully, false otherwise.
bool directoryDelete(const FileSystem* fs, const char* path, ...);

}
