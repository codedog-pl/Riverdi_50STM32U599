/**
 * @file        TimeSpan.hpp
 * @author      Adam Łyskawa
 *
 * @brief       A structure to store and manipulate time span values. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include <cstdint>

/// @brief A structure to store and manipulate time span values.
struct TimeSpan
{

    /// @brief Creates zero time span.
    TimeSpan();

    /// @brief Creates a time span from seconds.
    TimeSpan(double value);

    /// @brief Creates a time span from hours, minutes and seconds.
    TimeSpan(uint16_t hours, uint8_t minutes, double seconds);

    /// @brief Creates a time span from days, hours, minutes and seconds.
    TimeSpan(int days, uint8_t hours, uint8_t minutes, double seconds);

    /// @returns Seconds component of the time span.
    double seconds() const;

    /// @returns Minutes component of the time span.
    uint8_t minutes() const;

    /// @returns Hours component of the time span.
    uint8_t hours() const;

    /// @returns Days component of the time span.
    int days() const;

    /// @returns Sign component of the time span, -1 if negative, 1 if positive, 0 otherwise.
    int sign() const;

    /// @returns Total number of seconds.
    double totalSeconds() const;

    /// @returns Total number of minutes.
    double totalMinutes() const;

    /// @returns Total number of hours.
    double totalHours() const;

    /// @returns Total number of days.
    double totalDays() const;

    TimeSpan& operator+=(TimeSpan& other);
    TimeSpan& operator-=(TimeSpan& other);
    TimeSpan& operator+=(double other);
    TimeSpan& operator-=(double other);

    bool operator==(TimeSpan& other) const;
    bool operator!=(TimeSpan& other) const;
    bool operator<=(TimeSpan& other) const;
    bool operator>=(TimeSpan& other) const;
    bool operator<(TimeSpan& other) const;
    bool operator>(TimeSpan& other) const;

private:
    double m_value; // Internal number of seconds.

};
