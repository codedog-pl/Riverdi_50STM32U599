#include <gui/containers/DateTimeSetter.hpp>
#include "DateTime.hpp"

void DateTimeSetter::initialize()
{
    DateTimeSetterBase::initialize();
    DateTime initialValue = DateTime(2023, 12, 07, 10, 10, 0);
    setValue(initialValue);
    yearWheel.setItemSelectedCallback(yearWheelItemSelectedCallback);
    monthWheel.setItemSelectedCallback(monthWheelItemSelectedCallback);
    dayWheel.setItemSelectedCallback(dayWheelItemSelectedCallback);
    hourWheel.setItemSelectedCallback(hourWheelItemSelectedCallback);
    minuteWheel.setItemSelectedCallback(minuteWheelItemSelectedCallback);
    secondWheel.setItemSelectedCallback(secondWheelItemSelectedCallback);
}

void DateTimeSetter::setValue(DateTime &value)
{
    if (value.year < yearMin) value.year = yearMin;
    if (value.year > yearMax) value.year = yearMax;
    if (value.month < 1) value.month = 1;
    if (value.month > 12) value.month = 12;
    if (value.day < 1) value.day = 1;
    uint8_t dayMax = DateTime::daysInMonth(value.year, value.month);
    if (value.day > dayMax) value.day = dayMax;
    if (value.hour > 23) value.hour = 0;
    if (value.minute > 59) value.minute = 0;
    if (value.second > 59) value.second = 0;
    currentSetting = value;
    currentSetting.fraction = 0;
    yearWheel.animateToItem(value.year - yearMin);
    monthWheel.animateToItem(value.month - 1);
    dayWheel.animateToItem(value.day - 1);
    hourWheel.animateToItem(value.hour);
    minuteWheel.animateToItem(value.minute);
    secondWheel.animateToItem(value.second);
}

void DateTimeSetter::yearWheelUpdateItem(DateTimeSetter4DigitsN &item, int16_t itemIndex)
{
    item.setValue(yearMin + itemIndex);
}

void DateTimeSetter::monthWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex)
{
    item.setValue(itemIndex + 1);
}

void DateTimeSetter::dayWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex)
{
    item.setValue(itemIndex + 1);
}

void DateTimeSetter::hourWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex)
{
    item.setValue(itemIndex);
}

void DateTimeSetter::minuteWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex)
{
    item.setValue(itemIndex);
}

void DateTimeSetter::secondWheelUpdateItem(DateTimeSetter2DigitsN &item, int16_t itemIndex)
{
    item.setValue(itemIndex);
}
void DateTimeSetter::yearWheelItemSelectedHandler(int16_t itemSelected)
{
    currentSetting.year = yearMin + itemSelected;
    dayWheel.setNumberOfItems(DateTime::daysInMonth(currentSetting.year, currentSetting.month));
    dayWheel.invalidate();
    emitValueChangedCallback(currentSetting);
}

void DateTimeSetter::monthWheelItemSelectedHandler(int16_t itemSelected)
{
    currentSetting.month = 1 + itemSelected;
    dayWheel.setNumberOfItems(DateTime::daysInMonth(currentSetting.year, currentSetting.month));
    dayWheel.invalidate();
    emitValueChangedCallback(currentSetting);
}

void DateTimeSetter::dayWheelItemSelectedHandler(int16_t itemSelected)
{
    currentSetting.day = 1 + itemSelected;
    emitValueChangedCallback(currentSetting);
}

void DateTimeSetter::hourWheelItemSelectedHandler(int16_t itemSelected)
{
    currentSetting.hour = static_cast<uint8_t>(itemSelected);
    emitValueChangedCallback(currentSetting);
}

void DateTimeSetter::minuteWheelItemSelectedHandler(int16_t itemSelected)
{
    currentSetting.minute = static_cast<uint8_t>(itemSelected);
    emitValueChangedCallback(currentSetting);
}

void DateTimeSetter::secondWheelItemSelectedHandler(int16_t itemSelected)
{
    currentSetting.second = static_cast<uint8_t>(itemSelected);
    emitValueChangedCallback(currentSetting);
}
