/**
 * @file        HMI_C.h
 * @author      CodeDog
 * @brief       -
 * @remarks     -
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <stdint.h>

#define HMI_NONE        0u
#define HMI_FILEX       1u
#define HMI_USBX        2u
#define HMI_TOUCHGFX    4u
#define HMI_ALL         (HMI_FILEX | HMI_USBX | HMI_TOUCHGFX)

extern uint32_t HMI_SysInit;
void HMI_Start();
