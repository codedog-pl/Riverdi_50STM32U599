#pragma once

#include "FS_AdapterTypes.hpp"
#include <cstring>
#include <cstdarg>
#include <utility>

namespace FS
{

/// @brief File system entry.
struct FileSystem final : public AdapterTypes
{

    /// @brief Creates an empty file system target definition.
    FileSystem() : m_root(), m_media() { }

    /// @brief Creates a file system target definition.
    /// @param root The root path of the file system.
    /// @param media Media handle reference.
    FileSystem(const char* root, Media& media) : m_root(root), m_media(&media) { }

    /// @brief Clears the file system target definition making it empty for reuse.
    void clear(void) { m_root = nullptr; m_media = nullptr; }

    /// @returns The file system root path.
    inline const char* root() const { return m_root; }

    /// @returns The file system media structure pointer.
    inline Media* media() const { return m_media; }

    /// @returns True if the file system is actually mounted.
    inline bool isMounted() const { return !!m_root && !!m_media; }

private:
friend class FileSystemTable;
friend class MediaServices;

    const char* m_root;           // File system target root path pointer.
    Media* m_media;               // File system media handle pointer.

};

/// @brief File system table.
class FileSystemTable final : public AdapterTypes
{

public:

    static constexpr size_t max = 4; // The maximal amount of mount points.

    /// @brief Adds a new mount point to the mount table.
    /// @param root File system root path pointer.
    /// @param media Media structure pointer.
    /// @returns File system target pointer if successfully added, `nullptr` otherwise.
    static FileSystem* add(const char* root, Media* media)
    {
        auto existingEntry = find(root);
        if (existingEntry) return existingEntry->m_media == media ? existingEntry : nullptr;
        auto entry = getFree();
        if (!entry) return nullptr;
        entry->m_root = root;
        entry->m_media = media;
        return entry;
    }

    /// @brief Finds the file system target matching the root of the path.
    /// @param path An absolute path with the file system root to match.
    /// @returns Mount table entry if the path is matched, `nullptr` otherwise.
    static FileSystem* find(const char* path)
    {
        for (size_t i = 0; i < max; i++)
        {
            auto& entry = entries[i];
            if (!entry.m_root) continue;
            auto rootLength = std::strlen(entry.m_root);
            if (std::strncmp(path, entry.m_root, rootLength) == 0) return &entry;
        }
        return nullptr;
    }

    /// @brief Finds the file system target with specified media structure pointer.
    /// @param media Media structure pointer.
    /// @return Mount table entry if the media is found, `nullptr` otherwise.
    static FileSystem* find(Media* media)
    {
        for (size_t i = 0; i < max; i++) if (entries[i].m_media == media) return &entries[i];
        return nullptr;
    }

private:

    /// @returns A free mount table entry or `nullptr` if there are no free entries left.
    static FileSystem* getFree(void)
    {
        for (size_t i = 0; i < max; ++i) if (!entries[i].m_root) return &entries[i];
        return nullptr;
    }

    /// @brief Frees a mount table entry.
    /// @param entry A mount table entry.
    static void setFree(FileSystem* entry)
    {
        if (!entry) return;
        for (size_t i = 0; i < max; ++i) if (&entries[i] == entry) { entry->clear(); return; }
    }

private:
    inline static FileSystem entries[max] = {}; // Pre-allocated mount table entries.

};

}
