#include <gui/containers/DateTimeSetter4DigitsN.hpp>

DateTimeSetter4DigitsN::DateTimeSetter4DigitsN()
{

}

void DateTimeSetter4DigitsN::initialize()
{
    DateTimeSetter4DigitsNBase::initialize();
}

void DateTimeSetter4DigitsN::setValue(uint16_t value)
{
    Unicode::snprintf(digitsTxtBuffer, DIGITSTXT_SIZE, "%04u", value);
    digitsTxt.invalidate();
}
