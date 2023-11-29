/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_host_msc.c
  * @author  MCD Application Team
  * @brief   USBX host applicative file
  ******************************************************************************
   * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "ux_host_msc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_usbx_host.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern FX_MEDIA *media[];
extern UINT msc_index;
extern TX_EVENT_FLAGS_GROUP ux_app_EventFlag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing msc_process_thread_entry.
  * @param  thread_input: Not used
  * @retval none
  */
VOID msc_process_thread_entry(ULONG thread_input)
{
  ULONG storage_media_flag = 0;
  printf("Starting MSC thread...\r\n");
  while(1)
  {
    /* Wait until the requested flag STORAGE_MEDIA is received */
    if (tx_event_flags_get(&ux_app_EventFlag, STORAGE_MEDIA, TX_OR_CLEAR,
                           &storage_media_flag, TX_WAIT_FOREVER) != TX_SUCCESS)
    {

        printf("ERROR in tx_event_flags_get().\r\n");
    }
    else
    {
        printf("TICK: tx_event_flags_get().\r\n");
      tx_thread_sleep(MS_TO_TICK(10));
    }
  }
}
/* USER CODE END 1 */
