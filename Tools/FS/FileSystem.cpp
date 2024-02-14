/**
 * @file        FileSystem.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides file system metadata structure and static table. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "FileSystem.hpp"
#include "Media.hpp"
#include <cstring>
#include <cstdarg>
#include <utility>

const FS::FileSystem* FS::FileSystemTable::add(const char* root, Media* media)
{
    if (!root || !media) return nullptr;
    auto existingEntry = find(root);
    if (existingEntry) return existingEntry->m_media == media ? existingEntry : nullptr;
    auto entry = getFree();
    if (!entry) return nullptr;
    entry->m_root = root;
    entry->m_media = media;
    auto configuration = MediaServices::getConfiguration(root);
    if (!configuration) return nullptr;
    entry->m_type = configuration->type;
    return entry;
}

const FS::FileSystem* FS::FileSystemTable::find(const char *path)
{
    for (const auto& e : entries)
    {
        if (!e.m_root) continue;
        auto rootLength = std::strlen(e.m_root);
        if (std::strncmp(path, e.m_root, rootLength) == 0) return &e;
    }
    return nullptr;
}

const FS::FileSystem* FS::FileSystemTable::find(Media *media)
{
    for (const auto& e : entries) if (e.m_media == media) return &e;
    return nullptr;
}

const FS::FileSystem *FS::FileSystemTable::find(FS::MediaType mediaType)
{
    for (const auto& e : entries) if (e.m_type == mediaType) return &e;
    return nullptr;
}

FS::FileSystem* FS::FileSystemTable::getFree(void)
{
    for (auto& e : entries) if (!e.m_root) return &e;
    return nullptr;
}

void FS::FileSystemTable::setFree(FileSystem *entry)
{
    if (!entry) return;
    for (auto& e : entries) if (&e == entry) { e.clear(); return; }
}