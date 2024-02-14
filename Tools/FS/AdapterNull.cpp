/**
 * @file        AdapterNull.cpp
 * @author      Adam Łyskawa
 *
 * @brief       An empty file system adapter for no file system backend defined.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @remarks     Used as a template for concrete file system adapters or as a template for a new file system adapter.
 *              Simulates Linux /dev/null device.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "target.h"

#if !defined(USE_FILEX) && !defined(USE_FATFS)

#include "AdapterNull.hpp"
#include "BitFlags.hpp"

FS::AdapterTypes::Status FS::AdapterNull::find(Media &media, const char *path, DirectoryEntry &entry) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::created(Media &media, const char *path, DateTime &dateTime) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::modified(Media &media, const char *path, DateTime &dateTime) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::fileCreate(Media &media, const char *path) const
{
    return OK;
}

FS::AdapterTypes::Status FS::AdapterNull::fileExists(Media &media, const char *path) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::fileOpen(Media &media, FileControlBlock &file, const char *path, FileMode mode) const
{
    if (!BF::isSet(FileMode::write, mode) || file.isUsed) return FS_NEGATIVE;
    file.isUsed = true;
    return OK;
}

FS::AdapterTypes::Status FS::AdapterNull::fileSeek(FileControlBlock &file, FileOffset offset) const
{
    return file.isUsed ? OK : FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::fileRead(FileControlBlock &file, void *buffer, size_t size, size_t &bytesRead) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::fileWrite(FileControlBlock &file, const void *buffer, size_t size) const
{
    if (!file.isUsed || !buffer || !size) return FS_NEGATIVE;
    return OK;
}

FS::AdapterTypes::Status FS::AdapterNull::fileClose(FileControlBlock &file) const
{
    if (!file.isUsed) return FS_NEGATIVE;
    file.isUsed = false;
    return OK;
}

FS::AdapterTypes::Status FS::AdapterNull::fileRename(Media &media, const char *oldName, const char *newName) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::fileDelete(Media &media, const char *path) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::directoryCreate(Media &media, const char *path) const
{
    return OK;
}

FS::AdapterTypes::Status FS::AdapterNull::directoryExists(Media &media, const char *path) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::directoryRename(Media &media, const char *oldName, const char *newName) const
{
    return FS_NEGATIVE;
}

FS::AdapterTypes::Status FS::AdapterNull::directoryDelete(Media &media, const char *path) const
{
    return FS_NEGATIVE;
}

#endif
