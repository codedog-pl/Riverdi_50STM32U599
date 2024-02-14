/**
 * @file        AdapterFILEX.cpp
 * @author      Adam Łyskawa
 *
 * @brief       File system adapter for the Azure RTOS FILEX backend. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "target.h"

#ifdef USE_FILEX

#include "AdapterFILEX.hpp"
#include "BitFlags.hpp"

#include "bindings.h"
EXTERN_C_BEGIN
#include "fx_directory.h"
EXTERN_C_END
#include <cstring>

FS::AdapterTypes::Status FS::AdapterFILEX::find(Media &media, const char *path, DirectoryEntry &entry) const
{
    Status result = initializeEntry(media, entry); // The entry should be initialized first or the internal function call crash.
    if (result != OK) return result; // Entry initialization failed.
    result = _fx_directory_search(&media, (CHAR*)path, &entry, nullptr, nullptr); // Now the result tells if we fetched the entry.
    return result;
}

FS::AdapterTypes::Status FS::AdapterFILEX::created(Media &media, const char *path, DateTime &dateTime) const
{
    DirectoryEntry entry = {};
    Status result = find(media, path, entry);
    if (result != OK) return result;
    toDateTime(entry.fx_dir_entry_created_date, entry.fx_dir_entry_created_time, dateTime);
    return OK;
}

FS::AdapterTypes::Status FS::AdapterFILEX::modified(Media &media, const char *path, DateTime &dateTime) const
{
    DirectoryEntry entry = {};
    Status result = find(media, path, entry);
    if (result != OK) return result;
    toDateTime(entry.fx_dir_entry_date, entry.fx_dir_entry_time, dateTime);
    return OK;
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileCreate(Media &media, const char *path) const
{
    return fx_file_create(&media, (CHAR*)path);
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileExists(Media &media, const char *path) const
{
    DirectoryEntry entry = {};
    Status result = find(media, path, entry);
    if (result != OK) return result;
    return (entry.fx_dir_entry_attributes & (FX_VOLUME | FX_DIRECTORY)) == 0 ? FX_SUCCESS : FX_NOT_A_FILE;
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileOpen(Media &media, FileControlBlock &file, const char *path, FileMode mode) const
{
    UINT fxMode = 0;
    UINT fxStatus = FX_SUCCESS;

    if (mode & read) fxMode = (mode & write) ? FX_OPEN_FOR_READ : FX_OPEN_FOR_READ_FAST;
    else if (mode & write) fxMode = FX_OPEN_FOR_WRITE;
    if (mode & createNew)
    {
        fxStatus = fx_file_create(&media, (CHAR*)path);
        if (fxStatus == FX_ALREADY_CREATED) return fxStatus;
    }
    else if (mode & (createAlways | openAlways))
    {
        fxStatus = fx_file_create(&media, (CHAR*)path);
        if (fxStatus != OK && fxStatus != FX_ALREADY_CREATED) return fxStatus;
    }

    fxStatus = fx_file_open(&media, &file, (CHAR*)path, fxMode);
    if (fxStatus != OK) return fxStatus;

    if (BF::isSet(FileMode::openAppend, mode)) fxStatus = fx_file_seek(&file, offsetMax);
    else fxStatus = fx_file_seek(&file, 0);
    return fxStatus;
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileSeek(FileControlBlock &file, FileOffset offset) const
{
    return fx_file_seek(&file, offset);
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileRead(FileControlBlock &file, void *buffer, size_t size, size_t &bytesRead) const
{
    return fx_file_read(&file, buffer, size, (ULONG*)&bytesRead);
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileWrite(FileControlBlock &file, const void *buffer, size_t size) const
{
    return fx_file_write(&file, const_cast<void*>(buffer), size);
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileClose(FileControlBlock &file) const
{
    return fx_file_close(&file);
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileRename(Media &media, const char *oldName, const char *newName) const
{
    return fx_file_rename(&media, (CHAR*)oldName, (CHAR*)newName);
}

FS::AdapterTypes::Status FS::AdapterFILEX::fileDelete(Media &media, const char *path) const
{
    return fx_file_delete(&media, (CHAR*)path);
}

FS::AdapterTypes::Status FS::AdapterFILEX::directoryCreate(Media &media, const char *path) const
{
    return fx_directory_create(&media, (CHAR*)path);
}

FS::AdapterTypes::Status FS::AdapterFILEX::directoryExists(Media &media, const char *path) const
{
    DirectoryEntry entry = {};
    Status result = find(media, path, entry);
    if (result != OK) return false;
    return (entry.fx_dir_entry_attributes & FX_VOLUME) == 0 && (entry.fx_dir_entry_attributes & FX_DIRECTORY) != 0;
}

FS::AdapterTypes::Status FS::AdapterFILEX::directoryRename(Media &media, const char *oldName, const char *newName) const
{
    return fx_directory_rename(&media, (CHAR*)oldName, (CHAR*)newName);
}

FS::AdapterTypes::Status FS::AdapterFILEX::directoryDelete(Media &media, const char *path) const
{
    return fx_directory_delete(&media, (CHAR*)path);
}

FS::AdapterTypes::Status FS::AdapterFILEX::initializeEntry(Media &media, DirectoryEntry &entry)
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

void FS::AdapterFILEX::toDateTime(UINT date, UINT time, DateTime &dateTime)
{
    dateTime.year = ((date >> FX_YEAR_SHIFT) & FX_YEAR_MASK) + FX_BASE_YEAR;
    dateTime.month = (date >> FX_MONTH_SHIFT) & FX_MONTH_MASK;
    dateTime.day = date & FX_DAY_MASK;
    dateTime.hour = (time >> FX_HOUR_SHIFT) & FX_HOUR_MASK;
    dateTime.minute = (time >> FX_MINUTE_SHIFT) & FX_MINUTE_MASK;
    dateTime.second = (time & FX_SECOND_MASK) * 2;
    dateTime.fraction = 0;
}

#endif
