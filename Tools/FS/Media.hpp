/**
 * @file        Media.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides C++ types and services for the file system media. Header file.
 * @remark      A part of the Woof Toolkit (WTK), File System API
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "fs_bindings.h"
#include "Action.hpp"
#include "AdapterTypes.hpp"
#include "StaticClass.hpp"

namespace FS
{

/// @brief Physical media type.
enum class MediaType {
    NONE, eMMC, SD, USB
};

/// @brief Media file system format.
enum class MediaFormat {
    DEFAULT, ExFAT, FAT12, FAT16, FAT32
};

using MediaDriver = FS_MediaDriver;         // Media driver type.
using MediaRoot = FS_MediaRoot;             // Media root path.

/// @brief Contains system media configuration.
struct MediaConfiguration
{
    MediaType type = MediaType::NONE;
    const char* root = nullptr;
    MediaDriver driver = nullptr;
};

/// @brief Provides services for media types.
class MediaServices : public AdapterTypes
{

STATIC(MediaServices)

public:

    static constexpr size_t maxConfigurations = 3;  // Maximal number of media type configurations.

    /// @brief Registers a media type.
    /// @param mediaType Media type.
    /// @param root Root path. For FATFS compatibility it needs to be formatted like "0:/".
    /// @param driver  Media driver (if required).
    static void registerType(MediaType mediaType, const char* root, MediaDriver driver = nullptr);

    /// @brief Gets a configuration for the specified media type.
    /// @param mediaType Media type.
    /// @returns Matching configuration or `nullptr` if the type is not registered.
    static const MediaConfiguration* getConfiguration(MediaType mediaType);

    /// @brief Gets the configuration for the specified root, if it was registered.
    /// @param root File system root path.
    /// @returns Matching configuration or `nullptr` if the root is not registered.
    static const MediaConfiguration* getConfiguration(const char* root);

    /// @brief Formats the media.
    /// @param root File system root path.
    /// @param format File system type.
    /// @param label Volume label.
    /// @returns True if performed successfully, false otherwise.
    static bool format(const char* root, MediaFormat format, const char* label);

    /// @brief Mounts a media to a specified file system root path.
    /// @param media Media structure reference to mount.
    /// @param root File system root path pointer.
    /// @returns True if the media was successfully mounted, false otherwise.
    static bool mount(Media& media, const char* root);

    /// @brief Unmouts a media from the specified system root path.
    /// @param root File system root path.
    /// @returns True if the media was successfully unmounted, false otherwise.
    static bool umount(const char* root);

    /// @brief Unmounts a media from a specified media structure location.
    /// @param media Media structure pointer.
    /// @returns Status.
    static bool umount(Media& media);

    /// @brief Notifies the subscriber that the `FS::FileSystemTable` entries changed.
    static void notifyChanged(void);

public:

    inline static Action mountNotify = nullptr; // An action to be called when a file system is mounted or unmounted.

private:

    inline static MediaConfiguration configurations[maxConfigurations] = {}; // Configurations storage.

};

}
