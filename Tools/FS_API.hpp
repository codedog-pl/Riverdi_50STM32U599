#pragma once

#include "FS_File.hpp"
#include "FS_Media.hpp"

namespace FS
{

/// @brief Gets the file or directory creation time.
/// @param fs File system reference.
/// @param path File or directory path.
/// @param dateTime `DateTime` structure reference.
/// @return True if completed successfully, false otherwise.
bool created(FileSystem& fs, const char* path, DateTime& dateTime);

/// @brief Gets the file or directory last modification time.
/// @param fs File system reference.
/// @param path File or directory path.
/// @param dateTime `DateTime` structure reference.
/// @return True if completed successfully, false otherwise.
bool modified(FileSystem& fs, const char* path, DateTime& dateTime);

/// @brief Creates a file.
/// @param fs File system reference.
/// @param path File path.
/// @return True if completed successfully, false otherwise.
bool fileCreate(FileSystem& fs, const char* path);

/// @brief Tests if a file exist on the media.
/// @param fs File system reference.
/// @param path File path.
/// @return True if the file exists, false otherwise.
bool fileExists(FileSystem& fs, const char* path);

/// @brief Renames a file on the media.
/// @param fs File system reference.
/// @param oldName Old file name.
/// @param newName New file name.
/// @return True if completed successfully, false otherwise.
bool fileRename(FileSystem& fs, const char* oldName, const char* newName);

/// @brief Deletes a file from the media.
/// @param fs File system reference.
/// @param path File name.
/// @return True if completed successfully, false otherwise.
bool fileDelete(FileSystem& fs, const char* path);

/// @brief Creates a directory on the media.
/// @param fs File system reference.
/// @param path Directory name.
/// @return True if completed successfully, false otherwise.
bool directoryCreate(FileSystem& fs, const char* path);

/// @brief Tests if a directory exists on the media.
/// @param fs File system reference.
/// @param path Directory name.
/// @return True if completed successfully, false otherwise.
bool directoryExists(FileSystem& fs, const char* path);

/// @brief Renames a directory on the media.
/// @param fs File system reference.
/// @param oldName Old directory name.
/// @param newName New directory name.
/// @return True if completed successfully, false otherwise.
bool directoryRename(FileSystem& fs, const char* oldName, const char* newName);

/// @brief Deletes a directory from the media.
/// @param fs File system reference.
/// @param path Directory name.
/// @return True if completed successfully, false otherwise.
bool directoryDelete(FileSystem& fs, const char* path);

}