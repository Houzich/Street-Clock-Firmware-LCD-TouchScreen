/**
  ******************************************************************************
  * File Name          : Date.c
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "Street_Clock.h"
#include "Date.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//used when debugging, declared in the file Street_Clock.h
#ifdef DEBUG_DATE
#undef DEBUG /* DEBUG */
#undef DEBUG_PRINT /* DEBUG_PRINT */
#define DEBUG(...)		do {debug_header("DATE: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(x)		debug_print x;
#else
#define DEBUG(...)
#define DEBUG_PRINT(...)
#endif // DEBUG_TERMINAL_SETTINGS
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


void String_To_Time (char *str, RTC_TimeTypeDef *time) {
			time->Hours = atoi(&str[0]);
			time->Minutes = atoi(&str[3]);
			time->Seconds = atoi(&str[6]);
}


bool check_time_in_interval(RTC_TimeTypeDef* start, RTC_TimeTypeDef* end, RTC_TimeTypeDef* curr) {
	if(start->Hours < end->Hours){ //Exsample: 13-17	
			if((curr->Hours > start->Hours)&&(curr->Hours < end->Hours)) return true;		
			if((curr->Hours == start->Hours)&&(curr->Minutes >= start->Minutes)) return true;
			if((curr->Hours == end->Hours)&&(curr->Minutes <= end->Minutes)) return true;		
			return false;
	}
	else if(start->Hours > end->Hours){  //Exsample: 22-5
			if(curr->Hours > start->Hours) return true;	
			if(curr->Hours < end->Hours) return true;		
			if((curr->Hours == start->Hours)&&(curr->Minutes >= start->Minutes)) return true;
			if((curr->Hours == end->Hours)&&(curr->Minutes <= end->Minutes)) return true;		
			return false;
	}
	else{  //Exsample: 22-22
		if((curr->Minutes > start->Minutes)&&(curr->Minutes < end->Minutes)) return true;
	}
	return false;
}

//#define RTC_MONTH_JANUARY              ((uint8_t)0x01U)
//#define RTC_MONTH_FEBRUARY             ((uint8_t)0x02U)
//#define RTC_MONTH_MARCH                ((uint8_t)0x03U)
//#define RTC_MONTH_APRIL                ((uint8_t)0x04U)
//#define RTC_MONTH_MAY                  ((uint8_t)0x05U)
//#define RTC_MONTH_JUNE                 ((uint8_t)0x06U)
//#define RTC_MONTH_JULY                 ((uint8_t)0x07U)
//#define RTC_MONTH_AUGUST               ((uint8_t)0x08U)
//#define RTC_MONTH_SEPTEMBER            ((uint8_t)0x09U)
//#define RTC_MONTH_OCTOBER              ((uint8_t)0x10U)
//#define RTC_MONTH_NOVEMBER             ((uint8_t)0x11U)
//#define RTC_MONTH_DECEMBER             ((uint8_t)0x12U)


bool Check_Time_Night_Illumination(RTC_TimeTypeDef* time, RTC_DateTypeDef* date) {
		RTC_TimeTypeDef start; 
		RTC_TimeTypeDef end;
	
	if(date->Month == 1){
#if (MOUNTH_1_En	== 1)
		String_To_Time(MOUNTH_1_START_TIME, &start);
		String_To_Time(MOUNTH_1_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
	
		if(date->Month == 2){
#if (MOUNTH_2_En	== 1)
		String_To_Time(MOUNTH_2_START_TIME, &start);
		String_To_Time(MOUNTH_2_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
		
		if(date->Month == 3){
#if (MOUNTH_3_En	== 1)
		String_To_Time(MOUNTH_3_START_TIME, &start);
		String_To_Time(MOUNTH_3_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
		
		if(date->Month == 4){
#if (MOUNTH_4_En	== 1)
		String_To_Time(MOUNTH_4_START_TIME, &start);
		String_To_Time(MOUNTH_4_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
	
		if(date->Month == 5){
#if (MOUNTH_5_En	== 1)
		String_To_Time(MOUNTH_5_START_TIME, &start);
		String_To_Time(MOUNTH_5_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
		
	
		if(date->Month == 6){
#if (MOUNTH_6_En	== 1)
		String_To_Time(MOUNTH_6_START_TIME, &start);
		String_To_Time(MOUNTH_6_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
	
		if(date->Month == 7){
#if (MOUNTH_7_En	== 1)
		String_To_Time(MOUNTH_7_START_TIME, &start);
		String_To_Time(MOUNTH_7_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
	
		if(date->Month == 8){
#if (MOUNTH_8_En	== 1)
		String_To_Time(MOUNTH_8_START_TIME, &start);
		String_To_Time(MOUNTH_8_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
	
		if(date->Month == 9){
#if (MOUNTH_9_En	== 1)
		String_To_Time(MOUNTH_9_START_TIME, &start);
		String_To_Time(MOUNTH_9_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
		
		if(date->Month == 10){
#if (MOUNTH_10_En	== 1)
		String_To_Time(MOUNTH_10_START_TIME, &start);
		String_To_Time(MOUNTH_10_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
	
	
		if(date->Month == 11){
#if (MOUNTH_11_En	== 1)
		String_To_Time(MOUNTH_11_START_TIME, &start);
		String_To_Time(MOUNTH_11_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
		
		if(date->Month == 12){
#if (MOUNTH_12_En	== 1)
		String_To_Time(MOUNTH_12_START_TIME, &start);
		String_To_Time(MOUNTH_12_END_TIME, &end);
		return check_time_in_interval(&start, &end, time);
#endif	
	}
	
	return false;
}


		



