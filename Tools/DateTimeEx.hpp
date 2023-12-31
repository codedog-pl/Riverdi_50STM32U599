/**
 * @file        DateTimeEx.hpp
 * @author      CodeDog
 *
 * @brief       DateTime class extended with RTC / FAT methods.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include "DateTime.hpp"
#include "bindings.h"
EXTERN_C_BEGIN
#include "datetime.h"
EXTERN_C_END

class __attribute__((__packed__)) DateTimeEx : public DateTime
{
public:

    /// @brief Creates an empty `DateTimeEx` object, with all fields set to zero.
    DateTimeEx() : DateTime() { }

    /// @brief Creates an initialized `DateTimeEx` object, either with zero or the current time.
    /// @param initializeFromRTC 0: Reset to zero (empty). 1: Initialize with current time.
    DateTimeEx(bool initializeFromRTC) : DateTime() { if (initializeFromRTC) getRTC(); }

    /// @brief Creates a `DateTimeEx` object from `DateTimeTypeDef` reference.
    /// @param dt `DateTimeTypeDef` reference.
    DateTimeEx(DateTimeTypeDef& dt) : DateTime(dt.date.y, dt.date.m, dt.date.d, dt.time.h, dt.time.m, dt.time.s, dt.time.f) { }

    /// @returns Reinterpretted C compatible `DateTimeTypeDef` pointer.
    DateTimeTypeDef* c_ptr() { return (DateTimeTypeDef*)(void*)this; }

    /// @brief Loads current real time clock into this structure.
    /// @return 1: Success. 0: Failure.
    bool getRTC() { return RTC_GetDateTime(c_ptr()) == HAL_OK; }

    /// @brief Sets the real time clock with value with this structure.
    /// @return 1: Success. 0: Failure.
    bool setRTC() { return RTC_SetDateTime(c_ptr()) == HAL_OK; }

    /// @brief Sets this structure with the FATFS timestamp value.
    /// @param ftd FATFS timestamp value.
    void getFAT(DWORD ftd) { FAT2DateTime(ftd, c_ptr()); }

    /// @brief Sets the FATFS timestamp value with this structure.
    /// @param ftd FATFS timestamp reference.
    void setFAT(DWORD& ftd) { ftd = DateTime2FAT(c_ptr()); }

};
