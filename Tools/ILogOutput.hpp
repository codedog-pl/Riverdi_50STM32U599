/**
 * @file        ILogOutput.hpp
 * @author      CodeDog
 *
 * @brief       System logger interface for message output.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

/// @brief Defines a debug output interface.
/// @remark Implementation should define how to physically send the debug text output.
class ILogOutput
{

public:

    /// @returns true if the output is currently available.
    virtual bool isAvailable(void) { return true; }

    /// @brief Starts asynchronous operation as soon as the RTOS is started.
    /// @remarks If not defined in derived class, it does nothing.
    virtual void startAsync(void) { }

    /// @brief Sends a message to the output.
    /// @param index Message index.
    virtual void send(int index) = 0;

};