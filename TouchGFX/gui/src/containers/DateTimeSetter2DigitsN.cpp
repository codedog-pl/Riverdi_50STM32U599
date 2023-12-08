#include <gui/containers/DateTimeSetter2DigitsN.hpp>

DateTimeSetter2DigitsN::DateTimeSetter2DigitsN()
{

}

void DateTimeSetter2DigitsN::initialize()
{
    DateTimeSetter2DigitsNBase::initialize();
}

void DateTimeSetter2DigitsN::setValue(uint16_t value)
{
    Unicode::snprintf(digitsTxtBuffer, DIGITSTXT_SIZE, "%02u", value);
    digitsTxt.invalidate();
}