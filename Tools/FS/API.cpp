/**
 * @file        API.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Defines the file system adapter functions in the `FS` namespace. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "Adapter.hpp"
#include "API.hpp"
#include "DateTime.hpp"
#include <utility>
#include <cstdarg>

USE_ADAPTER

static constexpr FS::AdapterTypes::Status ok = FS::AdapterTypes::OK;

const FS::FileSystem* FS::internal()
{
    FileSystem* fs = nullptr;
    fs = const_cast<FileSystem*>(FileSystemTable::find(MediaType::eMMC));
    if (fs) return const_cast<FileSystem*>(fs);
    return FileSystemTable::find(MediaType::SD);
}

bool FS::created(const FileSystem *fs, const char *path, DateTime &dateTime)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.created(*context.fileSystem()->media(), context.relativePath(), dateTime) == ok;
}

bool FS::modified(const FileSystem *fs, const char *path, DateTime &dateTime)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.modified(*context.fileSystem()->media(), context.relativePath(), dateTime) == ok;
}

bool FS::fileCreate(const FileSystem *fs, const char *path, ...)
{
    va_list args;
    va_start(args, path);
    Path context(args, fs, path);
    va_end(args);
    if (!context.isValid()) return false;
    return adapter.fileCreate(*context.fileSystem()->media(), context.relativePath()) == ok;
}

bool FS::fileExists(const FileSystem *fs, const char *path, ...)
{
    va_list args;
    va_start(args, path);
    Path context(args, fs, path);
    va_end(args);
    if (!context.isValid()) return false;
    return adapter.fileExists(*context.fileSystem()->media(), context.relativePath()) == ok;
}

bool FS::fileRename(const FileSystem *fs, const char *oldName, const char *newName, ...)
{
    va_list args1, args2;
    va_start(args1, newName);
    va_copy(args2, args1);
    Path n1(args1, fs, oldName);
    Path n2(args2, fs, newName);
    va_end(args2);
    va_end(args1);
    if (!n1.isValid() || !n2.isValid()) return false;
    return adapter.fileRename(*n1.fileSystem()->media(), n1.relativePath(), n2.relativePath()) == ok;
}

bool FS::fileDelete(const FileSystem *fs, const char *path, ...)
{
    va_list args;
    va_start(args, path);
    Path context(args, fs, path);
    va_end(args);
    if (!context.isValid()) return false;
    return adapter.fileDelete(*context.fileSystem()->media(), context.relativePath()) == ok;
}

bool FS::directoryCreate(const FileSystem *fs, const char *path, ...)
{
    va_list args;
    va_start(args, path);
    Path context(args, fs, path);
    va_end(args);
    if (!context.isValid()) return false;
    return adapter.directoryCreate(*context.fileSystem()->media(), context.relativePath()) == ok;
}

bool FS::directoryExists(const FileSystem *fs, const char *path, ...)
{
    va_list args;
    va_start(args, path);
    Path context(args, fs, path);
    va_end(args);
    if (!context.isValid()) return false;
    return adapter.directoryExists(*context.fileSystem()->media(), context.relativePath()) == ok;
}

bool FS::directoryRename(const FileSystem *fs, const char *oldName, const char *newName, ...)
{
    va_list args1, args2;
    va_start(args1, newName);
    va_copy(args2, args1);
    Path n1(args1, fs, oldName);
    Path n2(args2, fs, newName);
    va_end(args2);
    va_end(args1);
    if (!n1.isValid() || !n2.isValid()) return false;
    return adapter.directoryRename(*n1.fileSystem()->media(), n1.relativePath(), n2.relativePath()) == ok;
}

bool FS::directoryDelete(const FileSystem *fs, const char *path, ...)
{
    va_list args;
    va_start(args, path);
    Path context(args, fs, path);
    va_end(args);
    if (!context.isValid()) return false;
    return adapter.directoryDelete(*context.fileSystem()->media(), context.relativePath()) == ok;
}
