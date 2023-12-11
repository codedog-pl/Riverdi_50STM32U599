/**
 * @file        Thread.cpp
 * @author      CodeDog
 * @brief       Quick thread synchronization tool.
 * @remarks     In order to use:
 *                  - call tick() from the frame display thread,
 *                  - call dispatchLoop() at the end of the main application thread.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "hal_mcu.h"
#include "Log.hpp"
#include "Thread.hpp"

/**
 * @remarks When called again with the same action before the last one completed - it's ignored.
 *          Also does nothing if the action pool is full (scheduled actions limit exceeded).
 */
void Thread::sync(Action action, Context context)
{
    if (!action || !context) return;
    if ((context == application && !dispatcherStarted) || (context == frame && !displayStarted))
    {
        action();
        return;
    }
    for (uint8_t i = 0; i < max; i++)
    {
        if (scheduled[i].action.plain == action) return;
        if (!scheduled[i].action.plain)
        {
            scheduled[i].action = action;
            scheduled[i].binding = nullptr;
            scheduled[i].context = context;
            ++q;
            if (context == application && dispatchSemaphore) OS::semaphoreRelease(dispatchSemaphore);
            return;
        }
    }
}

void Thread::sync(BindingAction action, void *argument, Context context)
{
    if (!action || !context) return;
    if ((context == application && !dispatcherStarted) || (context == frame && !displayStarted))
    {
        action(argument);
        return;
    }
    for (uint8_t i = 0; i < max; i++)
    {
        if (scheduled[i].action.binding == action) return;
        if (!scheduled[i].action.binding)
        {
            scheduled[i].action = action;
            scheduled[i].binding = argument;
            scheduled[i].context = context;
            ++q;
            if (context == application && dispatchSemaphore) OS::semaphoreRelease(dispatchSemaphore);
            return;
        }
    }
}

void Thread::syncISR(Action action, Context context)
{
    if (((context == application && dispatcherStarted) || (context == frame && displayStarted)) && fromISR())
        sync(action, context); else action();
}

void Thread::syncISR(BindingAction action, void *argument, Context context)
{
    if (((context == application && dispatcherStarted) || (context == frame && displayStarted)) && fromISR())
        sync(action, argument, context); else action(argument);
}

bool Thread::fromISR()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

void Thread::warnISR()
{
    if (fromISR()) Log::msg(LogMessage::error, "Invalid call from ISR, system compromised!");
}

void Thread::dispatchLoop()
{
    if (!dispatchSemaphore) dispatchSemaphore = OS::semaphoreCreate("Thread::dispatchSemaphore");
    dispatcherStarted = true;
    while (1)
    {
        if (q)
        {
            for (uint8_t i = 0; i < max; i++)
            {
                ScheduledTask task = scheduled[i];
                if (!task.action.plain || task.context != application) continue;
                if (q) --q;
                if (task.binding) task.action.binding(task.binding);
                else task.action.plain();
                scheduled[i].action.plain = nullptr;
                scheduled[i].binding = nullptr;
                scheduled[i].context = none;
            }
        }
        OS::semaphoreWait(dispatchSemaphore);
    }
}

void Thread::tick()
{
    displayStarted = true;
    if (!q) return;
    for (uint8_t i = 0; i < max; i++)
    {
        ScheduledTask task = scheduled[i];
        if (!task.action.plain || task.context != frame) continue;
        if (q) --q;
        if (task.binding) task.action.binding(task.binding);
        else task.action.plain();
        scheduled[i].action.plain = nullptr;
        scheduled[i].binding = nullptr;
        scheduled[i].context = none;
    }
}
