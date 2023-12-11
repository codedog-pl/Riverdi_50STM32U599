#pragma once

#include "bindings.h"
#if defined(USE_FILEX)
#include "fx_api.h"
#endif

namespace FS
{

/// @brief Physical media type.
enum class MediaType {
    NONE, SD, USB
};

/// @brief Media file system format.
enum class MediaFormat {
    DEFAULT, ExFAT, FAT12, FAT16, FAT32
};

#if defined(USE_FILEX)

using MediaDriver = VOID(*)(FX_MEDIA*); // Media driver type.
using MediaDriverInfo = VOID*;          // Media driver optional data.

#else

using MediaDriver = Placeholder;        // Media driver type.
using MediaDriverInfo = void*;          // Media driver optional data.

#endif

/// @brief Contains system media configuration.
struct MediaConfiguration
{
    MediaType type = MediaType::NONE;
    MediaDriver driver = nullptr;
    MediaDriverInfo driverInfo = nullptr;
};

/// @brief Provides services for media types.
class MediaServices : public AdapterTypes
{

public:

    static constexpr size_t maxConfigurations = 2;  // Maximal number of media type configurations.

    /// @brief Registers a media type.
    /// @param mediaType Media type.
    /// @param driver  Media driver (if required).
    /// @param driverInfo Media driver additional information (if required).
    static void registerType(MediaType mediaType, MediaDriver driver = nullptr, MediaDriverInfo driverInfo = nullptr)
    {
        auto configuration = getConfiguration(mediaType);
        if (!configuration) configuration = getConfiguration(MediaType::NONE);
        configuration->type = mediaType;
        configuration->driver = driver;
        configuration->driverInfo = driverInfo;
    }

    /// @brief Formats the media.
    /// @param mediaType Media type.
    /// @param format File system type.
    /// @param label Volume label.
    /// @return True if performed successfully, false otherwise.
    static bool format(MediaType mediaType, MediaFormat format, const char* label)
    {
        auto configuration = getConfiguration(mediaType);
        return false; // Not implemented yet.
    }

    /// @brief Mounts a media to a specified file system root path.
    /// @param media Media structure reference to mount.
    /// @param root File system root path pointer.
    /// @return True if the media was successfully mounted, false otherwise.
    static bool mount(Media& media, const char* root)
    {
        auto entry = FileSystemTable::find(root);
        if (!entry) // New entry.
        {
            entry = FileSystemTable::add(root, &media);
            if (!entry) return false; // FS table full.
        }
        else if (entry->media) // Existing entry with media set.
        {
            if (entry->media == &media) return OK; // Already mounted to this media.
            return false;
        }
        else entry->media = &media; // Existing entry without media set.
        entry->isMounted = true;
        entry->isIdle = true;
        return true;
    }

    /// @brief Unmouts a media from the specified system root path.
    /// @param root File system root path pointer.
    /// @return True if the media was successfully unmounted, false otherwise.
    static bool umount(const char* root)
    {
        auto entry = FileSystemTable::find(root);
        if (!entry) return false; // FS root not found.
        entry->clear();
        return true;
    }

    /// @brief Unmounts a media from a specified media structure location.
    /// @param media Media structure pointer.
    /// @return Status.
    static bool umount(Media& media)
    {
        auto entry = FileSystemTable::find(&media);
        if (!entry) return false; // Media not found.
        entry->clear();
        return true;
    }

private:

    /// @brief Gets a configuration for the specified media type.
    /// @param mediaType Media type.
    /// @return Matching configuration or `nullptr` if the type is not registered.
    static MediaConfiguration* getConfiguration(MediaType mediaType)
    {
        for (int i = 0; i < maxConfigurations; ++i)
            if (configurations[i].type == mediaType) return &configurations[i];
        return nullptr;
    }

    inline static MediaConfiguration configurations[maxConfigurations] = {}; // Configurations storage.

};

}

// using FS_Media = FS::AdapterTypes::Media;

EXTERN_C_BEGIN

void fs_register_type(FS_MediaType mediaType, FS_MediaDriver driver, FS_MediaDriverInfo driverInfo)
{
    FS::MediaServices::registerType(static_cast<FS::MediaType>(mediaType), driver, driverInfo);
}

bool fs_format(FS_MediaType mediaType, FS_MediaFormat format, const char* label)
{
    return FS::MediaServices::format(static_cast<FS::MediaType>(mediaType), static_cast<FS::MediaFormat>(format), label);
}

bool fs_mount(FS_Media* media, const char* root)
{
    return FS::MediaServices::mount(*media, root);
}

bool fs_umount(const char* root)
{
    return FS::MediaServices::umount(root);
}

bool fs_umount(FS_Media* media)
{
    return FS::MediaServices::umount(*media);
}

EXTERN_C_END
