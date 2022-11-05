/**
  ******************************************************************************
  * File Name          : LCD.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_LCD
#define __BOARD_LCD
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#ifdef STM32F7
#include "stm32f7xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h" 
#endif
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
/* Exported constants --------------------------------------------------------*/
typedef enum __LCDMsgIDEnumTypeDef{
	LCDMsgID_STR_RTC = 0x01,
	LCDMsgID_STR_CLOCK = 0x02,
  LCDMsgID_STR_STATE = 0x03,
  LCDMsgID_STR_NOTE = 0x04,
  LCDMsgID_STR_UTC = 0x05,
	LCDMsgID_STR_TEST_MODE = 0x06,
}LCDMsgID;
   
#define LCD_MSG_IN_QUEUE_OBJECTS 40
typedef struct {                                // object data type
  LCDMsgID id;
  char* string;
  int32_t len;
} LCDMsgInQueueTypeDef;

//#define LCD_MSG_OUT_QUEUE_OBJECTS 4
//typedef struct {                                // object data type
//  LCDMsgID id;
//} LCDMsgOutQueueTypeDef;

//#define LCD_MSG_QUEUE_OBJECTS 4
//typedef struct {                                // object data type
//  LCDMsgID id;
//} LCDMsgQueueTypeDef;

/**
  * @brief  Terminal main structure
  */
typedef struct __STREETCLOCK_HandleLCDTypeDef
{
	void(*init)(void);
  osMessageQueueId_t queue_in;
//  osMessageQueueId_t queue_out;
//  osMessageQueueId_t queue;
  
  LCDMsgInQueueTypeDef msg_in;
//  LCDMsgOutQueueTypeDef msg_out;
//  LCDMsgQueueTypeDef msg;
} STREETCLOCK_HandleLCDTypeDef;
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
__NO_RETURN void LCD_Thread (void *argument);
#ifdef __cplusplus
}
#endif
#endif /*__BOARD_LCD*/
