/**
 * @file        FileSystem.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides file system metadata structure and static table. Header file.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "AdapterTypes.hpp"
#include "Media.hpp"
#include "StaticClass.hpp"

namespace FS
{

/// @brief Contains file system metadata.
struct FileSystem final : public AdapterTypes
{

    /// @brief Creates an empty file system target definition.
    FileSystem() : m_root(), m_media() { }

    /// @brief Creates a file system target definition.
    /// @param root The root path of the file system.
    /// @param media Media handle reference.
    FileSystem(const char* root, Media& media) : m_root(root), m_media(&media) { }

    /// @brief Clears the file system target definition making it empty for reuse.
    inline void clear(void) { m_root = nullptr; m_media = nullptr; m_type = MediaType::NONE; }

    /// @returns The file system root path.
    inline const char* root() const { return m_root; }

    /// @returns The file system media structure pointer.
    inline Media* media() const { return m_media; }

    /// @returns True if the file system is actually mounted.
    inline bool isMounted() const { return !!m_root && !!m_media; }

private:
friend class FileSystemTable;
friend class MediaServices;

    const char* m_root; // File system target root path pointer.
    Media* m_media;     // File system media handle pointer.
    MediaType m_type;   // Media type enumeration member.

};

/// @brief File system table.
class FileSystemTable final : public AdapterTypes
{

    STATIC(FileSystemTable)

public:

    static constexpr size_t max = 4; // The maximal amount of mount points.

    /// @brief Adds a new mount point to the mount table.
    /// @param root File system root path pointer.
    /// @param media Media structure pointer.
    /// @returns File system target pointer if successfully added, `nullptr` otherwise.
    static const FileSystem* add(const char* root, Media* media);

    /// @brief Finds the file system target matching the root of the path.
    /// @param path An absolute path with the file system root to match.
    /// @returns File system target pointer if found, `nullptr` otherwise.
    static const FileSystem* find(const char* path);

    /// @brief Finds the file system target with specified media structure pointer.
    /// @param media Media structure pointer.
    /// @returns File system target pointer if found, `nullptr` otherwise.
    static const FileSystem* find(Media* media);

    /// @brief Finds the file system target with specified media type.
    /// @param mediaType Media type enumeration member.
    /// @returns File system target pointer if found, `nullptr` otherwise.
    static const FileSystem* find(MediaType mediaType);

private:

    /// @returns A free file system entry or `nullptr` if there are no free entries left.
    static FileSystem* getFree(void);

    /// @brief Frees a mount table entry.
    /// @param entry A mount table entry.
    static void setFree(FileSystem* entry);

private:
    inline static FileSystem entries[max] = {}; // Pre-allocated mount table entries.

};

}
