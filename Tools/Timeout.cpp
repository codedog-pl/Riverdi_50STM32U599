/**
 * @file        Timeout.cpp
 * @author      CodeDog
 * @brief       A simple timeout feature implementation using FreeRTOS threads.
 *
 * @copyright	(c)2023 CodeDog, All rights reserved.
 */

#include "Timeout.hpp"

Timeout::Timeout(double seconds, Action action) : m_waitHandle(), m_ticks(1000.0 * seconds), m_action(action) { }

Timeout::~Timeout()
{
    if (!m_waitHandle) return;
    Delay::clear(&m_waitHandle);
    m_ticks = 0;
    m_action = nullptr;
}

void Timeout::set()
{
    if (m_waitHandle || !m_ticks) return;
    m_waitHandle = Delay::set(m_ticks, m_action);
}

void Timeout::set(double seconds)
{
    if (seconds <= 0 || m_waitHandle || !m_ticks) return;
    m_ticks = 1000.0 * seconds;
    m_waitHandle = Delay::set(m_ticks, m_action);
}

void Timeout::reset()
{
    clear();
    set();
}

void Timeout::clear()
{
    Delay::clear(&m_waitHandle);
}
