/**
 * @file        datetime.h
 * @author      Adam Łyskawa
 *
 * @brief       FATFS date / time tools. Header file.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "target.h"
#include "bindings.h"

#ifdef USE_FATFS

#include "integer.h"
#include "datetime.h"

/**
 * @def FAT_TIME
 * @brief Returns FAT time calculated from date components.
 */
#define FAT_TIME(y, M, d, h, m, s) ((DWORD)(y-1980)<<25|(DWORD)M<<21|(DWORD)d<<16|(DWORD)h<<11|(DWORD)m<<5|(DWORD)s>>1)

/**
 * @def FAT_TIME_SET
 * @brief Sets the date components from FAT time.
 */
#define FAT_TIME_SET(f, y, M, d, h, m, s) {y=1980+(f>>25&127);M=f>>21&15;d=f>>16&31;h=f>>11&31;m=f>>5&63;s=(f&15)<<1;}

EXTERN_C_BEGIN

DWORD DateTime2FAT(DateTimeTypeDef* dt);
DWORD RTC2FAT(RTC_DateTypeDef* rd, RTC_TimeTypeDef* rt);
void FAT2DateTime(DWORD ft, DateTimeTypeDef* dt);
DWORD RTC_GetFATTime();

EXTERN_C_END

#endif
