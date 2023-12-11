#pragma once

#include <stdbool.h>
#include <stddef.h>

// Common types:

/// @brief A placeholder structure for a NULL file system.
typedef struct
{
    bool isUsed = 0;    // 1: The structure is in use. 0: The structure is reset.
} FS_Placeholder;

/// @brief Physical media type.
typedef enum
{
    FS_MEDIA_NONE, FS_MEDIA_SD, FS_MEDIA_USB
} FS_MediaType;

typedef enum
{
    FS_FORMAT_DEFAULT, FS_FORMAT_ExFAT, FS_FORMAT_FAT12, FS_FORMAT_FAT16, FS_FORMAT_FAT32
} FS_MediaFormat;

// File system middleware backend specific:

#if defined(USE_FILEX)

// FILEX types:

#include "fx_api.h"
typedef VOID (*FS_MediaDriver)(FX_MEDIA*);
typedef VOID*           FS_MediaDriverInfo;
typedef FX_MEDIA        FS_Media;
typedef FX_DIR_ENTRY    FS_DirectoryEntry;
typedef FX_FILE         FS_FileHandle;
typedef ULONG           FS_FileOffset;
typedef UINT            FS_Status;

#else

// `NullAdapter` types:

typedef void*           FS_MediaDriver;
typedef void*           FS_MediaDriverInfo;
typedef FS_Placeholder  FS_Media;
typedef FS_Placeholder  FS_DirectoryEntry;
typedef FS_Placeholder  FS_FileHandle;
typedef size_t          FS_FileOffset;
typedef int             FS_Status;

#endif

// Media services declarations:

/// @brief Registers a media type.
/// @param mediaType Media type.
/// @param driver  Media driver (if required).
/// @param driverInfo Media driver additional information (if required).
void fs_register_type(FS_MediaType mediaType, FS_MediaDriver driver, FS_MediaDriverInfo driverInfo);

/// @brief Formats the media.
/// @param mediaType Media type.
/// @param format File system type.
/// @param label Volume label.
/// @return True if performed successfully, false otherwise.
bool fs_format(FS_MediaType mediaType, FS_MediaFormat format, const char* label);

/// @brief Mounts a media to a specified file system root path.
/// @param media Media structure reference to mount.
/// @param root File system root path pointer.
/// @return True if the media was successfully mounted, false otherwise.
bool fs_mount(FS_Media* media, const char* root);

/// @brief Unmouts a media from the specified system root path.
/// @param root File system root path pointer.
/// @return True if the media was successfully unmounted, false otherwise.
bool fs_umount(const char* root);

/// @brief Unmounts a media from a specified media structure location.
/// @param media Media structure pointer.
/// @return Status.
bool fs_umount(FS_Media* media);
