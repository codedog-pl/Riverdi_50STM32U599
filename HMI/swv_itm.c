/**
 * @file        swv_itm.c
 * @author      CodeDog
 * @brief       Redirects standard output into SWV ITM Data Console.
 * @remarks     Reuqires SWV debugging enabled in trace mode.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include "stm32u5xx_hal.h"

/**
 * @brief Sends data to the SWV ITM Data Console.
 * @remarks This function will block when there are unsent characters in the buffer and the console is connected.
 *
 * @param file Ignored.
 * @param ptr Data pointer.
 * @param len Data length.
 * @return Data length.
 */
int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++) ITM_SendChar(*ptr++);
    return len;
}
