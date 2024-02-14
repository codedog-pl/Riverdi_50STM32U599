/**
 * @file        TaskScheduler.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A task scheduler that provides scheduling immediate and delayed tasks to the main thread. Header file.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "Crash.hpp"
#include "Task.hpp"
#include "Thread.hpp"
#include "Semaphore.hpp"
#include <tuple>

namespace OS
{

/// @brief A pool of scheduled action calls.
class TaskScheduler final
{

public:

    /// @brief Schedules a new action call. Thread safe.
    /// @param arg Binding argument. If set, the action will be calleded with it.
    /// @param action Action to call when task `run` method is called.
    /// @param context Target thread context. Default: `application`.
    /// @param time The number of RTOS ticks to wait before the task can be run. Default: 0.
    /// @param reset The number of RTOS ticks to reset the `time` value to when it elapses. Default: 0.
    /// @returns Task identifier.
    TaskId schedule(void* arg, OptionalBindingAction action, ThreadContext context = application,
                    TickCount time = 0, TickCount reset = 0);

    /// @brief Starts the task scheduler, immediatelly calls immediate tasks, starts waiting for delayed tasks if any.
    void start(void)
    {
        m_delayThread.start(this, delayTask, "TaskScheduler::delayTask", ThreadPriority::belowNormal);
        if (!immediateCount()) m_dispatchSemaphore.wait();
        for (;;)
        {
            if (immediateCount()) processImmediate(application);
            m_dispatchSemaphore.wait();
        }
    }

    /// @brief Cancels an active task. Thread safe.
    /// @param id Task identifier reference. Gets zeroed if task canceled.
    inline void cancel(TaskId& id)
    {
        for (auto& task : m_tasks) if (task.cancel(id, &m_immediate, &m_delayed)) return;
    }

    void frameTick(void)
    {
        if (!immediateCount()) return;
        processImmediate(frame);
    }

friend class AppThread;
private:

    TaskScheduler() : m_tasks(), m_immediate(0), m_delayed(0), m_delayThread(), m_delaySemaphore(), m_dispatchSemaphore() { }
    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler(TaskScheduler&&) = delete;

    /// @brief Processes all immediate tasks in the pool. Thread safe.
    /// @param context Target thread context.
    inline void processImmediate(ThreadContext context = application)
    {
        for (auto& task : m_tasks) task.process(context, &m_immediate, &m_delayed);
    }

    // Processes all delayed tasks in the pool.
    // Decreases tick counters.
    // Makes task immediate when the tick counter is zeroed.
    inline void processDelayed(void)
    {
        for (auto& task : m_tasks)
            if (task.delayTick(&m_immediate, &m_delayed))
                m_dispatchSemaphore.release();
    }

    /// @returns The immediate scheduled tasks count.
    inline size_t immediateCount() const { return m_immediate; }

    /// @returns The delayed scheduled tasks count.
    inline size_t delayedCount() const { return m_delayed; }

    /// @brief A loop that decreases delayed tasks tick counters and notifies the application thread when a task is ready to run.
    /// @param arg Scheduler instance as `void*` pointer.
    static inline void delayTask(OS::ThreadArg arg)
    {
        TaskScheduler& instance = *reinterpret_cast<TaskScheduler*>(arg);
        for (;;)
        {
            if (instance.m_delayed) instance.processDelayed();
            if (instance.m_delayed) OS::delay(1); else instance.m_delaySemaphore.wait();
        }
    }

private:

    /// @brief Maximum number of tasks that can be scheduled at the same time.
    static constexpr size_t size = WTK_OS_TASKS;

    /// @brief Internal tasks pool.
    Task m_tasks[size];

    /// @brief Internal number of immediate tasks currently scheduled.
    size_t m_immediate;

    /// @brief Internal number of delayed tasks currently scheduled.
    size_t m_delayed;

    /// @brief Thread responsible for scheduling delayed tasks.
    Thread m_delayThread;

    /// @brief Semaphore used to suspend the delay task.
    Semaphore m_delaySemaphore;

    /// @brief Semaphore used to supend the main task.
    Semaphore m_dispatchSemaphore;

};

}
