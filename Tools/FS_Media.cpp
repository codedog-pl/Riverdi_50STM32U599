/**
 * @file        FS_Media.cpp
 * @author      CodeDog
 *
 * @brief       Defines C bindings for the file system media services.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#include "FS_Media.hpp"
#include "fs_bindings.h"

EXTERN_C_BEGIN

void fs_register_type(FS_MediaType mediaType, FS_MediaDriver driver, FS_MediaDriverInfo driverInfo)
{
    FS::MediaServices::registerType(static_cast<FS::MediaType>(mediaType), driver, driverInfo);
}

int fs_format(FS_MediaType mediaType, FS_MediaFormat format, const char* label)
{
    return FS::MediaServices::format(static_cast<FS::MediaType>(mediaType), static_cast<FS::MediaFormat>(format), label);
}

int fs_mount(FS_Media* media, const char* root)
{
    return FS::MediaServices::mount(*media, root);
}

int fs_umount(const char* root)
{
    return FS::MediaServices::umount(root);
}

int fs_umount_media(FS_Media* media)
{
    return FS::MediaServices::umount(*media);
}

EXTERN_C_END
