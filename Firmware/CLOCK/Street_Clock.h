/**
******************************************************************************
* File Name          : Street_Clock.h
******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STREET_CLOCK_H
#define __STREET_CLOCK_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef STM32F7
#include "stm32f7xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h" 
#endif   
   
#include "Board_LCD.h"
#include "RTC.h"
#include "FLASH.h"

#ifdef STM32F7
#include "Clock_F7.h"
#elif defined(STM32F4)
#include "Clock_F4.h"
#endif 

#include "Settings.h"
#include "Buttons.h"
#include "GPS.h"
#include "Date.h"
/* Exported types ------------------------------------------------------------*/
/*###############################################################*/
/*###############################################################* DEBUG -->*/
/*###############################################################*/
//#define DEBUG_RTC
//#define DEBUG_SETTINGS
//#define DEBUG_BUTTONS
//#define DEBUG_CORE
//#define DEBUG_GPS
//#define DEBUG_FLASH
//#define STREET_CLOCK_RTC_12_HOUR_FORMAT
/* Exported constants --------------------------------------------------------*/
/**
  * @brief  Terminal main structure
  */
typedef struct __STREETCLOCK_HandleTypeDef
{
	void(*init)(void);
  bool stop_clock;
} STREETCLOCK_HandleTypeDef;
/* Exported Macros -----------------------------------------------------------*/
#ifdef STREET_CLOCK_USE_LCD_FOR_DEBUG
   
#else
_ARMABI int header_print(const char* str, const char* format, ...);
_ARMABI int print(const char *format, ...);
#define debug_header header_print
#define debug_print	printf
#endif // STREET_CLOCK_USE_LCD_FOR_DEBUG
   
/* Exported functions --------------------------------------------------------*/
extern void Error_Handler(void);
uint32_t Check_CMSIS_Flags_Functions_Error_Codes(uint32_t flags);  
__NO_RETURN void Street_Clock_Thread (void *argument);
void Street_Clock_Init(void);
#ifdef __cplusplus
}
#endif

#endif /*__STREET_CLOCK_H*/
