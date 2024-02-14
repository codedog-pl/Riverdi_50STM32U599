/**
 * @file        EventGroup.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An object that allows to signal events across the RTOS threads. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "RTOS.hpp"

namespace OS
{

/// @brief An object that allows to signal events across the RTOS threads.
class EventGroup
{

public:

    /// @brief Creates an event group with lazy initialization.
    EventGroup();

    /// @brief Additional cleanup.
    ~EventGroup();

    /// @brief Sets the specified bits for this event group.
    /// @param bits Bits to set.
    /// @returns True if the bits was set successfully.
    bool signal(EventFlags bits);

    /// @brief Blocks the current thread until the requested bits are set.
    /// @param bits Bits to wait.
    /// @param options Wait options.
    /// @param timeout Time limit. Default: `waitForever`.
    /// @returns Bits that was actually set.
    EventFlags wait(EventFlags bits, WaitOptions options = waitAny, TickCount timeout = waitForever);

private:

    void init(void);

#if defined(USE_AZURE_RTOS)
    TX_EVENT_FLAGS_GROUP m_controlBlock;
    bool m_isCreated;
#elif defined(USE_FREE_RTOS)
    StaticEventGroup_t m_buffer;    // A statically allocated buffer for the data.
    EventGroupHandle_t m_handle;    // A pointer used to access the data.
#endif

};

}