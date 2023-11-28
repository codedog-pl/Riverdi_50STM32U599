/**
 * @file        Delay.hpp
 * @author      CodeDog
 * @brief       Delays execution of actions using OS thread ticks. Header file.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "Action.hpp"
#include "WaitHandle.hpp"
#include <cstddef>

/**
 * Delays execution of actions using OS thread ticks.
 * The resolution of the delays is limited to the tick calls frequency.
 * Calling `set` from a thread other than the thread the `tick` method is called can introduce +/- 1 tick inaccuracy.
 */
class Delay final
{
public:

    static constexpr int poolSize = 128; // A maximal number of the active wait handles.

    /**
     * @brief Process the ticks for the wait handle pool.
     */
    static void tick();

    /**
     * @brief Delays the action for specified number of OS ticks.
     *
     * @param ticks Delay time in OS ticks, 1ms default.
     * @param action Action to call when the time elapses.
     * @returns Wait handle pointer.
     */
    static WaitHandle* set(uint32_t ticks, Action action);

    /**
     * @brief Repeats the action every specified number of OS ticks.
     *
     * @param ticks Delay time in OS ticks, 1ms default.
     * @param action Action to call when the time elapses.
     * @returns Wait handle pointer.
     */
    static WaitHandle* repeat(uint32_t ticks, Action action);

    /**
     * @brief Delays the action for specified number of OS ticks.
     *
     * @param ticks Delay time in OS ticks, 1ms default.
     * @param action Action to call when the time elapses.
     * @param binding An optional binding pointer.
     * @returns Wait handle pointer.
     */
    static WaitHandle* set(uint32_t ticks, BindingAction action, void* binding);

    /**
     * @brief Repeats the action every specified number of OS ticks.
     *
     * @param ticks Delay time in OS ticks, 1ms default.
     * @param action Action to call when the time elapses.
     * @param binding An optional binding pointer.
     * @returns Wait handle pointer.
     */
    static WaitHandle* repeat(uint32_t ticks, BindingAction action, void* binding);

    /**
     * @brief If the pointer points to a wait handle pointer, it cancels the wait handle and clears the target pointer.
     *
     * @param waitHandlePtr A pointer to a wait handle pointer.
     */
    static void clear(WaitHandle** waitHandlePtr);

protected:

    /// @returns A first free (inactive) wait handle or `nullptr` if wait handles exhausted.
    static WaitHandle* getWaitHandle();

    /// @brief Updates the index of the last active wait handle.
    static void lastActiveUpdate();

    static inline WaitHandle m_pool[poolSize];  // A pool of available wait handles to use.
    static inline int lastActive = -1;          // Last active wait handle index.

};
