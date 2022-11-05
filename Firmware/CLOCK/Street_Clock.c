/**
  ******************************************************************************
  * File Name          : Street_Clock.c
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "Street_Clock.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//used when debugging, declared in the file Street_Clock.h
#ifdef DEBUG_CORE
#undef DEBUG /* DEBUG */
#undef DEBUG_PRINT /* DEBUG_PRINT */
#define DEBUG(...)		do {debug_header("CORE: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(x)		debug_print x;
#else
#define DEBUG(...)
#define DEBUG_PRINT(...)
#endif // DEBUG_TERMINAL_SETTINGS

STREETCLOCK_HandleTypeDef streetclock = {
	Street_Clock_Init,
};
extern STREETCLOCK_HandleButtonsTypeDef buttons;
extern STREETCLOCK_HandleSettingsTypeDef sett;
extern STREETCLOCK_HandleLCDTypeDef lcd;
extern STREETCLOCK_HandleRTCTypeDef rtc;
extern STREETCLOCK_HandleClockTypeDef clock;
extern STREETCLOCK_HandleGPSTypeDef gps;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#define STREETCLOCK_STK_SZ (1024*2)
uint64_t streetclock_stk[STREETCLOCK_STK_SZ / 8];
const osThreadAttr_t streetclock_attr = {
  .stack_mem  = &streetclock_stk[0],
  .stack_size = sizeof(streetclock_stk)
};
osThreadId_t StreetClock_TaskHandle = NULL;
/*###############################################################*/
/*###############################################################* Street_Clock_Init -->*/
/*###############################################################*/
void Street_Clock_Init(void)
{
  streetclock.stop_clock = false;
  
	sett.queue_in = osMessageQueueNew(SETTINGS_MSG_IN_QUEUE_OBJECTS, sizeof(sett.msg_in), NULL);
	sett.queue_out = osMessageQueueNew(SETTINGS_MSG_OUT_QUEUE_OBJECTS, sizeof(sett.msg_out), NULL);
	sett.queue = osMessageQueueNew(SETTINGS_MSG_QUEUE_OBJECTS, sizeof(sett.msg), NULL);
  
  buttons.queue_in = osMessageQueueNew(BUTTONS_MSG_IN_QUEUE_OBJECTS, sizeof(buttons.msg_in), NULL);
	buttons.queue_out = osMessageQueueNew(BUTTONS_MSG_OUT_QUEUE_OBJECTS, sizeof(buttons.msg_out), NULL);
	buttons.queue = osMessageQueueNew(BUTTONS_MSG_QUEUE_OBJECTS, sizeof(buttons.msg), NULL);
  
	lcd.queue_in = osMessageQueueNew(LCD_MSG_IN_QUEUE_OBJECTS, sizeof(lcd.msg_in), NULL);

  clock.queue_in = osMessageQueueNew(CLOCK_MSG_IN_QUEUE_OBJECTS, sizeof(clock.msg_in), NULL);
	clock.queue_out = osMessageQueueNew(CLOCK_MSG_OUT_QUEUE_OBJECTS, sizeof(clock.msg_out), NULL);

  gps.queue_in = osMessageQueueNew(GPS_MSG_IN_QUEUE_OBJECTS, sizeof(gps.msg_in), NULL);
	gps.queue_out = osMessageQueueNew(GPS_MSG_OUT_QUEUE_OBJECTS, sizeof(gps.msg_out), NULL);
	gps.queue = osMessageQueueNew(GPS_MSG_QUEUE_OBJECTS, sizeof(gps.msg), NULL);

  
  clock.init();
  lcd.init();
  buttons.init();
  sett.init();
  gps.init();
  //rtc.init();
  osThreadNew(Street_Clock_Thread, NULL, &streetclock_attr);
}

