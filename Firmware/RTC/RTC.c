/**
  ******************************************************************************
  * File Name          : RTC.c
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "Street_Clock.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#ifdef DEBUG_RTC
#undef DEBUG
#undef DEBUG_PRINTF
#undef DEBUG_PUTCHAR
#define DEBUG(...)		do {debug_header("RTC: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(...)		    do {debug_print(__VA_ARGS__);} while (0)
#else
#define DEBUG(...)
#define DEBUG_PRINT(...)
#endif // DEBUG_FLASH
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;
STREETCLOCK_HandleRTCTypeDef rtc;
extern STREETCLOCK_HandleSettingsTypeDef sett;
extern STREETCLOCK_HandleStoreSettingsTypeDef sett_store;
extern STREETCLOCK_HandleButtonsTypeDef buttons;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*###############################################################*/
/*###############################################################* GetUTC -->*/
/*###############################################################*/
int GetUTC(void)
{ 
  return sett_store.UTC;
}
/*###############################################################*/
/*###############################################################* SetUTC -->*/
/*###############################################################*/
void SetUTC(int utc)
{ 
  sett_store.UTC = utc;
//  static RTC_TimeTypeDef time;
//  GetRTCTime(&time);
//  int8_t hours = time.Hours;
//  hours += rtc.UTC;
//#ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT
//#else
//    if (hours > 23){ 
//      hours -= 24;
//    }
//    if (hours < 0){ 
//      hours += 24;
//    }  
//    time.Hours = hours;
//#endif
//    Set_RTC_Time(&time);
    FLASH_Save_Settings();
}
/*###############################################################*/
/*###############################################################* Set_RTC_Time -->*/
/*###############################################################*/
void Set_RTC_Time(RTC_TimeTypeDef *time)
{
  static RTC_TimeTypeDef sTime;
  memcpy(&sTime, time, sizeof(RTC_TimeTypeDef));
  int8_t hours = sTime.Hours;
  hours -= sett_store.UTC;
#ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT
#else
    if (hours > 23){ 
      hours -= 24;
    }
    if (hours < 0){ 
      hours += 24;
    }  
    sTime.Hours = hours;
#endif
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&RtcHandle, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
      DEBUG("ERROR HAL_RTC_SetTime!!!\n");
      //Error_Handler();
    }
}

/*###############################################################*/
/*###############################################################* Set_RTC_Date -->*/
/*###############################################################*/
void Set_RTC_Date(RTC_DateTypeDef *sDate)
{
		sDate->WeekDay = RTC_WEEKDAY_MONDAY;
    if(HAL_RTC_SetDate(&RtcHandle,sDate,RTC_FORMAT_BIN) != HAL_OK)
    {
      DEBUG("ERROR HAL_RTC_SetTime!!!\n");
      //Error_Handler();
    }
}


