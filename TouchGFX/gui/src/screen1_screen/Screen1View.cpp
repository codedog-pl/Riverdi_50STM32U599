#include <gui/screen1_screen/Screen1View.hpp>
#include "DateTimeEx.hpp"

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    DateTimeEx now(true);
    dateTimeSetter1.setValue(now);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::setDateTime(DateTime& value)
{
#ifndef SIMULATOR
    reinterpret_cast<DateTimeEx&>(value).setRTC();
#endif
}