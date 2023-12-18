/**
 * @file        Crash.hpp
 * @author      CodeDog
 *
 * @brief       Crashes the application for debugging on unrecoverable error.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "StaticClass.hpp"

/// @brief Contains methods to crash application for debugging on unrecoverable error.
class Crash
{

    STATIC(Crash)

public:

    /// @brief Halts the application for debugging with a spin wait loop.
    static void here()
    {
        while (1);
    }

    /// @brief Halts the application for debugging with a spin wait loop with a message left.
    /// @param msg A message for the debugger.
    static void withMessage(const char* msg)
    {
        message = (char*)msg;
        while (1);
    }

    /// @brief Optional message for the debugger.
    inline static char* message = nullptr;

};
