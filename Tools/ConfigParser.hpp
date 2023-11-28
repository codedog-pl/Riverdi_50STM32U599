/**
 * @file        ConfigParser.hpp
 * @author      CodeDog
 * @brief       Simple configuration file parser. Header file.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include <functional>

/**
 * A simple configuration file parser.
 *
 * Parses text file containing lines like "aKey = 123" against known key names.
 * When a key is matched a function is called with the key index and an integer value.
 *
 * Invalid data will be quietly ignored.
 */
class ConfigParser
{

public:

    static constexpr size_t maxTokenLength = 32;                    // Maximal token (key or value) length.
    static constexpr char keyListSeparator = '|';                   // Key list separator character.
    static constexpr int maxLineLength = 2 * maxTokenLength + 5;    // Maximal length of the expression line.

    /// @brief Calculates a buffer size for the `n` lines.
    /// @param n Maximal number of lines.
    /// @return Size in bytes aligned to 32 bits.
    static constexpr int bufferSizeForNLines(int n) { return (((n * maxLineLength - 1) >> 2) << 2) + 4; }

    /// @brief A setter function that will receive parsed values.
    using Setter = std::function<void(int, int)>;

    /// @brief Creates a parser over a C string buffer.
    /// @param content Content address.
    /// @param length Content length.
    ConfigParser(void* content, size_t length) : m_content(content), m_contentLength(length) { }

    ConfigParser(const ConfigParser&) = delete;
    ConfigParser(ConfigParser&&) = delete;

    /// @brief Parses the configuration text by calling the setter on each matched key and value.
    /// @param setter A function accepting key index and an integer value.
    /// @param keys Key list separated with `keyListSeparator` character. Cannot contain whitespace.
    void parse(Setter setter, const char* keys);

private:

    static constexpr char HT = '\t';    // Horizontal TAB character.
    static constexpr char LF = '\n';    // Line feed character.
    static constexpr char CR = '\r';    // Carriage return character.
    static constexpr char SP = ' ';     // Space character.
    static constexpr char EQ = '=';     // Assignment operator, equals character.

    /// @brief Starts parsing the content.
    void parse();

    /// @brief Process a single parsed configuration line.
    /// @param key Key string.
    /// @param value Value string.
    void processLine(const char* key, const char* value);

    /// @brief Matches the given key against the provided key list.
    /// @param key A key to match.
    /// @returns A zero based index of the key in the provided key list or -1 if the key is not found on the list.
    int matchKey(const char* key);

    void* m_content{};          // Content buffer address.
    size_t m_contentLength = 0; // Content buffer length.
    const char* m_keys{};       // Recognized keys list.
    int m_keysLength = 0;       // The precalculated length of the key list.
    Setter m_setter{};          // A function to be called when a key list match is found.

};
