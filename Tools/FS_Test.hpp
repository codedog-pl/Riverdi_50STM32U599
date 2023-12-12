#pragma once

#include "FS_API.hpp"
#include "Log.hpp"
#include "StaticClass.hpp"
#include <cstring>
#include <cstdio>

namespace FS
{

/// @brief FS API test.
class Test final
{

public:

    static constexpr size_t bufferSize = 128;
    static constexpr size_t slack = 10;

    /// @brief Tests the file API.
    /// @param root File system root path.
    /// @param fileName Test file name.
    /// @param bufferSize Buffer size in bytes.
    /// @return True if passed, false if failed.
    static bool fileAPI(const char* root, const char* fileName)
    {
        if (!root || !fileName || bufferSize < slack + 1)
        {
            Log::msg(LogMessage::error, "Invalid parameters!");
            return false;
        }
        Log::msg("Testing FS file API, file = %s%s:", root, fileName);
        auto fs = FileSystemTable::find(root);
        if (!fs)
        {
            Log::msg(LogMessage::error, "File system not found!");
            return false;
        }
        char buffer[bufferSize];
        {
            Log::msg("Creating file...");
            File file(fs, fileName, FileMode::write | FileMode::createAlways);
            if (!file.isOpen)
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
        }
        {
            Log::msg("Opening file...");
            File file(fs, fileName, FileMode::read);
            if (!file.isOpen)
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
        }
        {
            Log::msg("Prefixing the file...");
            Path prefixed(fs, "_%s", fileName);
            if (!prefixed.isValid())
            {
                Log::msg(LogMessage::error, "Prefixed path considered invalid!");
                return false;
            }
            if (!fileRename(fs, fileName, prefixed.relativePath))
            {
                Log::msg(LogMessage::error, "Rename failed!");
                return false;
            }
            Log::msg("Deleting the file...");
            if (!fileDelete(fs, prefixed.relativePath))
            {
                Log::msg(LogMessage::error, "Delete failed!");
                return false;
            }
        }
        Log::msg("SUCCESS!");
        return true;
    }

private:

    static void bufferClear(char* buffer)
    {
        memset(buffer, 0, bufferSize);
    }

    static void bufferFill(char* buffer)
    {
        for (size_t i = 0; i < bufferSize - slack; ++i) buffer[i] = offsetValue(i);
    }

    static bool bufferTest(char* buffer)
    {
        for (size_t i = 0; i < bufferSize - slack; ++i) if (buffer[i] != offsetValue(i)) return false;
        return true;
    }

    static char offsetValue(size_t offset)
    {
        return (offset & 0xffu) ^ 0xAA;
    }

};

}
