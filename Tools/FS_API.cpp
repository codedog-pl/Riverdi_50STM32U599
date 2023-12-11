#include "FS_API.hpp"

static FS::AdapterTypes::Status ok = FS::AdapterTypes::OK;

bool FS::created(FileSystem &fs, const char* path, DateTime &dateTime)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.created(*context.fileSystem->media, context.relativePath, dateTime) == ok;
}

bool FS::modified(FileSystem &fs, const char *path, DateTime &dateTime)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.modified(*context.fileSystem->media, context.relativePath, dateTime) == ok;
}

bool FS::fileCreate(FileSystem &fs, const char *path)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.fileCreate(*context.fileSystem->media, context.relativePath) == ok;
}

bool FS::fileExists(FileSystem &fs, const char *path)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.fileExists(*context.fileSystem->media, context.relativePath) == ok;
}

bool FS::fileRename(FileSystem &fs, const char *oldName, const char *newName)
{
    Path n1(fs, oldName);
    Path n2(fs, newName);
    if (!n1.isValid() || !n2.isValid()) return false;
    return adapter.fileRename(*n1.fileSystem->media, n1.relativePath, n2.relativePath) == ok;
}

bool FS::fileDelete(FileSystem &fs, const char *path)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.fileDelete(*context.fileSystem->media, context.relativePath) == ok;
}

bool FS::directoryCreate(FileSystem &fs, const char *path)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.directoryCreate(*context.fileSystem->media, context.relativePath) == ok;
}

bool FS::directoryExists(FileSystem &fs, const char *path)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.directoryExists(*context.fileSystem->media, context.relativePath) == ok;
}

bool FS::directoryRename(FileSystem &fs, const char *oldName, const char *newName)
{
    Path n1(fs, oldName);
    Path n2(fs, newName);
    if (!n1.isValid() || !n2.isValid()) return false;
    return adapter.directoryRename(*n1.fileSystem->media, n1.relativePath, n2.relativePath) == ok;
}

bool FS::directoryDelete(FileSystem &fs, const char *path)
{
    Path context(fs, path);
    if (!context.isValid()) return false;
    return adapter.directoryDelete(*context.fileSystem->media, context.relativePath) == ok;
}
