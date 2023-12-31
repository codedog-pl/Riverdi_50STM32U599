/**
 * @file        FS_FILEX.hpp
 * @author      CodeDog
 *
 * @brief       File system adapter for the AzureRTOS FILEX backend.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#ifdef USE_FILEX

#include "BitFlags.hpp"
#include "FS_AdapterMethods.hpp"
#include "bindings.h"
EXTERN_C_BEGIN
#include "fx_directory.h"
EXTERN_C_END
#include <cstring>

namespace FS
{

/// @brief A unified file system access API for FILEX.
class FILEXAdapter final : public AdapterMethods
{

public:

    /// @brief Finds the directory entry that matches the path.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param entry Directory entry reference.
    /// @return Status.
    Status find(Media& media, const char* path, DirectoryEntry& entry) override
    {
        Status result = initializeEntry(media, entry); // The entry should be initialized first or the internal function call crash.
        if (result != OK) return result; // Entry initialization failed.
        result = _fx_directory_search(&media, (CHAR*)path, &entry, nullptr, nullptr); // Now the result tells if we fetched the entry.
        return result;
    }

    /// @brief Gets the file or directory creation time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    Status created(Media& media, const char* path, DateTime& dateTime) override
    {
        DirectoryEntry entry = {};
        Status result = find(media, path, entry);
        if (result != OK) return result;
        toDateTime(entry.fx_dir_entry_created_date, entry.fx_dir_entry_created_time, dateTime);
        return OK;
    }

    /// @brief Gets the file or directory last modification time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @return Status.
    Status modified(Media& media, const char* path, DateTime& dateTime) override
    {
        DirectoryEntry entry = {};
        Status result = find(media, path, entry);
        if (result != OK) return result;
        toDateTime(entry.fx_dir_entry_date, entry.fx_dir_entry_time, dateTime);
        return OK;
    }

    /// @brief Creates a file.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return Status.
    Status fileCreate(Media& media, const char* path) override
    {
        return fx_file_create(&media, (CHAR*)path);
    }

    /// @brief Tests if a file exist on the media.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @return FX_SUCCESS (0x00) if the file exitsts. FX_NOT_FOUND (0x04), FX_NOT_A_FILE (0x05) or other codes otherwise.
    Status fileExists(Media& media, const char* path) override
    {
        DirectoryEntry entry = {};
        Status result = find(media, path, entry);
        if (result != OK) return result;
        return (entry.fx_dir_entry_attributes & (FX_VOLUME | FX_DIRECTORY)) == 0 ? FX_SUCCESS : FX_NOT_A_FILE;
    }

    /// @brief Opens a file.
    /// @param media Media structure reference.
    /// @param file File handle reference.
    /// @param path A path to the file relative to the file system root.
    /// @param mode File opening mode. Default opens existing file for reading.
    /// @return Status.
    Status fileOpen(Media& media, FileHandle& file, const char* path, FileMode mode = FileMode::read) override
    {
        UINT fxMode = 0;
        UINT fxStatus = FX_SUCCESS;
        if ((mode & FileMode::read) != 0 && (mode & FileMode::write) != 0) fxMode = FX_OPEN_FOR_READ;
        else if ((mode & FileMode::read) != 0 && (mode & FileMode::write) == 0) fxMode = FX_OPEN_FOR_READ_FAST;
        else if ((mode & FileMode::read) == 0 && (mode & FileMode::write) != 0) fxMode = FX_OPEN_FOR_WRITE;

        if ((mode & FileMode::createNew) != 0)
        {
            fxStatus = fx_file_create(&media, (CHAR*)path);
            if (fxStatus == FX_ALREADY_CREATED) return fxStatus;
        }
        else if ((mode & FileMode::createAlways) != 0 || (mode & FileMode::openAlways) != 0)
        {
            fxStatus = fx_file_create(&media, (CHAR*)path);
            if (fxStatus != OK && fxStatus != FX_ALREADY_CREATED) return fxStatus;
        }

        fxStatus = fx_file_open(&media, &file, (CHAR*)path, fxMode);
        if (fxStatus != OK) return fxStatus;

        if ((mode & FileMode::openAppend) != 0) fxStatus = fx_file_seek(&file, offsetMax);
        else fxStatus = fx_file_seek(&file, 0);
        return fxStatus;
    }

    /// @brief Moves the file pointer to the specified offset.
    /// @param file File handle reference.
    /// @param offset Position within the file.
    /// @return Status.
    Status fileSeek(FileHandle& file, FileOffset offset) override
    {
        return fx_file_seek(&file, offset);
    }

    /// @brief Reads data from a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesRead Number of bytes read variable reference.
    /// @return Status.
    Status fileRead(FileHandle& file, void* buffer, size_t size, size_t& bytesRead) override
    {
        return fx_file_read(&file, buffer, size, (ULONG*)&bytesRead);
    }

    /// @brief Writes data to a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesWritten Number of bytes written variable reference.
    /// @return Status.
    virtual Status fileWrite(FileHandle& file, void* buffer, size_t size) override
    {
        return fx_file_write(&file, buffer, size);
    }

    /// @brief Closes a file.
    /// @param file File handle reference.
    /// @return Status.
    Status fileClose(FileHandle& file) override
    {
        return fx_file_close(&file);
    }

    /// @brief Renames a file.
    /// @param media Media structure reference.
    /// @param oldName Old file name.
    /// @param newName New file name.
    /// @return Status.
    Status fileRename(Media& media, const char* oldName, const char* newName) override
    {
        return fx_file_rename(&media, (CHAR*)oldName, (CHAR*)newName);
    }

    /// @brief Deletes a file.
    /// @param media Media structure reference.
    /// @param path File name.
    /// @return Status.
    Status fileDelete(Media& media, const char* path) override
    {
        return fx_file_delete(&media, (CHAR*)path);
    }

    /// @brief Creates a directory on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    Status directoryCreate(Media& media, const char* path) override
    {
        return fx_directory_create(&media, (CHAR*)path);
    }

    /// @brief Tests if a directory exists on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return FX_SUCCESS (0x00) if file exitsts. FX_NOT_FOUND (0x04) if it doesn't exist. Other code if another error occurred.
    Status directoryExists(Media& media, const char* path) override
    {
        DirectoryEntry entry = {};
        Status result = find(media, path, entry);
        if (result != OK) return false;
        return (entry.fx_dir_entry_attributes & FX_VOLUME) == 0 && (entry.fx_dir_entry_attributes & FX_DIRECTORY) != 0;
    }

    /// @brief Renames a directory on the media.
    /// @param media Media structure reference.
    /// @param oldName Old directory name.
    /// @param newName New directory name.
    /// @return Status.
    Status directoryRename(Media& media, const char* oldName, const char* newName) override
    {
        return fx_directory_rename(&media, (CHAR*)oldName, (CHAR*)newName);
    }

    /// @brief Deletes a directory from the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @return Status.
    Status directoryDelete(Media& media, const char* path) override
    {
        return fx_directory_delete(&media, (CHAR*)path);
    }

private:

    /// @brief Initializes the entry for the use with internal FILEX functions.
    /// @param media Media structure reference.
    /// @param entry Uninitialized directory entry reference.
    inline static Status initializeEntry(Media& media, DirectoryEntry& entry)
    {
        Status result = OK;
        if (media.fx_media_id != FX_MEDIA_ID) return FX_MEDIA_NOT_OPEN; // Exit early if the media is not open.
        result = tx_mutex_get(&media.fx_media_protect, TX_WAIT_FOREVER); // Before WRITING to the media structure we must ensure no other thread is using it.
        if (result != OK) return result; // Acquiring the mutex failed.
        entry.fx_dir_entry_name = media.fx_media_name_buffer + FX_MAX_LONG_NAME_LEN; // Setup pointer to media name buffer.
        entry.fx_dir_entry_short_name[0] =  0; // Clear the short name string.
        result = tx_mutex_put(&media.fx_media_protect); // Now we MUST release the mutex taken.
        if (result != OK) while(1); // OOPSIE! If we can't release a mutex taken, we crashed!
        return result;
    }

    /// @brief Converts the FILEX date and time into a `DateTime` structure.
    /// @param date FILEX date.
    /// @param time FILEX time.
    /// @param dateTime `DateTime` reference.
    static void toDateTime(UINT date, UINT time, DateTime& dateTime)
    {
        dateTime.year = ((date >> FX_YEAR_SHIFT) & FX_YEAR_MASK) + FX_BASE_YEAR;
        dateTime.month = (date >> FX_MONTH_SHIFT) & FX_MONTH_MASK;
        dateTime.day = date & FX_DAY_MASK;
        dateTime.hour = (time >> FX_HOUR_SHIFT) & FX_HOUR_MASK;
        dateTime.minute = (time >> FX_MINUTE_SHIFT) & FX_MINUTE_MASK;
        dateTime.second = (time & FX_SECOND_MASK) * 2;
        dateTime.fraction = 0;
    }

};

}

#endif
