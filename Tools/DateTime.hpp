/**
 * @file        DateTime.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A universal, simple date / time storage class. Header only.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <cmath>
#include <cstdint>
#include <ctime>
#include "TimeSpan.hpp"

#pragma pack(push, 1)
/// @brief Date and time storage class. Provides comparison operators and validation.
/// @remarks Binary level compatible with the `DateTimeTypeDef` defined in `datetime.h`.
struct DateTime
{

    static constexpr int16_t startYearCTime = 1900; // A starting year used by `ctime` library.
    static constexpr int16_t startYearUnix = 1970; // A starting year of the UNIX epoch.

    int16_t year;       // Year value.
    uint8_t month;      // Month number counted from 1.
    uint8_t day;        // Day number counted from 1.
    uint8_t hour;       // Hour number [0..23].
    uint8_t minute;     // Minute number [0..59].
    uint8_t second;     // Second number [0..59].
    double fraction;    // Additional fraction of the second value (x >= 0 && x < 1).

    /// @brief Creates an empty DateTime instance.
    DateTime() : year(0), month(1), day(1), hour(0), minute(0), second(0), fraction(0) { }

    /// @brief Constructs a `DateTime` instance from a `time_t` value.
    /// @param cTime C time.
    DateTime(time_t cTime)
    {
        struct tm ltm;
        #ifdef _WIN32
        localtime_s(&ltm, &cTime);
        #else
        localtime_r(&cTime, &ltm);
        #endif
        year = startYearCTime + ltm.tm_year;
        month = ltm.tm_mon + 1; // `tm_mon` is 0-based!
        day = ltm.tm_mday;      // ...and `tm_mday` is not!
        hour = ltm.tm_hour;
        minute = ltm.tm_min;
        second = ltm.tm_sec;
        fraction = 0;
    }

    /// @brief Creates a `DateTime` instance for a specific date and time.
    /// @param _year Year number.
    /// @param _month Month number, 1..12.
    /// @param _day Day number, 1..31.
    /// @param _hour Hour number, 0..23.
    /// @param _minute Minute number, 0..59.
    /// @param _second Second number, 0..59.
    /// @param _fraction Fraction of second floating point value, 0..1, less than 1.
    DateTime(
        int16_t _year, uint8_t _month, uint8_t _day,
        uint8_t _hour = 0, uint8_t _minute = 0, uint8_t _second = 0, double _fraction = 0)
        : year(_year), month(_month), day(_day),
          hour(_hour), minute(_minute), second(_second), fraction(_fraction) { }

    /// @brief Calculates the number of days in month.
    static inline uint8_t daysInMonth(uint16_t _year, uint8_t _month)
    {
        return _month != 2 ? 30 + ((_month % 2) ^ (_month > 7)) : 28 + (_year % 4 == 0 && (_year % 100 != 0 || _year % 400 == 0));
    }

    /// @returns 1: The date is set. 0: The date is empty / zero.
    inline bool isSet() const
    {
        return
            (year != 0 && year != startYearCTime && year != startYearUnix) ||
            month != 1 || day != 1 ||
            hour != 0 || minute != 0 || second != 0 || fraction != 0.0;
    }

    /// @returns 1: The data is valid. 0: The data is invalid.
    inline bool isValid() const
    {
        return
            month >= 1 && month <= 12 && day >= 1 && day <= daysInMonth(year, month) &&
            hour < 24 && minute < 60 && second < 60 && fraction < 1;
    }

    /// @returns 1: The other date has the same day.
    inline bool isTheSameDay(DateTime& other) const
    {
        return year == other.year && month == other.month && day == other.day;
    }

    /// @returns 1: This time is at least 1 second ahead of the other time (the other time lags behind).
    bool atLeastSecondAheadOf(DateTime& other) const
    {
        if (year > other.year) return true;
        if (year < other.year) return false;
        if (month > other.month) return true;
        if (month < other.month) return false;
        if (day > other.day) return true;
        if (day < other.day) return false;
        if (hour > other.hour) return true;
        if (hour < other.hour) return false;
        if (minute > other.minute) return true;
        if (minute < other.minute) return false;
        if (second > other.second) return true;
        if (second < other.second) return false;
        return false;
    }

    /// @brief Resets time to the empty / zero value.
    inline void reset()
    {
        year = 0; month = 1; day = 1;
        hour = 0; minute = 0; second = 0; fraction = 0.0;
    }

    /// @brief Converts `DateTime` structure into `time_t` value.
    operator time_t() const
    {
        tm ltm = {};
        ltm.tm_year = year - startYearCTime;
        ltm.tm_mon = month - 1; // `tm_mon` is 0-based!
        ltm.tm_mday = day;      // ...and `tm_mday` is not!
        ltm.tm_hour = hour;
        ltm.tm_min = minute;
        ltm.tm_sec = second;
        return mktime(&ltm);
    }

    /// @brief The object will evaluate to true if it's set to a non-default value.
    inline operator bool() const { return isSet(); }

    /// @returns A time span between this and the other date/time object.
    TimeSpan operator-(DateTime& other) const
    {
        time_t t1 = *this;
        time_t t0 = other;
        double diffIntPart = difftime(t1, t0);
        double diffFractPart = fraction - other.fraction;
        return diffIntPart + diffFractPart;
    }

    /// @brief Tests if this instance value is equal to the other instance value.
    /// @param other Other instance reference.
    /// @returns True if equal.
    bool operator ==(const DateTime& other) const { return eq(other); }

    /// @brief Tests if this instance value is NOT equal to the other instance value.
    /// @param other Other instance reference.
    /// @returns True if NOT equal.
    bool operator !=(const DateTime& other) const { return !eq(other); }

    /// @brief Tests if this instance value is less or equal to the other instance value.
    /// @param other Other instance reference.
    /// @returns True if less or equal.
    bool operator <=(const DateTime& other) const { return cmp(other, true, true); }

    /// @brief Tests if this instance value is greater or equal to the other instance value.
    /// @param other Other instance reference.
    /// @returns True if greater or equal.
    bool operator >=(const DateTime& other) const { return cmp(other, false, true); }

    /// @brief Tests if this instance value is less than the other instance value.
    /// @param other Other instance reference.
    /// @returns True if less (earlier).
    bool operator <(const DateTime& other) const { return cmp(other, true, false); }

    /// @brief Tests if this instance value is greater than the other instance value.
    /// @param other Other instance reference.
    /// @returns True if greater (later).
    bool operator >(const DateTime& other) const { return cmp(other, false, false); }

protected:

    /// @brief Compares this time with the other time.
    /// @param other The other time reference.
    /// @param lt 1: Return true if this time is less (or equal if `eq` is set) than the other time.
    /// @param eq 1: Return true also when the times are equal. 0: Return true only when the times differ.
    /// @returns Comparison result dependent on `lt` and `eq` arguments.
    bool cmp(const DateTime& other, bool lt, bool eq) const
    {
        if (year < other.year) return lt;
        if (year > other.year) return !lt;
        if (month < other.month) return lt;
        if (month > other.month) return !lt;
        if (day < other.day) return lt;
        if (day > other.day) return !lt;
        if (hour < other.hour) return lt;
        if (hour > other.hour) return !lt;
        if (minute < other.minute) return lt;
        if (minute > other.minute) return !lt;
        if (second < other.second) return lt;
        if (second > other.second) return !lt;
        if (fraction < other.fraction) return lt;
        if (fraction > other.fraction) return !lt;
        return eq;
    }

    /// @brief Returns true only if the other time is equal to this time.
    /// @param other The other time reference.
    /// @returns 1: Times are equal. 0: Times are different.
    bool eq(const DateTime& other) const
    {
        return
            year == other.year &&
            month == other.month &&
            day == other.day &&
            hour == other.hour &&
            minute == other.minute &&
            second == other.second &&
            fraction == other.fraction;
    }

};
#pragma pack(pop)
