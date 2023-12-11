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
#include "fs_bindings.h"
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

VOID msc_process_thread_entry(ULONG thread_input)
{
  ULONG storage_media_flag = 0;
  USBH_UsrLog("USBH: MSC thread started.");
  while(1)
  {
    if (tx_event_flags_get(
      &ux_app_EventFlag,
      STORAGE_MEDIA_CONNECTED | STORAGE_MEDIA_DISCONNECTED,
      TX_OR_CLEAR,
      &storage_media_flag,
      TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      if (storage_media_flag & STORAGE_MEDIA_CONNECTED)
      {
        fs_mount(media[msc_index], "1:/");
        log_msg(3, "USBH: External storage mounted as \"1:/\".");
      }
      else if (storage_media_flag & STORAGE_MEDIA_DISCONNECTED)
      {
        fs_umount("1:/");
        log_msg(3, "USBH: External storage at \"1:/\" disconnected.");
      }
    }
    else Error_Handler();
  }
}

/* USER CODE END 1 */
