/**
 * @file        TaskScheduler.cpp
 * @author      Adam Łyskawa
 *
 * @brief       A task scheduler that provides scheduling immediate and delayed tasks to the main thread. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "TaskScheduler.hpp"

OS::TaskId OS::TaskScheduler::schedule(void *arg, OptionalBindingAction action, ThreadContext context, TickCount time, TickCount reset)
{
    TaskId id = 0;
    for (auto& task : m_tasks)
    {
        task.lock();
        if (task.acquireUnsafe())
        {
            if (time) ++m_delayed; else ++m_immediate;
            id = task.scheduleUnsafe(arg, action, context, time, reset);
            task.unlock();
            if (time)
                m_delaySemaphore.release();
            else
                m_dispatchSemaphore.release();
            return id;
        }
        task.unlock();
    }
    Crash::here(); // Crash if task pool depleted.
    return id; // Silence compiler warning.
}
