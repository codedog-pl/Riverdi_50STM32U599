/**
 * @file        Task.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Scheduled task class. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "Task.hpp"

void OS::Task::process(ThreadContext context, size_t* immediateCount, size_t* delayedCount)
{
    m_mutex.acquire();
    auto tcb = m_tcb; // Since we release mutex while the task is being run, we use a snapshot of the task control block.
    m_mutex.release();
    if (!tcb.id || tcb.delayTicks || tcb.context != context) return;
    if (tcb.binding) tcb.action.binding(tcb.binding);
    else tcb.action.plain();
    m_mutex.acquire();
    if (m_tcb.resetTicks)
    {
        m_tcb.delayTicks = m_tcb.resetTicks;
        if (immediateCount && *immediateCount) --*immediateCount; // The task stops being immediate...
        if (delayedCount) ++*delayedCount; // ...and becomes delayed.
    }
    else
    {
        if (immediateCount && *immediateCount) --*immediateCount;
        m_tcb.clear();
    }
    m_mutex.release();
}

bool OS::Task::delayTick(size_t* immediateCount, size_t* delayedCount)
{
    bool matched = false, zeroed = false;
    m_mutex.acquire();
    matched = m_tcb.id != 0 && m_tcb.delayTicks != 0;
    zeroed = matched && --m_tcb.delayTicks == 0;
    m_mutex.release();
    if (zeroed)
    {
        if (immediateCount) ++*immediateCount; // The task becomes immediate...
        if (delayedCount && *delayedCount) --*delayedCount; // ...and stops being delayed.
    }
    return zeroed;
}

bool OS::Task::cancel(TaskId &id, size_t* immediateCount, size_t* delayedCount)
{
    bool matched = false;
    m_mutex.acquire();
    matched = m_tcb.id == id;
    if (matched)
    {
        id = 0;
        if (m_tcb.delayTicks)
        {
            if (delayedCount && *delayedCount) --*delayedCount;
        }
        else
        {
            if (immediateCount && *immediateCount) --*immediateCount;
        }
        m_tcb.clear();
    }
    m_mutex.release();
    return matched;
}
