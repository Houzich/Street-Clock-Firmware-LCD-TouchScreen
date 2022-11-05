/**
  ******************************************************************************
  * File Name          : Buttons.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTONS_H
#define __BUTTONS_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#ifdef STM32F7
#include "stm32f7xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h" 
#endif
/* Exported constants --------------------------------------------------------*/
/**
* @brief  Buttons
*/
typedef enum __ButtonsNumEnumTypeDef{
	Buttons_UP = 1 << 5,
	Buttons_DOWN = 1 << 6,
	Buttons_LEFT = 1 << 7,
	Buttons_RIGHT = 1 << 8,
	Buttons_SAVE = 1 << 9,
	Buttons_SET_RTC = 1 << 10,
	Buttons_SET_CLOCK = 1 << 11,
	Buttons_RETURN = 1 << 12,
  Buttons_SET_UTC_DOWN = 1 << 13,
  Buttons_SET_UTC_UP = 1 << 14,
}Buttons;
/**
* @brief  Buttons message
*/
typedef enum __ButtonsMsgIDEnumTypeDef{
	ButtonsMsgID_UP_CLICKED = 1 << 5,
	ButtonsMsgID_DOWN_CLICKED = 1 << 6,
	ButtonsMsgID_LEFT_CLICKED = 1 << 7,
	ButtonsMsgID_RIGHT_CLICKED = 1 << 8,
	ButtonsMsgID_SAVE_CLICKED = 1 << 9,
	ButtonsMsgID_SET_RTC_CLICKED = 1 << 10,
	ButtonsMsgID_SET_CLOCK_CLICKED = 1 << 11,
	ButtonsMsgID_RETURN_CLICKED = 1 << 12,
  ButtonsMsgID_BUTTON_PROCESSED_OK = 1 << 20,
  ButtonsMsgID_SET_UTC_CLICKED = 1 << 21,
}ButtonsMsgID;
/**
* @brief  Buttons status
*/
typedef enum __ButtonsStatusEnumTypeDef{
	ButtonsStatus_BTN_READY = 0x00,
}ButtonsStatus;
/**
* @brief  Buttons error
*/
typedef enum __ButtonsErrorEnumTypeDef{
	ButtonsError_BTN_OK = 0x00,
}ButtonsError;


#define BUTTONS_MSG_IN_QUEUE_OBJECTS 4
typedef struct {                                // object data type
  ButtonsMsgID id;
} ButtonsMsgInQueueTypeDef;

#define BUTTONS_MSG_OUT_QUEUE_OBJECTS 20
typedef struct {                                // object data type
  ButtonsMsgID id;
} ButtonsMsgOutQueueTypeDef;

#define BUTTONS_MSG_QUEUE_OBJECTS 4
typedef struct {                                // object data type
  ButtonsMsgID id;
} ButtonsMsgQueueTypeDef;

/**
  * @brief  Terminal main structure
  */
typedef struct __STREETCLOCK_HandleButtonsTypeDef
{
	void(*init)(void);
	ButtonsStatus status;
	ButtonsError error;
  
  osMessageQueueId_t queue_in;
  osMessageQueueId_t queue_out;
  osMessageQueueId_t queue;
  
  ButtonsMsgInQueueTypeDef msg_in;
  ButtonsMsgOutQueueTypeDef msg_out;
  ButtonsMsgQueueTypeDef msg;
  
  bool enable_button_up;
	bool enable_button_down;
	bool enable_button_left;
	bool enable_button_right;
	bool enable_button_ok;
	bool enable_button_settings;
	bool enable_button_return;
  int button_utc_count;
  bool button_utc_delay_start;
} STREETCLOCK_HandleButtonsTypeDef;



/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Callback_Buttons(Buttons num);

#ifdef __cplusplus
}
#endif
#endif /*__BUTTONS_H*/