extern RTC_HandleTypeDef RtcHandle;
/*###############################################################*/
/*###############################################################* Street_Clock_Thread -->*/
/*###############################################################*/
__NO_RETURN void Street_Clock_Thread (void *argument) {
  osStatus_t status;
	
	
//			  RTC_DateTypeDef sDate;
//  RTC_TimeTypeDef sTime;

//  /*##-1- Configure the Date #################################################*/
//  /* Set Date: Tuesday October 14th 2019 */
//  sDate.Year = 19;
//  sDate.Month = 12;
//  sDate.Date = 14;
//  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
//  
//  if(HAL_RTC_SetDate(&RtcHandle,&sDate,RTC_FORMAT_BIN) != HAL_OK)
//  {
//    /* Initialization Error */
//    DEBUG("ERROR HAL_RTC_SetDate!!!\n");
//    Error_Handler(); 
//  } 
//  
//  /*##-2- Configure the Time #################################################*/
//  /* Set Time: 00:00:00 */
//  sTime.Hours = 0x15;
//  sTime.Minutes = 0x00;
//  sTime.Seconds = 0x00;
//  sTime.TimeFormat = RTC_HOURFORMAT_24;
//  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//  
//  if(HAL_RTC_SetTime(&RtcHandle,&sTime,RTC_FORMAT_BCD) != HAL_OK)
//  {
//    /* Initialization Error */
//    DEBUG("ERROR HAL_RTC_SetTime!!!\n");
//    Error_Handler(); 
//  }
	
	
	
	while (1) {
    osDelay(1);    
    //Buttons - Settings
		status = osMessageQueueGet(buttons.queue_out, &buttons.msg_out, NULL, 0);   // wait for message
		if (status == osOK){
      sett.msg_in.id = SettingsMsgID_BUTTON_EVNT;
      sett.msg_in.button_msg_id = buttons.msg_out.id;
      osMessageQueuePut(sett.queue_in, &sett.msg_in, NULL, osWaitForever);
      osThreadYield();
      while(1){
        osMessageQueueGet(sett.queue_out, &sett.msg_out, NULL, osWaitForever);
        if(sett.msg_out.id == SettingsMsgID_BUTTON_EVNT_PROCESSED_OK) break;
      }
      buttons.msg_in.id = ButtonsMsgID_BUTTON_PROCESSED_OK;
      osMessageQueuePut(buttons.queue_in, &buttons.msg_in, NULL, osWaitForever);     
      osThreadYield();
    }

    
     //Settings - LCD and Core
		status = osMessageQueueGet(sett.queue_out, &sett.msg_out, NULL, 0);   // wait for message
		if (status == osOK){
      if(sett.msg_out.id == SettingsMsgID_STRING_RTC_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_RTC;
        lcd.msg_in.string = sett.msg_out.str_lcd;
        lcd.msg_in.len = sett.msg_out.len;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        osThreadYield();
      }
      if(sett.msg_out.id == SettingsMsgID_STRING_CLOCK_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_CLOCK;
        lcd.msg_in.string = sett.msg_out.str_lcd;
        lcd.msg_in.len = sett.msg_out.len;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        osThreadYield();
      }
      if(sett.msg_out.id == SettingsMsgID_STRING_UTC_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_UTC;
        lcd.msg_in.string = sett.msg_out.str_lcd;
        lcd.msg_in.len = sett.msg_out.len;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        osThreadYield();
      }
      if(sett.msg_out.id == SettingsMsgID_EXIT_TEST_MODE){
        lcd.msg_in.id = LCDMsgID_STR_TEST_MODE;
        lcd.msg_in.string = sett.msg_out.str_lcd;
        lcd.msg_in.len = sett.msg_out.len;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);
     }	
      if(sett.msg_out.id == SettingsMsgID_STRING_STATE_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_STATE;
        lcd.msg_in.string = sett.msg_out.str_lcd;
        lcd.msg_in.len = sett.msg_out.len;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);
        //тут не надо, они обе вместе идут и не должны отвлекать ядро
        //osThreadYield();
      }
      
      if(sett.msg_out.id == SettingsMsgID_STRING_NOTE_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_NOTE;
        lcd.msg_in.string = sett.msg_out.str_lcd;
        lcd.msg_in.len = sett.msg_out.len;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        //тут не надо, они обе вместе идут и не должны отвлекать ядро
        //osThreadYield();
      }
      //Settings - Clock
      if(sett.msg_out.id == SettingsMsgID_STOP_CLOCK){
        streetclock.stop_clock = true;
        clock.msg_in.id = ClockMsgID_STOP;
        DEBUG("Send ClockMsgID_STOP to Clock\n");
        osMessageQueuePut(clock.queue_in, &clock.msg_in, NULL, osWaitForever);        
        osThreadYield();
      }
      if(sett.msg_out.id == SettingsMsgID_START_CLOCK){
        streetclock.stop_clock = false;
        clock.msg_in.id = ClockMsgID_START;
        DEBUG("Send ClockMsgID_START to Clock\n");
        osMessageQueuePut(clock.queue_in, &clock.msg_in, NULL, osWaitForever);        
        osThreadYield();
     }
				 
    }
     //RTC - LCD   
		status = osMessageQueueGet(rtc.queue_out, &rtc.msg_out, NULL, 0);   // wait for message
		if (status == osOK){
      if (sett.status != SettingsStatus_SET_RTC){
        if(rtc.msg_out.id == RTCMsgID_RTC_STRING_TIME_TO_LCD){
          lcd.msg_in.id = LCDMsgID_STR_RTC;
          lcd.msg_in.string = rtc.msg_out.str_lcd;
          lcd.msg_in.len = rtc.msg_out.len;
          osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        }          
        if(rtc.msg_out.id == RTCMsgID_SIGNAL_9_21){
          osThreadFlagsSet(Signal_1_TaskHandle, 0x00000001U);
        }
				if(rtc.msg_out.id == RTCMsgID_SIGNAL_LED){
					osThreadFlagsSet(Signal_LED_TaskHandle, 0x00000001U);
        }			
        if(rtc.msg_out.id == RTCMsgID_SIGNAL_2){
          osThreadFlagsSet(Signal_2_TaskHandle, 0x00000001U);
        }
				if(rtc.msg_out.id == RTCMsgID_SIGNAL_3){
          osThreadFlagsSet(Signal_3_TaskHandle, 0x00000001U);
        }
      }
      if (sett.status != SettingsStatus_SET_UTC){      
        if(rtc.msg_out.id == RTCMsgID_RTC_STRING_UTC_TO_LCD){
          lcd.msg_in.id = LCDMsgID_STR_UTC;
          lcd.msg_in.string = rtc.msg_out.str_lcd;
          lcd.msg_in.len = rtc.msg_out.len;
          osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);               
        }  
      }  
			if(rtc.msg_out.id == RTCMsgID_RTC_STRING_MODE_TO_LCD){
				lcd.msg_in.id = LCDMsgID_STR_TEST_MODE;
				lcd.msg_in.string = rtc.msg_out.str_lcd;
				lcd.msg_in.len = rtc.msg_out.len;
				osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);               
			}  

      osThreadYield();
    }

       
		status = osMessageQueueGet(clock.queue_out, &clock.msg_out, NULL, 0);   // wait for message
		if (status == osOK){
      //Clock - LCD
        lcd.msg_in.string = clock.msg_out.str_lcd;
        lcd.msg_in.len = clock.msg_out.len;      
      if(clock.msg_out.id == ClockMsgID_CLOCK_HANDS_POSITION_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_CLOCK;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        osThreadYield();
      }
      if(clock.msg_out.id == ClockMsgID_STRING_STATE_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_STATE;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        osThreadYield();
      }
      if(clock.msg_out.id == ClockMsgID_STRING_NOTE_TO_LCD){
        lcd.msg_in.id = LCDMsgID_STR_NOTE;
        osMessageQueuePut(lcd.queue_in, &lcd.msg_in, NULL, 0);        
        osThreadYield();
      }
      //Clock - Settings
      if(clock.msg_out.id == ClockMsgID_STOP_OK){
        sett.msg_in.id = SettingsMsgID_STOP_CLOCK_OK;
        DEBUG("SEND SettingsMsgID_STOP_CLOCK_OK to Settings\n");
        osMessageQueuePut(sett.queue_in, &sett.msg_in, NULL, osWaitForever);        
        osThreadYield();
      }                       
    }
 	
	 RTC_TimeTypeDef time;	
   if(!streetclock.stop_clock){
    GetRTCTime(&time);	
    //Core - Clock
    #ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT      
    #else
    if(time.Hours == 0)  time.Hours = 12;
    if(time.Hours > 12) time.Hours -= 12;
    #endif 
    if((time.Hours != clock.curr_state_time.Hours) ||
      (time.Minutes != clock.curr_state_time.Minutes)){
        if(clock.status == ClockStatus_READY){
          clock.msg_in.id = ClockMsgID_MOVE_CLOCK_HANDS;     
          memcpy(&clock.msg_in.time, &time, sizeof(time));
          osMessageQueuePut(clock.queue_in, &clock.msg_in, NULL, osWaitForever);                  
          osThreadYield();
          osMessageQueueGet(clock.queue_out, &clock.msg_out, NULL, osWaitForever);          
        } 
      } 
    }
      


		
		
		//Night Light
