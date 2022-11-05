/**
******************************************************************************
* File Name          : Date.h
******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATE_H
#define __DATE_H
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
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------


// <e> MOUNTH 1
#define MOUNTH_1_En	1
#if (MOUNTH_1_En	== 1)
// <s.8> month 1 start time
#define MOUNTH_1_START_TIME	"00:00:00"
// <s.8> month 1 end time
#define MOUNTH_1_END_TIME		"00:00:00"
#else
#define MOUNTH_1_START_TIME	"00:00:00"
#define MOUNTH_1_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 2
#define MOUNTH_2_En	1
#if (MOUNTH_2_En	== 1)
// <s.8> month 2 start time
#define MOUNTH_2_START_TIME	"00:00:00"
// <s.8> month 2 end time
#define MOUNTH_2_END_TIME		"00:00:00"
#else
#define MOUNTH_2_START_TIME	"00:00:00"
#define MOUNTH_2_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 3
#define MOUNTH_3_En	1
#if (MOUNTH_3_En	== 1)
// <s.8> month 3 start time
#define MOUNTH_3_START_TIME	"00:00:00"
// <s.8> month 3 end time
#define MOUNTH_3_END_TIME		"00:00:00"
#else
#define MOUNTH_3_START_TIME	"00:00:00"
#define MOUNTH_3_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 4
#define MOUNTH_4_En	1
#if (MOUNTH_4_En	== 1)
// <s.8> month 4 start time
#define MOUNTH_4_START_TIME	"00:00:00"
// <s.8> month 4 end time
#define MOUNTH_4_END_TIME		"00:00:00"
#else
#define MOUNTH_4_START_TIME	"00:00:00"
#define MOUNTH_4_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 5
#define MOUNTH_5_En	1
#if (MOUNTH_5_En	== 1)
// <s.8> month 5 start time
#define MOUNTH_5_START_TIME	"00:00:00"
// <s.8> month 5 end time
#define MOUNTH_5_END_TIME		"00:00:00"
#else
#define MOUNTH_5_START_TIME	"00:00:00"
#define MOUNTH_5_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 6
#define MOUNTH_6_En	1
#if (MOUNTH_6_En	== 1)
// <s.8> month 6 start time
#define MOUNTH_6_START_TIME	"00:00:00"
// <s.8> month 6 end time
#define MOUNTH_6_END_TIME		"00:00:00"
#else
#define MOUNTH_6_START_TIME	"00:00:00"
#define MOUNTH_6_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 7
#define MOUNTH_7_En	1
#if (MOUNTH_7_En	== 1)
// <s.8> month 7 start time
#define MOUNTH_7_START_TIME	"00:00:00"
// <s.8> month 7 end time
#define MOUNTH_7_END_TIME		"00:00:00"
#else
#define MOUNTH_7_START_TIME	"00:00:00"
#define MOUNTH_7_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 8
#define MOUNTH_8_En	1
#if (MOUNTH_8_En	== 1)
// <s.8> month 8 start time
#define MOUNTH_8_START_TIME	"00:00:00"
// <s.8> month 8 end time
#define MOUNTH_8_END_TIME		"00:00:00"
#else
#define MOUNTH_8_START_TIME	"00:00:00"
#define MOUNTH_8_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 9
#define MOUNTH_9_En	1
#if (MOUNTH_9_En	== 1)
// <s.9> month 9 start time
#define MOUNTH_9_START_TIME	"00:00:00"
// <s.9> month 9 end time
#define MOUNTH_9_END_TIME		"00:00:00"
#else
#define MOUNTH_9_START_TIME	"00:00:00"
#define MOUNTH_9_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 10
#define MOUNTH_10_En	1
#if (MOUNTH_10_En	== 1)
// <s.10> month 10 start time
#define MOUNTH_10_START_TIME	"00:00:00"
// <s.10> month 10 end time
#define MOUNTH_10_END_TIME		"00:00:00"
#else
#define MOUNTH_10_START_TIME	"00:00:00"
#define MOUNTH_10_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 11
#define MOUNTH_11_En	1
#if (MOUNTH_11_En	== 1)
// <s.11> month 11 start time
#define MOUNTH_11_START_TIME	"00:00:00"
// <s.11> month 11 end time
#define MOUNTH_11_END_TIME		"00:00:00"
#else
#define MOUNTH_11_START_TIME	"00:00:00"
#define MOUNTH_11_END_TIME		"00:00:00"
#endif
//</e>

// <e> MOUNTH 12
#define MOUNTH_12_En	1
#if (MOUNTH_12_En	== 1)
// <s.12> month 12 start time
#define MOUNTH_12_START_TIME	"00:00:00"
// <s.12> month 12 end time
#define MOUNTH_12_END_TIME		"00:00:00"
#else
#define MOUNTH_12_START_TIME	"00:00:00"
#define MOUNTH_12_END_TIME		"00:00:00"
#endif
//</e>



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void String_To_Time (char *str, RTC_TimeTypeDef *time);
bool check_time_in_interval(RTC_TimeTypeDef* start, RTC_TimeTypeDef* end, RTC_TimeTypeDef* curr);
bool Check_Time_Night_Illumination(RTC_TimeTypeDef* time, RTC_DateTypeDef* date);
#ifdef __cplusplus
}
#endif

#endif /*__DATE_H*/
