/**
 * @file        Test.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Tests the file system module. Header only.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "API.hpp"
#include "Log.hpp"
#include "StaticClass.hpp"
#include <cstring>
#include <cstdio>

namespace FS
{

/// @brief FS API test.
class Test final
{

STATIC(Test)

public:

    static constexpr size_t bufferSize = 16384; // Test buffer size.
    static constexpr size_t slack = 10; // Make the actual file size this amount of byte smaller than the buffer size.

    /// @brief Tests the file API.
    /// @param fs File system pointer.
    /// @param fileName Test file name.
    /// @returns True if passed, false if failed.
    static bool fileAPI(const FileSystem* fs, const char* fileName)
    {
        if (!fs || !fileName || bufferSize < slack + 1)
        {
            Log::msg(LogMessage::error, "Invalid parameters!");
            return false;
        }
        Log::msg("Testing FS file API, file = %s%s:", fs->root(), fileName);
        char buffer[bufferSize];
        { // Those braces delimit the life span of the file object...
            Log::msg("Creating file...");
            File file(fs, fileName, FileMode::write | FileMode::createAlways);
            if (!file)
            {
                Log::msg(LogMessage::error, "Create failed!");
                return false;
            }
            bufferClear(buffer);
            bufferFill(buffer);
            Log::msg("Writing...");
            if (!file.write(buffer, bufferSize - slack))
            {
                Log::msg(LogMessage::error, "Write failed!");
                return false;
            }
        } // ...and it is closed here, or wherever the braced block is left.
        { // The file for the write operation is closed to free the stack memory.
            Log::msg("Opening file...");
            File file(fs, fileName, FileMode::read);
            if (!file)
            {
                Log::msg(LogMessage::error, "Open failed!");
                return false;
            }
            bufferClear(buffer);
            Log::msg("Reading...");
            auto readResult = file.read(buffer, bufferSize);
            if (!readResult.has_value())
            {
                Log::msg(LogMessage::error, "Read failed!");
                return false;
            }
            if (readResult.value() != bufferSize - slack)
            {
                Log::msg(LogMessage::error, "Invalid file size: %u bytes!", readResult.value());
                return false;
            }
            if (!bufferTest(buffer))
            {
                Log::msg(LogMessage::error, "Invalid file data!", readResult.value());
                return false;
            }
        } // And now it can and should be closed before we modify its entry.
        { // We prefix the created file with a dot, to make it hidden for Linux based systems.
            Log::msg("Prefixing the file...");
            Path prefixed(fs, ".%s", fileName);
            if (fileExists(fs, prefixed.relativePath())) // But if the file with the new name exists, it would fail...
            {
                Log::msg("Prefixed file exists, deleting prefixed...");
                if (!fileDelete(fs, prefixed.relativePath())) // So we delete it first to make sure the rename operation will succeed.
                {
                    Log::msg("Delete prefixed failed!");
                    return false; // Of course it should not happen.
                }
            }
            if (!prefixed.isValid())
            {
                Log::msg(LogMessage::error, "Prefixed path considered invalid!");
                return false; // If it failed here, the `Path` module would be useless ;)
            }
            if (!fileRename(fs, fileName, prefixed.relativePath()))
            {
                Log::msg(LogMessage::error, "Rename failed!");
                return false; // This basically should not happen if previous operations completed successfully.
            }
            Log::msg("Deleting the file...");
            if (!fileDelete(fs, prefixed.relativePath()))
            {
                Log::msg(LogMessage::error, "Delete failed!");
                return false;
            }
        }
        Log::msg("SUCCESS!");
        return true;
    }

private:

    /// @brief Fills the buffer with zeroes.
    /// @param buffer The buffer pointer.
    static void bufferClear(char* buffer)
    {
        memset(buffer, 0, bufferSize);
    }

    /// @brief Fills the buffer with known non-zero values but leaves some bytes unset.
    /// @param buffer The buffer pointer.
    static void bufferFill(char* buffer)
    {
        for (size_t i = 0; i < bufferSize - slack; ++i) buffer[i] = offsetValue(i);
    }

    /// @brief Tests if the buffer is filled with known non-zero values up to the limit of the file length.
    /// @param buffer The buffer pointer.
    /// @returns The buffer contains the same values that were set with `bufferFill` method.
    static bool bufferTest(char* buffer)
    {
        for (size_t i = 0; i < bufferSize - slack; ++i) if (buffer[i] != offsetValue(i)) return false;
        return true;
    }

    /// @brief Returns the byte value that should be placed at specified offset, effectively provide the "known values".
    /// @param offset An offset location from the begining of the file.
    /// @returns The byte value that is expected to be at that offset.
    static char offsetValue(size_t offset)
    {
        return (offset & 0xffu) ^ 0xAA; // We flip every other bit of subsequent values to make them a little less boring.
    }

};

}