/*###############################################################*/
/*###############################################################* Synchronization_Time -->*/
/*###############################################################*/
void Synchronization_Time(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
			RTC_TimeTypeDef curr_time = {0};
			RTC_DateTypeDef curr_date = {0};
	
			GetRTCTimeDate(&curr_time, &curr_date);	
			if(
				(curr_time.Seconds != time->Seconds)
				||(curr_time.Minutes != time->Minutes)			
				||(curr_time.Hours != time->Hours)			
				||(curr_date.Date != date->Date)			
				||(curr_date.Month != date->Month)			
				||(curr_date.Year != date->Year)			
			)			

			time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
			time->StoreOperation = RTC_STOREOPERATION_RESET;
			if (HAL_RTC_SetTime(hrtc, time, RTC_FORMAT_BIN) != HAL_OK)
			{
        DEBUG("ERROR HAL_RTC_SetTime!!!\n");
        Error_Handler();
			}
			date->WeekDay = RTC_WEEKDAY_MONDAY;
			if (HAL_RTC_SetDate(hrtc, date, RTC_FORMAT_BIN) != HAL_OK)
			{
        DEBUG("ERROR HAL_RTC_SetDate!!!\n");
        Error_Handler();
			}		
}
/*###############################################################*/
/*###############################################################* RTC_CalendarShow -->*/
/*###############################################################*/
/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  GetRTCTimeDate(&stimestructureget, &sdatestructureget);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "%2d:%2d:%2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char *)showdate, "%2d-%2d-%2d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
}
/*###############################################################*/
/*###############################################################* TameDate_To_String -->*/
/*###############################################################*/
int32_t Time_To_String(char *str, int max_len)
{
      int ret;
			static RTC_TimeTypeDef time;	
      GetRTCTime(&time);
//      if(time.TimeFormat == RTC_HOURFORMAT12_AM)
//        ret = snprintf(str, max_len, "%.2d:%.2d:%.2d AM", time.Hours,time.Minutes,time.Seconds);
//      else
//        ret = snprintf(str, max_len, "%.2d:%.2d:%.2d PM", time.Hours,time.Minutes,time.Seconds);        

      ret = snprintf(str, max_len, "%.2d:%.2d:%.2d", time.Hours,time.Minutes,time.Seconds);        
  
      
      return ret;
}
/*###############################################################*/
/*###############################################################* TameDate_To_String -->*/
/*###############################################################*/
int32_t UTC_To_String(char *str, int max_len)
{
      int ret;
      if(sett_store.UTC >= 0) ret = snprintf(str, 4, "+%d", sett_store.UTC);
      else ret = snprintf(str, 4, "%d", sett_store.UTC);            
      return ret;
}
/*###############################################################*/
/*###############################################################* TimeStruct_To_String -->*/
/*###############################################################*/
int32_t TimeStruct_To_ClockHandsString(RTC_TimeTypeDef *time, char *str)
{
			int ret = snprintf(str, 9, "%.2d:%.2d", time->Hours, time->Minutes);
			return ret;
}
/*###############################################################*/
/*###############################################################* GetTime -->*/
/*###############################################################*/
void GetRTCTime(RTC_TimeTypeDef *time)
{
  RTC_DateTypeDef sDate;
	/* Get the RTC current Time */
	HAL_RTC_GetTime(&RtcHandle, time, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
//  You must call HAL_RTC_GetDate() after
//  HAL_RTC_GetTime() to unlock the values in the higher-order
//  calendar shadow registers to ensure consistency between the
//  time and date values. Reading RTC current time locks the
//  values in calendar shadow registers until Current date is read
  HAL_RTC_GetDate(&RtcHandle, &sDate, RTC_FORMAT_BIN);
  int8_t hours = time->Hours;
  hours += sett_store.UTC;
#ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT
#else
    if (hours > 23){ 
      hours -= 24;
    }
    if (hours < 0){ 
      hours += 24;
    }  
    time->Hours = hours;
#endif 
}

/*###############################################################*/
/*###############################################################* GetRTCTimeDate -->*/
/*###############################################################*/
void GetRTCTimeDate(RTC_TimeTypeDef *time, RTC_DateTypeDef *date)
{
	/* Get the RTC current Time */
	HAL_RTC_GetTime(&RtcHandle, time, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, date, RTC_FORMAT_BIN);
  int8_t hours = time->Hours;
  hours += sett_store.UTC;
#ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT
#else
    if (hours > 23){ 
      hours -= 24;
    }
    if (hours < 0){ 
      hours += 24;
    }  
    time->Hours = hours;
#endif   
}


/*###############################################################*/
/*###############################################################* TameDate_To_String -->*/
/*###############################################################*/
int32_t TimeDate_To_String(char *str)
{
			//16.05.19 19:04:35
			RTC_TimeTypeDef time;
			RTC_DateTypeDef date;
	
			GetRTCTimeDate(&time, &date);			
			int ret = sprintf(str,"%.2d/%.2d/%.2d %.2d:%.2d:%.2d; ", date.Date,date.Month,date.Year,
																			 time.Hours,time.Minutes,time.Seconds);
			return ret;
}

/**
  * @brief  Display the current time.
	* @param  time: RTC Time structure
	* @param  date: RTC Date structure
  * @retval None
  */
void debug_rtc_time(RTC_TimeTypeDef *time)
{
  /* Display time Format : hh:mm:ss */
  DEBUG("Current Time: %02d:%02d:%02d\n",time->Hours, time->Minutes, time->Seconds);
} 
/**
  * @brief  Alarm callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  static RTC_TimeTypeDef sTime = {0};
	static RTCMsgOutQueueTypeDef msg_out;
	
	//если в ознакомительном режиме, то уменьшаем кол-во секунд работы устройства
	if((sett_store.mode_rtc == MODE_RTC_TEST)||(sett_store.mode_rtc == MODE_RTC_BLOCK)){
		static char str_mode[sizeof("X\0") + 1] = {0};
		
		if(sett_store.mode_rtc == MODE_RTC_TEST){
			sett.time_test_mode--;
				//время вышло
			if(sett.time_test_mode <= 0){
				sett_store.mode_rtc = MODE_RTC_BLOCK;
				FLASH_Save_Settings();
				
				//отправляем статус на дисплей					
				msg_out.id = RTCMsgID_RTC_STRING_MODE_TO_LCD;
				msg_out.len = sprintf(str_mode, "%s", "X");
				msg_out.str_lcd = str_mode;
				osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);			
				return;
			}
		}
		//если была нажата кнопка "выход из тестового режима"
		if(sett.counter_click_to_exit_test_mode != NUM_CLICKS_EXIT_TEST_MODE){
			//уменьшаем задержку
			sett.counter_secundes_between_click--;
			//инициализируем счетчки и начинаем все сначала
			if(sett.counter_secundes_between_click == 0){
				sett.counter_secundes_between_click = SECUNDES_BETWEEN_CLICK; //счетчик секунд нажатия кнопки выхода из тестового режима
				sett.counter_click_to_exit_test_mode = NUM_CLICKS_EXIT_TEST_MODE;

				msg_out.id = RTCMsgID_RTC_STRING_MODE_TO_LCD;
				if(sett_store.mode_rtc == MODE_RTC_TEST)
					msg_out.len = sprintf(str_mode, "%s", "T");
				else
					msg_out.len = sprintf(str_mode, "%s", "X");
				msg_out.str_lcd = str_mode;
				osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);
			}
		}		
	}
	
		//исли мы не заблокировали часы
	if(sett_store.mode_rtc == MODE_RTC_BLOCK){
		return;
	}
	
	GetRTCTime(&sTime);
	//обрабатываем задержку кнопки UTC
  if(buttons.button_utc_delay_start)
      buttons.button_utc_count++;
  if((buttons.button_utc_count > 4)&&(buttons.button_utc_delay_start)){
    buttons.button_utc_delay_start = false;
    static ButtonsMsgQueueTypeDef msg;
    msg.id = ButtonsMsgID_SET_UTC_CLICKED;
    osMessageQueuePut(buttons.queue, &msg, NULL, 0);
  }

	
//#ifdef DEBUG_RTC	
//	debug_rtc_time(&sTime);
//#endif // DEBUG_RTC	
	
  //отправляем время на дисплей
	static char str[sizeof("00:00:00\0") + 1] = {0};
	int len = Time_To_String((char *)str, sizeof(str));
  msg_out.id = RTCMsgID_RTC_STRING_TIME_TO_LCD;
  msg_out.str_lcd = str;
  msg_out.len = len;
  osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);
  //отправляем UTC на дисплей
	static char str_utc[sizeof("+00\0") + 1] = {0};
	len = UTC_To_String((char *)str_utc, sizeof(str_utc));
  msg_out.id = RTCMsgID_RTC_STRING_UTC_TO_LCD;
  msg_out.str_lcd = str_utc;
  msg_out.len = len;
  osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);
  
  
  //для сигнала 9_21
  if((sTime.Minutes == 0) && (sTime.Seconds == 0))
    {
      msg_out.id = RTCMsgID_SIGNAL_9_21;       
      osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);
    }      
  
		  //для сигнала LED
  if(sTime.Seconds == 0)
    {
      msg_out.id = RTCMsgID_SIGNAL_LED;       
      osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);
    }  
		
  //для сигнала 2
  msg_out.id = RTCMsgID_SIGNAL_2;       
  osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);    

  //для сигнала 3
  msg_out.id = RTCMsgID_SIGNAL_3;       
  osMessageQueuePut(rtc.queue_out, &msg_out, NULL, 0);    
}

/*###############################################################*/
/*###############################################################* RTC_Alarm_IRQHandler -->*/
/*###############################################################*/
/**
  * @brief This function handles RTC alarms A and B interrupt through EXTI line 17.
  */
void RTC_Alarm_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&RtcHandle);
}
/*###############################################################*/
/*###############################################################* RTC_CalendarConfig -->*/
/*###############################################################*/
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sDate;
  RTC_TimeTypeDef sTime;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday October 14th 2019 */
  sDate.Year = 0x19;
  sDate.Month = RTC_MONTH_NOVEMBER;
  sDate.Date = 0x14;
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sDate,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    DEBUG("ERROR HAL_RTC_SetDate!!!\n");
    Error_Handler(); 
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 00:00:00 */
  sTime.Hours = 0x12;
  sTime.Minutes = 0x00;
  sTime.Seconds = 0x00;
  sTime.TimeFormat = RTC_HOURFORMAT_24;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&sTime,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    DEBUG("ERROR HAL_RTC_SetTime!!!\n");
    Error_Handler(); 
  }
  
  /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

