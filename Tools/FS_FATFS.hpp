/**
 * @file        FS_FATFS.hpp
 * @author      CodeDog
 *
 * @brief       File system adapter for the FATFS backend.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "FS_AdapterMethods.hpp"
#include "FS_Path.hpp"
#include <cstring>

#ifdef USE_FATFS

namespace FS
{

/// @brief A unified file system access API for FATFS.
class FATFSAdapter final : public AdapterMethods
{

public:

    /// @brief Finds the directory entry that matches the path.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param entry Directory entry reference.
    /// @return Status.
    Status find(Media& media, const char* path, DirectoryEntry& entry) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context(fs, path);
        entry.dir = {};
        entry.info = {};
        return f_findfirst(&entry.dir, &entry.info, context.absolutePath(), "*");
    }

    /// @brief Gets the file or directory creation time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    Status created(Media& media, const char* path, DateTime& dateTime) override
    {
        return FR_NOT_ENABLED;

    }

    /// @brief Gets the file or directory last modification time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    Status modified(Media& media, const char* path, DateTime& dateTime) override
    {
        FILINFO stat = {};
        Status result = fstat(media, path, stat);
        if (result != OK) return result;
        toDateTime(stat.fdate, stat.ftime, dateTime);
        return result;
    }

    /// @brief Creates a file.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return Status.
    Status fileCreate(Media& media, const char* path) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context(fs, path);
        FileHandle file = {};
        Status result = f_open(&file, context.absolutePath(), (BYTE)FileMode::createAlways);
        if (result == OK) result = f_close(&file);
        return result;
    }

    /// @brief Tests if a file exist on the media.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return True if the file exists, false otherwise.
    Status fileExists(Media& media, const char* path) override
    {
        FILINFO info = {};
        Status result = fstat(media, path, info);
        if (result != OK) return result;
        return (info.fattrib & AM_DIR) == 0 ? FR_OK : FR_EXIST;
    }

    /// @brief Opens a file.
    /// @param media Media structure reference.
    /// @param file File handle reference.
    /// @param path A path to the file relative to the file system root.
    /// @param mode File opening mode. Default opens existing file for reading.
    /// @return Status.
    Status fileOpen(Media& media, FileHandle& file, const char* path, FileMode mode = FileMode::read) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context(fs, path);
        file = {};
        return f_open(&file, context.absolutePath(), (BYTE)mode);
    }

    /// @brief Moves the file pointer to the specified offset.
    /// @param file File handle reference.
    /// @param offset Position within the file.
    /// @return Status.
    Status fileSeek(FileHandle& file, FileOffset offset) override
    {
        return f_lseek(&file, offset);
    }

    /// @brief Reads data from a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesRead Number of bytes read variable reference.
    /// @return Status.
    Status fileRead(FileHandle& file, void* buffer, size_t size, size_t& bytesRead) override
    {
        return f_read(&file, buffer, size, &bytesRead);
    }

    /// @brief Writes data to a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesWritten Number of bytes written variable reference.
    /// @return Status.
    virtual Status fileWrite(FileHandle& file, void* buffer, size_t size) override
    {
        size_t bytesWritten = 0;
        return f_write(&file, buffer, size, &bytesWritten);
    }

    /// @brief Closes a file.
    /// @param file File handle reference.
    /// @return Status.
    Status fileClose(FileHandle& file) override
    {
        return f_close(&file);
    }

    /// @brief Renames a file.
    /// @param media Media structure reference.
    /// @param oldName Old file name.
    /// @param newName New file name.
    /// @return Status.
    Status fileRename(Media& media, const char* oldName, const char* newName) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context1(fs, oldName);
        Path context2(fs, newName);
        FILINFO stat = {};
        Status result = f_stat(context1.absolutePath(), &stat);
        if (result != OK) return result;
        if ((stat.fattrib & AM_DIR) != 0) return FR_DENIED; // Don't allow directory rename!
        return f_rename(context1.absolutePath(), context2.absolutePath());
    }

    /// @brief Deletes a file.
    /// @param media Media structure reference.
    /// @param path File name.
    /// @return Status.
    Status fileDelete(Media& media, const char* path) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context(fs, path);
        FILINFO stat = {};
        Status result = f_stat(context.absolutePath(), &stat);
        if (result != OK) return result;
        if ((stat.fattrib & AM_DIR) != 0) return FR_DENIED; // Don't allow directory delete!
        return f_unlink(context.absolutePath());
    }

    /// @brief Creates a directory on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    Status directoryCreate(Media& media, const char* path) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context(fs, path);
        return f_mkdir(context.absolutePath());
    }

    /// @brief Tests if a directory exists on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    Status directoryExists(Media& media, const char* path) override
    {
        FILINFO stat = {};
        Status result = fstat(media, path, stat);
        if (result != OK) return result;
        return (stat.fattrib & AM_DIR) != 0 ? FR_OK : FR_EXIST;
    }

    /// @brief Renames a directory on the media.
    /// @param media Media structure reference.
    /// @param oldName Old directory name.
    /// @param newName New directory name.
    /// @return Status.
    Status directoryRename(Media& media, const char* oldName, const char* newName) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context1(fs, oldName);
        Path context2(fs, newName);
        FILINFO stat = {};
        Status result = f_stat(context1.absolutePath(), &stat);
        if (result != OK) return result;
        if ((stat.fattrib & AM_DIR) == 0) return FR_DENIED; // Don't allow file rename!
        return f_rename(context1.absolutePath(), context2.absolutePath());
    }

    /// @brief Deletes a directory from the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status code.
    Status directoryDelete(Media& media, const char* path) override
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context(fs, path);
        FILINFO stat = {};
        Status result = f_stat(context.absolutePath(), &stat);
        if (result != OK) return result;
        if ((stat.fattrib & AM_DIR) == 0) return FR_DENIED; // Don't allow file delete!
        return f_unlink(context.absolutePath());
    }

private:

    /// @brief Gets the file status.
    /// @param media Media structure reference.
    /// @param path File or directory name (relative).
    /// @param stat Entry status reference.
    /// @return Status code.
    Status fstat(Media& media, const char* path, FILINFO& stat)
    {
        FileSystem* fs = FileSystemTable::find(&media);
        if (!fs) return FR_INVALID_DRIVE;
        Path context(fs, path);
        return f_stat(context.absolutePath(), &stat);
    }

    /// @brief Converts the FATFS date and time into a `DateTime` structure.
    /// @param date FATFS date.
    /// @param time FATFS time.
    /// @param dateTime `DateTime` reference.
    static void toDateTime(WORD date, WORD time, DateTime& dateTime)
    {
        reinterpret_cast<DateTimeEx&>(dateTime).getFAT((date << 8) + time);
    }

};

}

#endif
