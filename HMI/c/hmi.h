/**
 * @file        hmi.h
 * @author      CodeDog
 * @brief       -
 * @remarks     -
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

#include <stdint.h>
#include "bindings.h"

#define HMI_NONE        0u
#define HMI_SD          1u
#define HMI_USB         2u
#define HMI_DISPLAY     4u
#define HMI_ALL         (HMI_SD | HMI_USB | HMI_DISPLAY)

#define HMI_SD_OK       HMI_Init(HMI_SD);
#define HMI_USB_OK      HMI_Init(HMI_USB);
#define HMI_DISPLAY_OK  HMI_Init(HMI_DISPLAY);

EXTERN_C_BEGIN

extern uint32_t HMI_SysInit;

void HMI_Start();
void HMI_Init(uint32_t flags);
void HMI_TriggerUSBMediaMounted();
void HMI_TriggerUSBMediaUnmounted();

EXTERN_C_END
