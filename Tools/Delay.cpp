/**
 * @file        Delay.cpp
 * @author      CodeDog
 * @brief       Delays execution of actions using OS thread ticks.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#include "Delay.hpp"
#include "Log.hpp"

void Delay::tick()
{
    if (lastActive < 0) return;
    for (int i = 0; i <= lastActive; i++)
    {
        WaitHandle* event = &m_pool[i];
        if (!event->action.plain || --event->ticks > 0) continue;
        if (event->binding) event->action.binding(event->binding);
        else event->action.plain();
        if (event->reset) event->ticks = event->reset;
        else { event->cancel(); lastActiveUpdate(); }
    }
}

WaitHandle *Delay::getWaitHandle()
{
    for (int i = 0; i < poolSize; i++)
    {
        WaitHandle* event = &m_pool[i];
        if (!event->action.plain) return event;
    }
    Log::msg(LogMessage::error, "WAIT HANDLE POOL EXHAUSTED!");
    return nullptr;
}

void Delay::lastActiveUpdate()
{
    for (int i = poolSize - 1; i >= 0; --i)
        if (m_pool[i].action.plain) { lastActive = i; return; }
    lastActive = -1;
}

WaitHandle *Delay::set(uint32_t ticks, Action action)
{
    WaitHandle* event = getWaitHandle();
    if (!event) return nullptr;
    event->action.plain = action;
    event->ticks = ticks;
    event->reset = 0;
    event->binding = nullptr;
    lastActiveUpdate();
    return event;
}

WaitHandle *Delay::repeat(uint32_t ticks, Action action)
{
    WaitHandle* event = getWaitHandle();
    if (!event) return nullptr;
    event->action.plain = action;
    event->ticks = ticks;
    event->reset = ticks;
    event->binding = nullptr;
    lastActiveUpdate();
    return event;
}

WaitHandle *Delay::set(uint32_t ticks, BindingAction action, void *binding)
{
    WaitHandle* event = getWaitHandle();
    if (!event) return nullptr;
    event->action.binding = action;
    event->ticks = ticks;
    event->reset = 0;
    event->binding = binding;
    lastActiveUpdate();
    return event;
}

WaitHandle *Delay::repeat(uint32_t ticks, BindingAction action, void *binding)
{
    WaitHandle* event = getWaitHandle();
    if (!event) return nullptr;
    event->action.binding = action;
    event->ticks = ticks;
    event->reset = ticks;
    event->binding = binding;
    lastActiveUpdate();
    return event;
}

void Delay::clear(WaitHandle **waitHandlePtr)
{
    if (!waitHandlePtr || !*waitHandlePtr) return;
    (*waitHandlePtr)->cancel();
    lastActiveUpdate();
    *waitHandlePtr = nullptr;
}
