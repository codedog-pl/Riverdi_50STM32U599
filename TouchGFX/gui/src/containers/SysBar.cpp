#include <gui/containers/SysBar.hpp>
#include <new>
#ifndef SIMULATOR
#include "DateTimeEx.hpp"
#endif

void SysBar::initialize()
{
    SysBarBase::initialize();
    updateTime();
}

void SysBar::resetTime()
{
    new (&m_currentTime) DateTime();
    updateTime();
}

DateTime& SysBar::time()
{
    return m_currentTime;
}

void SysBar::updateTime()
{
    bool update = false;
#ifndef SIMULATOR
    DateTimeEx now(true);
    update = !m_currentTime.isSet() || now.atLeastSecondAheadOf(m_currentTime);
    if (update) m_currentTime = now;
#endif
    if (update)
    {
        Unicode::snprintf(
            clockTxtBuffer, CLOCKTXT_SIZE,
            "%02i.%02i.%04i %02i:%02i:%02i",
            m_currentTime.day,
            m_currentTime.month,
            m_currentTime.year,
            m_currentTime.hour,
            m_currentTime.minute,
            m_currentTime.second
        );
        clockTxt.invalidate();
    }
}
