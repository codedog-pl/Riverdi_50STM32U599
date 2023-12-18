/**
 * @file        FS_AdapterTypes.hpp
 * @author      CodeDog
 *
 * @brief       Defines the types used by the file system adapter.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <optional>
#include "fs_bindings.h"
#include "BitFlags.hpp"

namespace FS
{

/// @brief Flags for the file open operation.
/// @remark Values made to match FATFS flags directly.
enum class FileMode : uint32_t
{
    none            = 0x00, // No access, fail if the file doesn't exist.
    read            = 0x01, // Read access.
    write           = 0x02, // Write access.
    createNew       = 0x04, // Fail if the file exits.
    createAlways    = 0x08, // Overwrite if the file exists.
    openAlways      = 0x10, // Create if the file doesn't exist.
    openAppend      = 0x30  // Set read/write pointer to the end of the file.
};

using ReadResult = std::optional<size_t>;

/// @brief A placeholder structure for a NULL file system.
using Placeholder = FS_Placeholder;

/// @brief File system adapter class type definitions (FILEX).
class AdapterTypes
{

public:

    using Media = FS_Media;                     // Media structure type.
    using DirectoryEntry = FS_DirectoryEntry;   // Directory entry structure type.
    using FileHandle = FS_FileHandle;           // File handle structure type.
    using FileOffset = FS_FileOffset;           // File offset number type.
    using Status = FS_Status;                   // I/O operation status type.

#if defined(USE_FILEX)

    static constexpr size_t lfnMaxLength = FX_MAX_LONG_NAME_LEN;    // Maximum length of the path string.
    static constexpr Status OK = FX_SUCCESS;                        // Successful operation status.
    static constexpr FileOffset offsetMax = -1UL;                   // Last possible file offset.

#else

    static constexpr size_t lfnMaxLength = 256;     // Maximum length of the path string.
    static constexpr Status OK = 0;                 // Successful operation status.
    static constexpr FileOffset offsetMax = -1UL;   // Last possible file offset.

#endif

};

}
