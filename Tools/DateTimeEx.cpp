/**
 * @file        DateTimeEx.cpp
 * @author      CodeDog
 * @brief       DateTime class extended with RTC / FAT methods.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "DateTimeEx.hpp"

DateTimeEx::DateTimeEx() : DateTime() { }

DateTimeEx::DateTimeEx(bool initializeFromRTC) : DateTime()
{
    if (initializeFromRTC) getRTC();
}

DateTimeEx::DateTimeEx(DateTimeTypeDef &dt)
    : DateTime(dt.date.y, dt.date.m, dt.date.d, dt.time.h, dt.time.m, dt.time.s, dt.time.f) {}

DateTimeTypeDef* DateTimeEx::c_ptr() { return (DateTimeTypeDef*)(void*)this; }

bool DateTimeEx::getRTC() { return RTC_GetDateTime(c_ptr()) == HAL_OK; }

bool DateTimeEx::setRTC() { return RTC_SetDateTime(c_ptr()) == HAL_OK; }

void DateTimeEx::getFAT(DWORD ftd) { FAT2DateTime(ftd, c_ptr()); }

void DateTimeEx::setFAT(DWORD& ftd) { ftd = DateTime2FAT(c_ptr()); }
