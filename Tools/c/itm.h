/**
 * @file        itm.h
 * @author      CodeDog
 * @brief       Redirects standard output into SWV ITM Data Console.
 *              Header file, exports `ITM_Enable` function.
 * @remarks     Reuqires SWV debugging enabled in trace mode.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

void ITM_Enable(void);
