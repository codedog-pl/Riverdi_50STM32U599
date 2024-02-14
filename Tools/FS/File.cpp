/**
 * @file        File.cpp
 * @author      Adam Łyskawa
 *
 * @brief       RAII file access API. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "File.hpp"
#include "Adapter.hpp"
#include <cstdarg>

USE_ADAPTER

void FS::File::open()
{
    if (!isValid() || isOpen()) return; // Invalid path or media, obviously file not found.
    m_status = adapter.fileOpen(*m_fileSystem->media(), m_file, m_relativePath, m_mode);
    m_isOpen = m_status == OK;
}

FS::File::File(const char *absolutePath, FileMode pMode, ...) : Path(), m_mode(pMode), m_isOpen(false)
{
    va_list args;
    va_start(args, pMode);
    initializeWithVariadicArgs(absolutePath, args);
    va_end(args);
    open();
}

FS::File::File(Path &path, FileMode pMode, ...) : Path(), m_mode(pMode), m_isOpen(false)
{
    va_list args;
    va_start(args, pMode);
    initializeWithVariadicArgs(path.fileSystem(), path.relativePath(), args);
    va_end(args);
    open();
}

FS::File::File(const FileSystem *fs, const char *relativePath, FileMode pMode, ...)
    : Path(), m_mode(pMode), m_isOpen(false)
{
    va_list args;
    va_start(args, pMode);
    initializeWithVariadicArgs(fs, relativePath, args);
    va_end(args);
    open();
}

FS::File::~File() { close(); }

bool FS::File::seek(FileOffset offset)
{
    if (!m_isOpen) return false;
    return adapter.fileSeek(m_file, offset) == OK;
}

FS::ReadResult FS::File::read(void *buffer, size_t size)
{
    if (!m_isOpen || !buffer || !size) return ReadResult();
    size_t bytesRead;
    return adapter.fileRead(m_file, buffer, size, bytesRead) == OK ? ReadResult(bytesRead) : ReadResult();
}

bool FS::File::write(const void *buffer, size_t size)
{
    if (!m_isOpen || !buffer || !size) return false;
    return adapter.fileWrite(m_file, buffer, size) == OK;
}

void FS::File::close()
{
    if (!m_isOpen) return;
    m_status = adapter.fileClose(m_file);
    m_isOpen = m_status != OK;  // If close failed, assume the file is still open.
    if (!m_isOpen) m_file = {}; // Clear the file handle just in case.
}
