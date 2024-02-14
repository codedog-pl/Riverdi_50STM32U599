/**
 * @file        Path.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Defines a file system path in a file system context. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "Path.hpp"
#include <cstdarg>
#include <cstring>
#include <cstdio>

FS::Path::Path() : m_fileSystem(), m_absolutePath(), m_relativePath() { }

FS::Path::Path(va_list args, const char* path) : m_fileSystem(), m_absolutePath(), m_relativePath()
{
    initializeWithVariadicArgs(path, args);
}

FS::Path::Path(va_list args, const FileSystem *fs, const char *path) : m_fileSystem(), m_absolutePath(), m_relativePath()
{
    initializeWithVariadicArgs(fs, path, args);
}

FS::Path::Path(const char *path, ...) : m_fileSystem(), m_absolutePath(), m_relativePath()
{
    va_list args;
    va_start(args, path);
    *this = Path(args, path);
    va_end(args);
}

FS::Path::Path(const FileSystem *fs, const char *path, ...) : m_fileSystem(), m_absolutePath(), m_relativePath()
{
    va_list args;
    va_start(args, path);
    *this = Path(args, fs, path);
    va_end(args);
}

void FS::Path::initializeWithVariadicArgs(const char *path, va_list args)
{
    m_fileSystem = FileSystemTable::find(path);
    if (!m_fileSystem || !m_fileSystem->root()) return;
    va_list argsCopy;
    va_copy(argsCopy, args);
    std::vsnprintf(&m_absolutePath[0], maxLength, path, args);
    std::vsnprintf(&m_relativePath[0], maxLength, path + std::strlen(m_fileSystem->root()), argsCopy);
    va_end(argsCopy);
}

void FS::Path::initializeWithVariadicArgs(const FileSystem* fs, const char *path, va_list args)
{
    if (!fs || !fs->root()) return;
    m_fileSystem = fs;
    auto rootLength = std::strlen(m_fileSystem->root());
    std::memcpy(&m_absolutePath, m_fileSystem->root(), rootLength);
    va_list argsCopy;
    va_copy(argsCopy, args);
    std::vsnprintf(&m_relativePath[0], maxLength, path, args);
    std::vsnprintf(&m_absolutePath[0] + rootLength, maxLength - rootLength, path, argsCopy);
    va_end(argsCopy);
}
