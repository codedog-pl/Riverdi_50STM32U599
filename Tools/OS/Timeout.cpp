/**
 * @file        Timeout.cpp
 * @author      Adam Łyskawa
 *
 * @brief       A scheduled task wrapper to be used for resettable and cancellable timeouts. Implementation.
 * @remark      A part of the Woof Toolkit (WTK), RTOS API.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#include "AppThread.hpp"
#include "Timeout.hpp"

OS::Timeout::Timeout(double seconds, Action action)
    : m_taskId(0), m_ticks(WTK_OS_TICKS_PER_SECOND * seconds), m_action(action), m_binding() { }

OS::Timeout::Timeout(double seconds, void *arg, BindingAction action)
    : m_taskId(0), m_ticks(WTK_OS_TICKS_PER_SECOND * seconds), m_action(action), m_binding(arg) { }

OS::Timeout::~Timeout()
{
    if (!m_taskId) return;
    AppThread::cancel(m_taskId);
    m_ticks = 0;
    m_action = nullptr;
    m_binding = nullptr;
}

void OS::Timeout::set()
{
    if (m_taskId || !m_ticks) return;
    m_taskId = m_binding
        ? AppThread::delay(m_ticks, m_binding, m_action.binding)
        : AppThread::delay(m_ticks, m_action.plain);
}

void OS::Timeout::set(double seconds)
{
    if (seconds <= 0 || m_taskId || !m_ticks) return;
    m_ticks = WTK_OS_TICKS_PER_SECOND * seconds;
    m_taskId = m_binding
        ? AppThread::delay(m_ticks, m_binding, m_action.binding)
        : AppThread::delay(m_ticks, m_action.plain);
}

void OS::Timeout::reset()
{
    clear();
    set();
}

void OS::Timeout::clear()
{
    AppThread::cancel(m_taskId);
}
