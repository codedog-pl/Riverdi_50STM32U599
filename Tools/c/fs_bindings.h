/**
 * @file        fs_bindings.h
 * @author      Adam Łyskawa
 *
 * @brief       File System API C bindings.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <stddef.h>
#include "bindings.h"
#include "target.h"

// Common types:

/// @brief A placeholder structure for a NULL file system.
typedef struct __FS_Placeholder
{
    int isUsed; // 1: The structure is in use. 0: The structure is reset.
} FS_Placeholder;

/// @brief Physical media type.
typedef enum
{
    FS_MEDIA_NONE, FS_MEDIA_eMMC, FS_MEDIA_SD, FS_MEDIA_USB
} FS_MediaType;

/// @brief Media format enumeration.
typedef enum
{
    FS_FORMAT_DEFAULT, FS_FORMAT_ExFAT, FS_FORMAT_FAT12, FS_FORMAT_FAT16, FS_FORMAT_FAT32
} FS_MediaFormat;

typedef char FS_MediaRoot[4]; // Media root path.

// File system middleware backend specific:

#if defined(USE_FILEX)

// FILEX types:

#include "fx_api.h"

typedef VOID (*FS_MediaDriver)(FX_MEDIA*);
typedef VOID*           FS_MediaDriverInfo;
typedef FX_MEDIA        FS_Media;
typedef FX_DIR_ENTRY    FS_DirectoryEntry;
typedef FX_FILE         FS_FileControlBlock;
typedef ULONG           FS_FileOffset;
typedef UINT            FS_Status;

#elif defined(USE_FATFS)

#include "fatfs.h"

typedef const Diskio_drvTypeDef*            FS_MediaDriver;
typedef FATFS                               FS_Media;

typedef struct { DIR dir; FILINFO info; }   FS_DirectoryEntry;
typedef FIL                                 FS_FileControlBlock;

typedef FSIZE_t                             FS_FileOffset;
typedef FRESULT                             FS_Status;

#else

// `NullAdapter` types:

typedef void*           FS_MediaDriver;
typedef void*           FS_MediaDriverInfo;
typedef FS_Placeholder  FS_Media;
typedef FS_Placeholder  FS_DirectoryEntry;
typedef FS_Placeholder  FS_FileControlBlock;
typedef size_t          FS_FileOffset;
typedef int             FS_Status;

#endif

// Media services declarations:

EXTERN_C_BEGIN

/// @brief Registers a media type.
/// @param mediaType Media type.
/// @param root Root path. For FATFS compatibility it needs to be formatted like "0:/".
/// @param driver  Media driver (if required).
/// @param driverInfo Media driver additional information (if required).
void fs_register_type(FS_MediaType mediaType, const char* root, FS_MediaDriver driver);

/// @brief Formats the media.
/// @param root Root path.
/// @param format File system type.
/// @param label Volume label.
/// @returns True if performed successfully, false otherwise.
int fs_format(const char* root, FS_MediaFormat format, const char* label);

/// @brief Mounts a media to a specified file system root path.
/// @param media Media structure reference to mount.
/// @param root File system root path pointer.
/// @returns 1 if the media was successfully mounted, 0 otherwise.
int fs_mount(FS_Media* media, const char* root);

/// @brief Unmouts a media from the specified system root path.
/// @param root File system root path pointer.
/// @returns 1 if the media was successfully unmounted, 0 otherwise.
int fs_umount(const char* root);

/// @brief Unmounts a media from a specified media structure location.
/// @param media Media structure pointer.
/// @returns 1 if the media was successfully unmounted, 0 otherwise.
int fs_umount_media(FS_Media* media);

EXTERN_C_END