/*###############################################################*/
/*###############################################################* MX_RTC_Init -->*/
/*###############################################################*/
/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
void MX_RTC_Init(void)
{
  RTC_AlarmTypeDef sAlarm = {0};
 
	rtc.queue_out = osMessageQueueNew(RTC_MSG_OUT_QUEUE_OBJECTS, sizeof(rtc.msg_out), NULL);
  
  /** Initialize RTC Only 
  */
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    DEBUG("ERROR HAL_RTC_Init!!!\n");
    Error_Handler();
  }

  /* Read the BackUp Register 1 Data */
  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2)
  {  
    /* Configure RTC Calendar */
    RTC_CalendarConfig();
  }
     /** Enable the Alarm A 
    */
    sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
    sAlarm.AlarmTime.Hours = 12;
    sAlarm.AlarmTime.Minutes = 0;
    sAlarm.AlarmTime.Seconds = 0;
    sAlarm.AlarmTime.SubSeconds = 0;
  
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 1;
    sAlarm.Alarm = RTC_ALARM_A;
    if (HAL_RTC_SetAlarm_IT(&RtcHandle, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
      DEBUG("ERROR HAL_RTC_SetAlarm_IT!!!\n");
      Error_Handler();
    }
}
/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources used in this example
  * @param hrtc RTC handle pointer
  * 
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *        the RTC clock source; in this case the Backup domain will be reset in  
  *        order to modify the RTC Clock source, as consequence RTC registers (including 
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *             
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  
  buttons.button_utc_count = 0;
  buttons.button_utc_delay_start = false;
  
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTC clock source */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  /*##-1- Configue LSE/LSI as RTC clock soucre ###############################*/
#ifdef RTC_CLOCK_SOURCE_LSE  
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    DEBUG("ERROR HAL_RTC_SetDate!!!\n");
    Error_Handler();
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    DEBUG("ERROR HAL_RTC_SetDate!!!\n");
    Error_Handler();
  }
  /* Configures the External Low Speed oscillator (LSE) drive capability */
#ifdef STM32F7
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH); 
#endif

#elif defined (RTC_CLOCK_SOURCE_LSI)  
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    DEBUG("ERROR HAL_RCC_OscConfig!!!\n");
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    DEBUG("ERROR HAL_RCCEx_PeriphCLKConfig!!!\n");
    Error_Handler();
  }
#else
#error Please select the RTC Clock source inside the RTC.h file
#endif /*RTC_CLOCK_SOURCE_LSE*/
  
  /*##-2- Enable RTC peripheral Clocks #######################################*/ 
  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE(); 
  
  /*##-3- Configure the NVIC for RTC Alarm ###################################*/
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
  * @brief RTC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();

  /*##-2- Disables the PWR Clock and Disables access to the backup domain ###################################*/
  HAL_PWR_DisableBkUpAccess();
  __HAL_RCC_PWR_CLK_DISABLE();
}
