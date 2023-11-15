/**
 * @file        HMI.hpp
 * @author      CodeDog
 * @brief       -
 * @remarks     -
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

extern "C" {
    #include "HMI_C.h"
}

class HMI final
{
public:
    HMI() = delete;
    HMI(const HMI& other) = delete;
    HMI(HMI&& other) = delete;

    /// @brief Starts the main application thread.
    static void start();

};