#if (HIGHT_LIGHT_Pin_En	== 1)
		RTC_DateTypeDef date;
		GetRTCTimeDate(&time, &date);
		if(Check_Time_Night_Illumination(&time, &date)){
			HAL_GPIO_WritePin(HIGHT_LIGHT_GPIO_Port, HIGHT_LIGHT_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(HIGHT_LIGHT_GPIO_Port, HIGHT_LIGHT_Pin, GPIO_PIN_RESET);		
		}
#endif
		
		
	}
}




_ARMABI int header_print(const char* str, const char* format, ...) {
	int ret;
	ret = printf("%s", str);
	va_list arg;
	va_start(arg, format);
	ret = vprintf(format, arg);
	va_end(arg);
	return ret;
}

_ARMABI int print(const char* format, ...) {
	int ret;
	va_list arg;
	va_start(arg, format);
	ret = vprintf(format, arg);
	va_end(arg);
	return ret;
}

uint32_t Check_CMSIS_Flags_Functions_Error_Codes(uint32_t flags){
  uint32_t ret = flags;
  switch (flags)
  {
    case  osFlagsErrorUnknown:
      DEBUG("osFlagsErrorUnknown: unspecified error\n");
      break;
    case  osFlagsErrorTimeout:
      //DEBUG("osFlagsErrorTimeout: awaited flags have not been set in the given time\n");
      break;
    case  osFlagsErrorResource:
      //DEBUG("osFlagsErrorResource: awaited flags have not been set when no timeout was specified\n");
      break;
    case  osFlagsErrorParameter:
      DEBUG("osFlagsErrorParameter: parameter ef_id does not identify a valid event flags object or flags has\n");
      break;
    default:
      ret = 0;
      break;
  }
return ret;
}

