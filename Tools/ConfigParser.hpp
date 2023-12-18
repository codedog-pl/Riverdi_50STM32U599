/**
 * @file        ConfigParser.hpp
 * @author      CodeDog
 *
 * @brief       Simple configuration file parser. Header only.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>

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

    ConfigParser(const ConfigParser&) = delete; // Instances should not be copied.

    ConfigParser(ConfigParser&&) = delete; // Instances should not be moved.

    /// @brief Parses the configuration text by calling the setter on each matched key and value.
    /// @param setter A function accepting key index and an integer value.
    /// @param keys Key list separated with `keyListSeparator` character. Cannot contain whitespace.
    void parse(Setter setter, const char* keys)
    {
        if (!setter || !keys) return;
        m_setter = setter;
        m_keys = keys;
        m_keysLength = strlen(m_keys);
        if (m_keysLength) parse();
    }

private:

    static constexpr char HT = '\t';    // Horizontal TAB character.
    static constexpr char LF = '\n';    // Line feed character.
    static constexpr char CR = '\r';    // Carriage return character.
    static constexpr char SP = ' ';     // Space character.
    static constexpr char EQ = '=';     // Assignment operator, equals character.

    /// @brief Starts parsing the content.
    void parse()
    {
        if (!m_setter || !m_keys || !m_keysLength) return;
        char key[maxTokenLength] = {};
        char value[maxTokenLength] = {};
        uint8_t keyOffset = 0;
        uint8_t valueOffset = 0;
        bool isParsingValue = false;
        char c; // Current character.
        bool isLast; // Current character is the last character of the text.
        for (size_t i = 0; i < m_contentLength; ++i)
        {
            c = reinterpret_cast<char*>(m_content)[i];
            isLast = i == m_contentLength - 1 || !c;
            if (c == SP || c == HT || c == CR) continue; // Ignore whitespace.
            if (c == LF || isLast) // End of line or end of text:
            {
                // If it's the last character in text, it needs to be added to the value string:
                if (isParsingValue && isLast && c) value[valueOffset++] = c;
                if (keyOffset && valueOffset && isParsingValue)
                    processLine(const_cast<char*>(key), const_cast<char*>(value));
                keyOffset = 0;
                valueOffset = 0;
                memset(key, 0, maxTokenLength);
                memset(value, 0, maxTokenLength);
                isParsingValue = false;
                if (c) continue; else return;
            }
            if (!isParsingValue)
            {
                if (c == EQ) isParsingValue = true;
                else if (keyOffset < maxTokenLength - 1) key[keyOffset++] = c;
            }
            else
            {
                if (valueOffset < maxTokenLength - 1) value[valueOffset++] = c;
            }
        }
    }

    /// @brief Process a single parsed configuration line.
    /// @param key Key string.
    /// @param value Value string.
    void processLine(const char* key, const char* value)
    {
        if (!m_setter || !key || !value) return;
        int keyIndex = matchKey(key);
        if (keyIndex >= 0) m_setter(keyIndex, atoi(value));
    }

    /// @brief Matches the given key against the provided key list.
    /// @param key A key to match.
    /// @returns A zero based index of the key in the provided key list or -1 if the key is not found on the list.
    int matchKey(const char* key)
    {
        if (!m_keys || !m_keysLength || !key) return -1;
        int itemIndex = 0;
        int itemOffset = 0;
        int keyLength = strlen(key);
        char c; // Current character.
        bool isLast; // Current character is the last character of the text.
        for (int i = 0; i < m_keysLength; i++)
        {
            c = m_keys[i];
            isLast = i == m_keysLength - 1;
            if (c == keyListSeparator || isLast)
            {
                if (itemOffset + keyLength <= m_keysLength &&
                    (isLast || m_keys[itemOffset + keyLength] == keyListSeparator) &&
                    memcmp(&m_keys[itemOffset], key, keyLength) == 0) return itemIndex;
                itemOffset = i + 1;
                ++itemIndex;
            }
        }
        return -1;
    }

    void* m_content{};          // Content buffer address.
    size_t m_contentLength = 0; // Content buffer length.
    const char* m_keys{};       // Recognized keys list.
    int m_keysLength = 0;       // The precalculated length of the key list.
    Setter m_setter{};          // A function to be called when a key list match is found.

};
