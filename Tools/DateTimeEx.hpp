/**
 * @file        DateTimeEx.hpp
 * @author      Adam Łyskawa
 *
 * @brief       `DateTime` class extension with HAL RTC access.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "DateTime.hpp"
#include "bindings.h"
EXTERN_C_BEGIN
#include "datetime.h"
EXTERN_C_END

/// @brief `DateTime` class extension with HAL RTC access.
class __attribute__((__packed__)) DateTimeEx final : public DateTime
{

public:

    /// @brief Creates an empty `DateTimeEx` object.
    DateTimeEx() : DateTime() { }

    /// @brief Creates an initialized `DateTimeEx` object, either with zero or the current time.
    /// @param initializeFromRTC 0: Reset to zero (empty). 1: Initialize with current time.
    DateTimeEx(bool initializeFromRTC) : DateTime() { if (initializeFromRTC) getRTC(); }

    /// @brief Creates a `DateTimeEx` object from `DateTimeTypeDef` reference.
    /// @param dt `DateTimeTypeDef` reference.
    DateTimeEx(const DateTimeTypeDef& dt) : DateTime(dt.date.y, dt.date.m, dt.date.d, dt.time.h, dt.time.m, dt.time.s, dt.time.f) { }

    /// @returns Reinterpretted C compatible `DateTimeTypeDef` pointer.
    inline DateTimeTypeDef* c_ptr() const { return (DateTimeTypeDef*)(void*)this; }

    /// @brief Loads current real time clock into this structure.
    /// @returns 1: Success. 0: Failure.
    inline bool getRTC() { return RTC_GetDateTime(c_ptr()) == HAL_OK; }

    /// @brief Sets the real time clock with value with this structure.
    /// @returns 1: Success. 0: Failure.
    inline bool setRTC() { return RTC_SetDateTime(c_ptr()) == HAL_OK; }

};
