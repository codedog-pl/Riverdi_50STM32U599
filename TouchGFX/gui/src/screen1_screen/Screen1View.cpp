#include <gui/screen1_screen/Screen1View.hpp>
#ifndef SIMULATOR
#include "DateTimeEx.hpp"
#endif

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    dateTimeSetter.setValue(sysBar.time());
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::getTimeClick()
{
    dateTimeSetter.setValue(sysBar.time());
}

void Screen1View::setTimeClick()
{
#ifndef SIMULATOR
    reinterpret_cast<DateTimeEx&>(dateTimeSetter.getValue()).setRTC();
    sysBar.resetTime();
#endif
}
