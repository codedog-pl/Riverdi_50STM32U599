#ifndef DATETIMESETTER_HPP
#define DATETIMESETTER_HPP

#include <gui_generated/containers/DateTimeSetterBase.hpp>

class DateTimeSetter : public DateTimeSetterBase
{
public:

    DateTimeSetter() :
        currentSetting(yearMin, 1, 1),
        yearWheelItemSelectedCallback(this, &DateTimeSetter::yearWheelItemSelectedHandler),
        monthWheelItemSelectedCallback(this, &DateTimeSetter::monthWheelItemSelectedHandler),
        dayWheelItemSelectedCallback(this, &DateTimeSetter::dayWheelItemSelectedHandler),
        hourWheelItemSelectedCallback(this, &DateTimeSetter::hourWheelItemSelectedHandler),
        minuteWheelItemSelectedCallback(this, &DateTimeSetter::minuteWheelItemSelectedHandler),
        secondWheelItemSelectedCallback(this, &DateTimeSetter::secondWheelItemSelectedHandler) { };

    virtual ~DateTimeSetter() { }

    void initialize() override;
    void setValue(DateTime& value) override;

protected:
    static constexpr uint16_t yearMin = 2023; // As used to set system time.
    static constexpr uint16_t yearMax = 2107; // Year the FATFS expires.
    DateTime currentSetting;

    void yearWheelUpdateItem(DateTimeSetter4DigitsN &item, int16_t itemIndex) override;

    void monthWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex) override;

    void dayWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex) override;

    void hourWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex) override;

    void minuteWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex) override;

    void secondWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex) override;

    Callback<DateTimeSetter, int16_t> yearWheelItemSelectedCallback;
    void yearWheelItemSelectedHandler(int16_t itemSelected);

    Callback<DateTimeSetter, int16_t> monthWheelItemSelectedCallback;
    void monthWheelItemSelectedHandler(int16_t itemSelected);

    Callback<DateTimeSetter, int16_t> dayWheelItemSelectedCallback;
    void dayWheelItemSelectedHandler(int16_t itemSelected);

    Callback<DateTimeSetter, int16_t> hourWheelItemSelectedCallback;
    void hourWheelItemSelectedHandler(int16_t itemSelected);

    Callback<DateTimeSetter, int16_t> minuteWheelItemSelectedCallback;
    void minuteWheelItemSelectedHandler(int16_t itemSelected);

    Callback<DateTimeSetter, int16_t> secondWheelItemSelectedCallback;
    void secondWheelItemSelectedHandler(int16_t itemSelected);

};

#endif // DATETIMESETTER_HPP
