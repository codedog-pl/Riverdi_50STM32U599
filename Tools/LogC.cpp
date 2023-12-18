/**
 * @file        LogC.cpp
 * @author      CodeDog
 *
 * @brief       Provides debug log C bindings.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "bindings.h"
#include "Log.hpp"
#include <cstdarg>
#include <cstring>

EXTERN_C_BEGIN

#include "log.h"

void log_level(bool isRelease) { Log::init(isRelease); }

void log_init(UART_HandleTypeDef* huart) { Log::initUART(huart); }

void log_msg(uint8_t severity, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    const size_t lineBufferSize = 255;
    char lineBuffer[lineBufferSize];
    vsnprintf(lineBuffer, lineBufferSize, format, args);
    va_end(args);
    Log::msg((LogMessage::Severity)severity, lineBuffer);
}

void log_start_async(void)
{
    Log::startAsync();
}

EXTERN_C_END
