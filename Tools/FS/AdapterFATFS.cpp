/**
 * @file        AdapterFATFS.cpp
 * @author      Adam Łyskawa
 *
 * @brief       File system adapter for the FATFS backend. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "target.h"

#ifdef USE_FATFS

#include "AdapterFATFS.hpp"
#include "Path.hpp"
#include "datetime_fat.h"
#include <cstring>

FS::AdapterTypes::Status FS::AdapterFATFS::find(Media &media, const char *path, DirectoryEntry &entry) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context(fs, path);
    entry.dir = {};
    entry.info = {};
    return f_findfirst(&entry.dir, &entry.info, context.absolutePath(), "*");
}

FS::AdapterTypes::Status FS::AdapterFATFS::created(Media &media, const char *path, DateTime &dateTime) const
{
    return FR_NOT_ENABLED;
}

FS::AdapterTypes::Status FS::AdapterFATFS::modified(Media &media, const char *path, DateTime &dateTime) const
{
    FILINFO stat = {};
    Status result = fstat(media, path, stat);
    if (result != OK) return result;
    toDateTime(stat.fdate, stat.ftime, dateTime);
    return result;
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileCreate(Media &media, const char *path) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context(fs, path);
    FileControlBlock file = {};
    Status result = f_open(&file, context.absolutePath(), (BYTE)FileMode::createAlways);
    if (result == OK) result = f_close(&file);
    return result;
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileExists(Media &media, const char *path) const
{
    FILINFO info = {};
    Status result = fstat(media, path, info);
    if (result != OK) return result;
    return (info.fattrib & AM_DIR) == 0 ? FR_OK : FR_EXIST;
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileOpen(Media &media, FileControlBlock &file, const char *path, FileMode mode) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context(fs, path);
    file = {};
    return f_open(&file, context.absolutePath(), (BYTE)mode);
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileSeek(FileControlBlock &file, FileOffset offset) const
{
    return f_lseek(&file, offset);
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileRead(FileControlBlock &file, void *buffer, size_t size, size_t &bytesRead) const
{
    return f_read(&file, buffer, size, &bytesRead);
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileWrite(FileControlBlock &file, const void *buffer, size_t size) const
{
    size_t bytesWritten = 0;
    return f_write(&file, buffer, size, &bytesWritten);
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileClose(FileControlBlock &file) const
{
    return f_close(&file);
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileRename(Media &media, const char *oldName, const char *newName) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context1(fs, oldName);
    Path context2(fs, newName);
    FILINFO stat = {};
    Status result = f_stat(context1.absolutePath(), &stat);
    if (result != OK) return result;
    if ((stat.fattrib & AM_DIR) != 0) return FR_DENIED; // Don't allow directory rename!
    return f_rename(context1.absolutePath(), context2.absolutePath());
}

FS::AdapterTypes::Status FS::AdapterFATFS::fileDelete(Media &media, const char *path) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context(fs, path);
    FILINFO stat = {};
    Status result = f_stat(context.absolutePath(), &stat);
    if (result != OK) return result;
    if ((stat.fattrib & AM_DIR) != 0) return FR_DENIED; // Don't allow directory delete!
    return f_unlink(context.absolutePath());
}

FS::AdapterTypes::Status FS::AdapterFATFS::directoryCreate(Media &media, const char *path) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context(fs, path);
    return f_mkdir(context.absolutePath());
}

FS::AdapterTypes::Status FS::AdapterFATFS::directoryExists(Media &media, const char *path) const
{
    FILINFO stat = {};
    Status result = fstat(media, path, stat);
    if (result != OK) return result;
    return (stat.fattrib & AM_DIR) != 0 ? FR_OK : FR_EXIST;
}

FS::AdapterTypes::Status FS::AdapterFATFS::directoryRename(Media &media, const char *oldName, const char *newName) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context1(fs, oldName);
    Path context2(fs, newName);
    FILINFO stat = {};
    Status result = f_stat(context1.absolutePath(), &stat);
    if (result != OK) return result;
    if ((stat.fattrib & AM_DIR) == 0) return FR_DENIED; // Don't allow file rename!
    return f_rename(context1.absolutePath(), context2.absolutePath());
}

FS::AdapterTypes::Status FS::AdapterFATFS::directoryDelete(Media &media, const char *path) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context(fs, path);
    FILINFO stat = {};
    Status result = f_stat(context.absolutePath(), &stat);
    if (result != OK) return result;
    if ((stat.fattrib & AM_DIR) == 0) return FR_DENIED; // Don't allow file delete!
    return f_unlink(context.absolutePath());
}

FS::AdapterTypes::Status FS::AdapterFATFS::fstat(Media &media, const char *path, FILINFO &stat) const
{
    auto fs = FileSystemTable::find(&media);
    if (!fs) return FR_INVALID_DRIVE;
    Path context(fs, path);
    return f_stat(context.absolutePath(), &stat);
}

void FS::AdapterFATFS::toDateTime(WORD date, WORD time, DateTime &dateTime)
{
    FAT2DateTime((date << 16) + time, reinterpret_cast<DateTimeTypeDef*>(&dateTime));
}

#endif
