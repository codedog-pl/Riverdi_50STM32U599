/**
 * @file        itm.c
 * @author      Adam Łyskawa
 *
 * @brief       Redirects standard output into SWV ITM Data Console. Implementation.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @remarks     Reuqires SWV debugging enabled in trace mode.
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#include "hal.h"
#include "itm.h"

/**
 * @brief Enables SWV ITM Data Console output.
 * @return
 */
void ITM_Enable(void)
{
#ifdef DCB
    DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;
#else
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#endif
    ITM->LAR = 0xC5ACCE55;
    ITM->TER = 0x1;
}

/**
 * @brief Sends data to the SWV ITM Data Console (used for `stdout` output).
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
