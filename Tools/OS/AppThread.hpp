/**
 * @file        AppThread.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides main application thread static methods. Header only.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "StaticClass.hpp"
#include "TaskScheduler.hpp"

namespace OS
{

/// @brief Provides main application thread static methods.
class AppThread final
{
    STATIC(AppThread)

public:

    /// @brief Starts the main application thread task scheduler.
    static inline void start() { m_scheduler.start(); }

    /// @brief Calls actions scheduled to the GUI display frame.
    static inline void frame() { m_scheduler.frameTick(); }

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    static inline void sync(Action action, ThreadContext context = application)
    {
        m_scheduler.schedule(nullptr, action, context, 0, 0);
    }

    /// @brief Schedules the action to be executed in the selected thread context.
    /// @param argument Pointer to pass to the action.
    /// @param action Action that passes an argument pointer.
    /// @param context Target thread context.
    static inline void sync(void* argument, BindingAction action, ThreadContext context = application)
    {
        m_scheduler.schedule(argument, action, context, 0, 0);
    }

    /// @brief Schedules the action to be executed when the `time` elapses.
    /// @param time The number of RTOS ticks to wait.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId delay(TickCount time, Action action, ThreadContext context = application)
    {
        return m_scheduler.schedule(nullptr, action, context, time, 0);
    }

    /// @brief Schedules the action to be executed when the `time` elapses.
    /// @param time The number of RTOS ticks to wait.
    /// @param argument Pointer to pass to the action.
    /// @param action Action that passes an argument pointer.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId delay(TickCount time, void* argument, BindingAction action, ThreadContext context = application)
    {
        return m_scheduler.schedule(argument, action, context, time, 0);
    }

    /// @brief Schedules the action to be repeated when the `time` elapses with the regular `time` interval.
    /// @param time The number of RTOS ticks to wait.
    /// @param action Action that passes no argument.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId repeat(TickCount time, Action action, ThreadContext context = application)
    {
        return m_scheduler.schedule(nullptr, action, context, time, time);
    }

    /// @brief Schedules the action to be repeated when the `time` elapses with the regular `time` interval.
    /// @param time The number of RTOS ticks to wait.
    /// @param argument Pointer to pass to the action.
    /// @param action Action that passes an argument pointer.
    /// @param context Target thread context.
    /// @returns Unique task identifier that can be used to cancel a scheduled task.
    static inline TaskId repeat(TickCount time, void* argument, BindingAction action, ThreadContext context = application)
    {
        return m_scheduler.schedule(argument, action, context, time, time);
    }

    /// @brief Cancels an active task. Thread safe.
    /// @param id Task identifier reference. Gets zeroed if task canceled.
    static inline void cancel(TaskId& taskId) { m_scheduler.cancel(taskId); }

private:

    static inline TaskScheduler m_scheduler{};

};

}
