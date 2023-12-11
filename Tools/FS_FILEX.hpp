#pragma once

#ifdef USE_FILEX

#include "FS_AdapterMethods.hpp"
#include "fx_directory.h"
#include <cstring>

namespace FS
{

/// @brief A unified file system access API for FILEX.
class FILEX final : public AdapterMethods
{

public:

    /// @brief Finds the directory entry that matches the path.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param entry Directory entry reference.
    /// @return Status.
    Status find(Media& media, const char* path, DirectoryEntry& entry) override
    {
        Status result = OK;
        result = tx_mutex_get(&media.fx_media_protect, TX_WAIT_ERROR);
        if (result != OK) return false;
        result = _fx_directory_search(&media, (CHAR*)path, &entry, nullptr, nullptr);
        tx_mutex_put(&media.fx_media_protect);
        return result = OK;
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
    /// @return True if the file exists, false otherwise.
    bool fileExists(Media& media, const char* path) override
    {
        DirectoryEntry entry = {};
        Status result = find(media, path, entry);
        if (result != OK) return false;
        return (entry.fx_dir_entry_attributes & (FX_VOLUME | FX_DIRECTORY)) == 0;
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

        if ((mode & FileMode::createNew))
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

        if ((mode & FileMode::openAppend)) fxStatus = fx_file_seek(&file, offsetMax);
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
        Status result = fx_file_read(&file, buffer, size, (ULONG*)&bytesRead);
    }

    /// @brief Writes data to a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesWritten Number of bytes written variable reference.
    /// @return Status.
    virtual Status fileWrite(FileHandle& file, void* buffer, size_t size) override
    {
        Status result = fx_file_write(&file, buffer, size);
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
    /// @return Status.
    bool directoryExists(Media& media, const char* path) override
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