/**
  ******************************************************************************
  * File Name          : RTC.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H
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
/* Exported types ------------------------------------------------------------*/
/**
* @brief  RTC acceptor status
*/
typedef enum __RTCMsgIDEnumTypeDef{
  RTCMsgID_RTC_STRING_TIME_TO_LCD = 0x01,
  RTCMsgID_SIGNAL_9_21 = 0x02,
  RTCMsgID_SIGNAL_2 = 0x03,
  RTCMsgID_SIGNAL_3 = 0x04,
  RTCMsgID_RTC_STRING_UTC_TO_LCD = 0x05,
	RTCMsgID_RTC_STRING_MODE_TO_LCD = 0x06,
	RTCMsgID_SIGNAL_LED = 0x07,
}RTCMsgID;

#define RTC_MSG_OUT_QUEUE_OBJECTS 30
typedef struct {                                // object data type
  RTCMsgID id;
  char *str_lcd;
  int len;
} RTCMsgOutQueueTypeDef;
/**
  * @brief  Terminal main structure
  */
typedef struct __STREETCLOCK_HandleRTCTypeDef
{
	void(*init)(void);
  osMessageQueueId_t queue_out;
  RTCMsgOutQueueTypeDef msg_out;

} STREETCLOCK_HandleRTCTypeDef;

/* Exported constants --------------------------------------------------------*/
#define RTC_CLOCK_SOURCE_LSE 
//#define RTC_CLOCK_SOURCE_LSI

#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV    0x7F
#define RTC_SYNCH_PREDIV     0x0130    
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void MX_RTC_Init(void);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc);
void RTC_Alarm_IRQHandler(void);
int32_t TimeDate_To_String(char *str);
void SetUTC(int utc);
int GetUTC(void);
void Set_RTC_Time(RTC_TimeTypeDef *time);
void Set_RTC_Date(RTC_DateTypeDef *sDate);
void GetRTCTime(RTC_TimeTypeDef *time);
void GetRTCTimeDate(RTC_TimeTypeDef *time, RTC_DateTypeDef *date);
int32_t TimeStruct_To_ClockHandsString(RTC_TimeTypeDef *time, char *str);
#ifdef __cplusplus
}
#endif
#endif /*__RTC_H*/



