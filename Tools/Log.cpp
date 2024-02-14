/**
 * @file        Log.cpp
 * @author      Adam Łyskawa
 *
 * @brief       System debug information logger. Implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "Log.hpp"
#include "LogITM.hpp"
#include "LogUART.hpp"

void Log::init(bool isRelase)
{
    level(isRelase ? LogMessage::info : LogMessage::detail);
    m_output = LogITM::getInstance(m_pool);
}

void Log::initUART(UART_HandleTypeDef *huart)
{
    m_output = LogUART::getInstance(huart, m_pool);
}

void Log::startAsync(void)
{
    if (m_output) m_output->startAsync();
}

void Log::printf(const char *format, ...)
{
    if (m_output && !m_output->isAvailable()) return;
    auto [message, offset] = getMessage(); if (!message) return;
    va_list args;
    va_start(args, format);
    message->vprintf(format, args);
    va_end(args);
    if (m_output && !offset) m_output->send(offset);
}

void Log::tsprintf(const char *format, ...)
{
    if (m_output && !m_output->isAvailable()) return;
    auto [message, offset] = getMessage(); if (!message) return;
    va_list args;
    va_start(args, format);
    message->addTimestamp()->add(' ')->vprintf(format, args);
    va_end(args);
    if (m_output && !offset) m_output->send(offset);
}

void Log::dump(const char *format, ...)
{
    if (m_output && !m_output->isAvailable()) return;
    if (m_level < LogMessage::detail) return;
    auto [message, offset] = getMessage(LogMessage::detail); if (!message) return;
    if (m_dumpIndentation) message->add(' ', m_dumpIndentation);
    va_list args;
    va_start(args, format);
    message->vprintf(format, args)->add("\r\n");
    va_end(args);
    if (m_output && !offset) m_output->send(offset);
}

void Log::msg(const char *format, ...)
{
    if (m_output && !m_output->isAvailable()) return;
    auto [message, offset] = getMessage(); if (!message) return;
    va_list args;
    va_start(args, format);
    message->addTimestamp()->add(' ')->vprintf(format, args)->add("\r\n");
    va_end(args);
    if (m_output && !offset) m_output->send(offset);
}

void Log::msg(LogMessage::Severity severity, const char *format, ...)
{
    if (m_output && !m_output->isAvailable()) return;
    auto [message, offset] = getMessage(severity); if (!message) return;
    message->addTimestamp()->add(' ');
    switch (severity)
    {
    case LogMessage::error:
        message->add("ERROR: ");
        break;
    case LogMessage::warning:
        message->add("WARNING: ");
        break;
    case LogMessage::info:
        message->add("INFO: ");
        break;
    default:
        break;
    }
    va_list args;
    va_start(args, format);
    message->vprintf(format, args)->add("\r\n");
    va_end(args);
    if (m_output && !offset) m_output->send(offset);
}
