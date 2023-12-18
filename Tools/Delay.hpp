/**
 * @file        Delay.hpp
 * @author      CodeDog
 *
 * @brief       A class that provides forking the application thread using waiting threads.
 *
 * @remarks     In order to use the `Thread::dispatchLoop()` method must be started in the main application thread!
 *              Do not overuse because the methods will crash if wait handle or thread pools get depleted.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <cstddef>
#include "target.h"
#include "Action.hpp"
#include "Crash.hpp"
#include "WaitHandle.hpp"

/// @brief Delays execution of actions in application thread
///        by forking a waiting thread that waits and executes the action in the application thread.
///        It requires the Thread::dispatchLoop() to be running!
class Delay final
{
public:

    static constexpr int poolSize = OS_THREADS / 2; // A maximal number of the active wait handles.

    /// @brief Starts the action after specified number of OS ticks. The action will be started in the application thread context.
    /// @remarks This call will crash if there is not enough of free wait handles or threads.
    /// @param ticks The number of OS ticks to wait.
    /// @param action Action to call when the time elapses.
    /// @return Wait handle pointer.
    static WaitHandle* set(uint32_t ticks, Action action)
    {
        WaitHandle* waitHandle = getWaitHandle();
        waitHandle->action.plain = action;
        waitHandle->binding = nullptr;
        waitHandle->threadId = 0;
        waitHandle->ticks = ticks;
        waitHandle->reset = 0;
        lastActiveUpdate();
        waitHandle->waitAsync();
        return (waitHandle->threadId) ? waitHandle : nullptr;
    }

    /// @brief Starts the action after specified number of OS ticks. The action will be started in the application thread context.
    /// @remarks This method will crash if there is not enough of free wait handles or threads.
    /// @param ticks The number of OS ticks to wait.
    /// @param action Action to call when the time elapses.
    /// @param binding An optional binding pointer passed to the action.
    /// @return Wait handle pointer.
    static WaitHandle* set(uint32_t ticks, BindingAction action, void* binding)
    {
        WaitHandle* waitHandle = getWaitHandle();
        waitHandle->action.binding = action;
        waitHandle->binding = binding;
        waitHandle->threadId = 0;
        waitHandle->ticks = ticks;
        waitHandle->reset = 0;
        lastActiveUpdate();
        waitHandle->waitAsync();
        return (waitHandle->threadId) ? waitHandle : nullptr;
    }

    /// @brief Repeats the the action every specified number of OS ticks. The action will be started in the application thread context.
    /// @remarks This method will crash if there is not enough of free wait handles or threads.
    /// @param ticks The number of OS ticks to wait.
    /// @param action Action to call when the time elapses.
    /// @return Wait handle pointer.
    static WaitHandle* repeat(uint32_t ticks, Action action)
    {
        WaitHandle* waitHandle = getWaitHandle();
        waitHandle->action.plain = action;
        waitHandle->binding = nullptr;
        waitHandle->threadId = 0;
        waitHandle->ticks = ticks;
        waitHandle->reset = ticks;
        lastActiveUpdate();
        waitHandle->waitAsync();
        return (waitHandle->threadId) ? waitHandle : nullptr;
    }

    /// @brief Repeats the the action every specified number of OS ticks. The action will be started in the application thread context.
    /// @remarks This method will crash if there is not enough of free wait handles or threads.
    /// @param ticks The number of OS ticks to wait.
    /// @param action Action to call when the time elapses.
    /// @param binding An optional binding pointer passed to the action.
    /// @return Wait handle pointer.
    static WaitHandle* repeat(uint32_t ticks, BindingAction action, void* binding)
    {
        WaitHandle* waitHandle = getWaitHandle();
        waitHandle->action.binding = action;
        waitHandle->binding = binding;
        waitHandle->threadId = 0;
        waitHandle->ticks = ticks;
        waitHandle->reset = ticks;
        lastActiveUpdate();
        waitHandle->waitAsync();
        return (waitHandle->threadId) ? waitHandle : nullptr;
    }

    /// @brief If the pointer points to a wait handle pointer, it cancels the wait handle and clears the target pointer.
    /// @param waitHandlePtr A pointer to a wait handle pointer.
    static void clear(WaitHandle** waitHandlePtr)
    {
        if (!waitHandlePtr || !*waitHandlePtr) return;
        (*waitHandlePtr)->cancel();
        lastActiveUpdate();
        *waitHandlePtr = nullptr;
    }

protected:

    /// @returns An empty wait handle from the pool.
    ///          Will crash if the pool is exhausted.
    static WaitHandle* getWaitHandle()
    {
        for (int i = 0; i < poolSize; i++)
        {
            WaitHandle* event = &m_pool[i];
            if (!event->threadId && !event->action.plain) return event;
        }
        Crash::here(); // Pool exhausted. This will never happen, because we'll run out of threads first.
        return nullptr;
    }

    /// @brief Updates the index of the last active wait handle.
    static void lastActiveUpdate()
    {
        for (int i = poolSize - 1; i >= 0; --i)
        if (m_pool[i].action.plain) { lastActive = i; return; }
        lastActive = -1;
    }

    static inline WaitHandle m_pool[poolSize];  // A pool of available wait handles to use.
    static inline int lastActive = -1;          // Last active wait handle index.

};
