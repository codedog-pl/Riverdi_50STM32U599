/**
 * @file        ConfigParser.cpp
 * @author      CodeDog
 * @brief       Simple configuration file parser.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "ConfigParser.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>

void ConfigParser::parse(Setter setter, const char* keys)
{
    if (!setter || !keys) return;
    m_setter = setter;
    m_keys = keys;
    m_keysLength = strlen(m_keys);
    if (m_keysLength) parse();
}

void ConfigParser::parse()
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

void ConfigParser::processLine(const char* key, const char* value)
{
    if (!m_setter || !key || !value) return;
    int keyIndex = matchKey(key);
    if (keyIndex >= 0) m_setter(keyIndex, atoi(value));
}

int ConfigParser::matchKey(const char* key)
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
