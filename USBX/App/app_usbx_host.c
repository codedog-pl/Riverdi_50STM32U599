/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_host.c
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
#include "app_usbx_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "hmi.h"
#include "log.h"

#include "usb_otg.h"
#include "ux_api.h"
#include "ux_hcd_stm32.h"
#include "ux_host_class_storage.h"
#include "ux_host_stack.h"

//#include "ux_host_stack.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MSC_INSTANCE     1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static TX_THREAD ux_host_app_thread;
/* USER CODE BEGIN PV */

UX_HOST_CLASS_STORAGE       *storage_instance[MSC_INSTANCE];
UX_HOST_CLASS_STORAGE_MEDIA *storage_media;
FX_MEDIA                    *media[MSC_INSTANCE];
TX_THREAD                   msc_app_thread;
TX_EVENT_FLAGS_GROUP        ux_app_EventFlag;
UINT                        msc_index;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID app_ux_host_thread_entry(ULONG thread_input);
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);
/* USER CODE BEGIN PFP */

/**
  * @brief  USBX_APP_Host_Init
  *         Initialization of USB host.
  * @param  none
  * @retval none
  */
VOID USBX_APP_Host_Init(VOID)
{
  /* USER CODE BEGIN USB_Host_Init_PreTreatment_0 */

  /* USER CODE END USB_Host_Init_PreTreatment_0 */

  /* Initialize the LL driver */
  MX_USB_OTG_HS_HCD_Init();
  /* Initialize the host controller driver */
  ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                             _ux_hcd_stm32_initialize, (ULONG)USB_OTG_HS,
                             (ULONG)&hhcd_USB_OTG_HS);

  /* Enable USB Global Interrupt*/
  HAL_HCD_Start(&hhcd_USB_OTG_HS);

  /* USER CODE BEGIN USB_Host_Init_PostTreatment1 */

  USBH_UsrLog("USBH: Ready.");
  HMI_USB_OK

  /* USER CODE END USB_Host_Init_PostTreatment1 */
}

/* USER CODE END PFP */

/**
  * @brief  Application USBX Host Initialization.
  * @param  memory_ptr: memory pointer
  * @retval status
  */