// OS Error Callback function
uint32_t osRtxErrorNotify (uint32_t code, void *object_id) {
  (void)object_id;

  switch (code) {
    case osRtxErrorStackUnderflow:
      // Stack overflow detected for thread (thread_id=object_id)
			DEBUG("ERROR RTX OS: Stack overflow detected for thread (thread_id = %d)\n", (uint32_t)object_id);
      break;
    case osRtxErrorISRQueueOverflow:
      // ISR Queue overflow detected when inserting object (object_id)
			DEBUG("ERROR RTX OS: ISR Queue overflow detected when inserting object (object_id = %d)\n", (uint32_t)object_id);
      break;
    case osRtxErrorTimerQueueOverflow:
      // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
			DEBUG("ERROR RTX OS: User Timer Callback Queue overflow detected for timer (timer_id = %d)\n", (uint32_t)object_id);
      break;
    case osRtxErrorClibSpace:
      // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
			DEBUG("ERROR RTX OS: Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM\n");
      break;
    case osRtxErrorClibMutex:
      // Standard C/C++ library mutex initialization failed
			DEBUG("ERROR RTX OS: Standard C/C++ library mutex initialization failed\n");
      break;
    default:
      // Reserved
      break;
  }
	//assert_param(0);
  for (;;) {}
//return 0U;
}




