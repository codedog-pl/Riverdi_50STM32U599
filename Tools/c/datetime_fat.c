/**
 * @file        datetime.h
 * @author      Adam Łyskawa
 *
 * @brief       FATFS date / time tools. Implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "datetime_fat.h"

#ifdef USE_FATFS

/**
 * @fn DWORD DateTime2FAT(DateTimeTypeDef*)
 * @brief Gets the FAT (MS-DOS type) date and time from ISO date/time.
 * @param dateTime ISO date/time pointer.
 * @return FAT time.
 */
DWORD DateTime2FAT(DateTimeTypeDef* dt)
{
    return FAT_TIME(dt->date.y, dt->date.m, dt->date.d, dt->time.h, dt->time.m, dt->time.s);
}

/**
 * @fn DWORD RTC2FAT(RTC_DateTypeDef*, RTC_TimeTypeDef*)
 * @brief Gets the FAT (MS-DOS type) date and time from RTC date and time.
 * @param rtcDate RTC date pointer.
 * @param rtcTime RTC time pointer.
 * @return FAT time.
 */
DWORD RTC2FAT(RTC_DateTypeDef* rd, RTC_TimeTypeDef* rt)
{
    return FAT_TIME(2000 + rd->Year, rd->Month, rd->Date, rt->Hours, rt->Minutes, rt->Seconds);
}

/**
 * @fn void FAT2DateTime(DWORD, DateTimeTypeDef*)
 * @brief Gets the ISO date/time from FAT (MS-DOS type) date/time format.
 * @param fatTime FAT time.
 * @param dateTime ISO date/time pointer.
 */
void FAT2DateTime(DWORD ft, DateTimeTypeDef* dt)
{
    FAT_TIME_SET(ft, dt->date.y, dt->date.m, dt->date.d, dt->time.h, dt->time.m, dt->time.s);
}

/**
 * @fn DWORD RTC_GetFATTime()
 * @brief Gets the current RTC date and time as FAT time.
 * @return FAT time.
 */
DWORD RTC_GetFATTime()
{
    RTC_DateTypeDef rd;
    RTC_TimeTypeDef rt;
    if (HAL_RTC_GetTime(&hrtc, &rt, RTC_FORMAT_BIN) != HAL_OK) return 0;
    if (HAL_RTC_GetDate(&hrtc, &rd, RTC_FORMAT_BIN) != HAL_OK) return 0;
    return RTC2FAT(&rd, &rt);
}

#endif