UINT MX_USBX_Host_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;
  UCHAR *pointer;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_USBX_Host_Init0 */
  USBH_UsrLog("USBH: Initializing...");
  /* USER CODE END MX_USBX_Host_Init0 */

  /* Allocate the stack for USBX Memory */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_HOST_MEMORY_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_ALLOCATE_STACK_ERROR */
    return TX_POOL_ERROR;
    /* USER CODE END USBX_ALLOCATE_STACK_ERROR */
  }

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_HOST_MEMORY_STACK_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_SYSTEM_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_SYSTEM_INITIALIZE_ERROR */
  }

  /* Install the host portion of USBX */
  if (ux_host_stack_initialize(ux_host_event_callback) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_HOST_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_HOST_INITIALIZE_ERROR */
  }

  /* Register a callback error function */
  ux_utility_error_callback_register(&ux_host_error_callback);

  /* Initialize the host storage class */
  if (ux_host_stack_class_register(_ux_system_host_class_storage_name,
                                   ux_host_class_storage_entry) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_HOST_STORAGE_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_HOST_STORAGE_REGISTER_ERROR */
  }

  /* Allocate the stack for host application main thread */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, UX_HOST_APP_THREAD_STACK_SIZE,
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_ALLOCATE_STACK_ERROR */
    return TX_POOL_ERROR;
    /* USER CODE END MAIN_THREAD_ALLOCATE_STACK_ERROR */
  }

  /* Create the host application main thread */
  if (tx_thread_create(&ux_host_app_thread, UX_HOST_APP_THREAD_NAME, app_ux_host_thread_entry,
                       0, pointer, UX_HOST_APP_THREAD_STACK_SIZE, UX_HOST_APP_THREAD_PRIO,
                       UX_HOST_APP_THREAD_PREEMPTION_THRESHOLD, UX_HOST_APP_THREAD_TIME_SLICE,
                       UX_HOST_APP_THREAD_START_OPTION) != TX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_THREAD_CREATE_ERROR */
    return TX_THREAD_ERROR;
    /* USER CODE END MAIN_THREAD_CREATE_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Host_Init1 */

  /* Allocate the stack for storrage app thread  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       ( 2* UX_HOST_APP_THREAD_STACK_SIZE), TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the storage applicative process thread */
  if (tx_thread_create(&msc_app_thread, "MSC App thread", msc_process_thread_entry,
                       0, pointer, ( 2* UX_HOST_APP_THREAD_STACK_SIZE), 23, 23, 0,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create the event flags group */
  if (tx_event_flags_create(&ux_app_EventFlag, "Event Flag") != TX_SUCCESS)
  {
    return TX_GROUP_ERROR;
  }

  /* USER CODE END MX_USBX_Host_Init1 */

  return ret;
}

/**
  * @brief  Function implementing app_ux_host_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
static VOID app_ux_host_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN app_ux_host_thread_entry */
  USBH_UsrLog("USBH: Hardware initialization...");
  USBX_APP_Host_Init();
  /* USER CODE END app_ux_host_thread_entry */
}

/**
  * @brief  ux_host_event_callback
  *         This callback is invoked to notify application of instance changes.
  * @param  event: event code.
  * @param  current_class: Pointer to class.
  * @param  current_instance: Pointer to class instance.
  * @retval status
  */
UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN ux_host_event_callback0 */
  UINT idx;
  /* USER CODE END ux_host_event_callback0 */

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* USER CODE BEGIN UX_DEVICE_INSERTION */

      USBH_UsrLog("USBH: Device connected.");


      if (current_class->ux_host_class_entry_function == ux_host_class_storage_entry)
      {
        status = UX_TOO_MANY_DEVICES;
        for (idx = 0; idx < MSC_INSTANCE; )
        {
          if (storage_instance[idx]) ++idx;
          else
          {
            storage_instance[idx] = (UX_HOST_CLASS_STORAGE*)current_instance;
            msc_index = idx;
            status = UX_SUCCESS;
            break;
          }
        }
        if (status != UX_SUCCESS)
        {
          USBH_ErrLog("USBH: Too many devices connected. Consider increasing MSC_INSTANCE.");
          return status;
        }

        USBH_UsrLog("USBH: MSC device detected:");
        USBH_UsrLog("  PID: %#x",
          (UINT)storage_instance[msc_index]->ux_host_class_storage_device->ux_device_descriptor.idProduct);
        USBH_UsrLog("  VID: %#x",
          (UINT)storage_instance[msc_index]->ux_host_class_storage_device->ux_device_descriptor.idVendor);

        storage_media = (UX_HOST_CLASS_STORAGE_MEDIA *)current_class -> ux_host_class_media;
        if (storage_media[msc_index].ux_host_class_storage_media_lun != 0)
        {
          storage_media = UX_NULL;
        }
        else
        {
          media[msc_index] = &storage_media[msc_index].ux_host_class_storage_media;
          if (storage_instance[msc_index] -> ux_host_class_storage_state ==  UX_HOST_CLASS_INSTANCE_LIVE)
          {
            // Pass the control to MSC thread:
            if (tx_event_flags_set(&ux_app_EventFlag, STORAGE_MEDIA_CONNECTED, TX_OR) != TX_SUCCESS) Error_Handler();
          }
        }
      }

      /* USER CODE END UX_DEVICE_INSERTION */

      break;

    case UX_DEVICE_REMOVAL:

      /* USER CODE BEGIN UX_DEVICE_REMOVAL */

      USBH_UsrLog("USBH: Device disconnected.");

      for (msc_index = 0; msc_index < MSC_INSTANCE; msc_index++)
      {
        if ((VOID*)storage_instance[msc_index] == current_instance)
        {
          storage_instance[msc_index] = UX_NULL;
          storage_media = UX_NULL;
          media[msc_index] = UX_NULL;
          USBH_UsrLog("USBH: MSC device unregistered.");
          if (tx_event_flags_set(&ux_app_EventFlag, STORAGE_MEDIA_DISCONNECTED, TX_OR) != TX_SUCCESS)
            Error_Handler();
          return status;
        }
      }

      /* USER CODE END UX_DEVICE_REMOVAL */

      break;

    case UX_DEVICE_CONNECTION:

      /* USER CODE BEGIN UX_DEVICE_CONNECTION */
      /* USER CODE END UX_DEVICE_CONNECTION */

      break;

    case UX_DEVICE_DISCONNECTION:

      /* USER CODE BEGIN UX_DEVICE_DISCONNECTION */
      /* USER CODE END UX_DEVICE_DISCONNECTION */

      break;

    default:

      /* USER CODE BEGIN EVENT_DEFAULT */
      /* USER CODE END EVENT_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_event_callback1 */

  /* USER CODE END ux_host_event_callback1 */

  return status;
}

/**
  * @brief ux_host_error_callback
  *         This callback is invoked to notify application of error changes.
  * @param  system_level: system level parameter.
  * @param  system_context: system context code.
  * @param  error_code: error event code.
  * @retval Status
  */
VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
{
  /* USER CODE BEGIN ux_host_error_callback0 */
  UX_PARAMETER_NOT_USED(system_level);
  UX_PARAMETER_NOT_USED(system_context);
  /* USER CODE END ux_host_error_callback0 */

  switch (error_code)
  {
    case UX_DEVICE_ENUMERATION_FAILURE:

      /* USER CODE BEGIN UX_DEVICE_ENUMERATION_FAILURE */
      USBH_ErrLog("USBH: Device enumeration failure.");
      /* USER CODE END UX_DEVICE_ENUMERATION_FAILURE */

      break;

    case  UX_NO_DEVICE_CONNECTED:

      /* USER CODE BEGIN UX_NO_DEVICE_CONNECTED */
      USBH_ErrLog("USBH: No device connected.");
      /* USER CODE END UX_NO_DEVICE_CONNECTED */

      break;

    default:

      /* USER CODE BEGIN ERROR_DEFAULT */

      /* USER CODE END ERROR_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_error_callback1 */

  /* USER CODE END ux_host_error_callback1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
