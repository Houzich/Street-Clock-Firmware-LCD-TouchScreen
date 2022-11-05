/**
  ******************************************************************************
  * File Name          : Settings.c
  ******************************************************************************
  */
  /* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "cmsis_os2.h"
#include "Street_Clock.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//used when debugging, declared in the file Street_Clock.h
#ifdef DEBUG_SETTINGS
#undef DEBUG /* DEBUG */
#undef DEBUG_PRINT /* DEBUG_PRINT */
#define DEBUG(...)		do {debug_header("SETTINGS: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(x)		debug_print x;
#else
#define DEBUG(...)
#define DEBUG_PRINT(...)
#endif // DEBUG_TERMINAL_SETTINGS
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void Callback_Evnt_Buttons_Settings(ButtonsMsgID num);
static void Callback_Evnt_Buttons_SetRTC(ButtonsMsgID num);
static void Callback_Evnt_Buttons_SetClock(ButtonsMsgID num);
static void Callback_Evnt_Buttons_SetUTC(ButtonsMsgID num);
static void Check_Buttons(void);
static void Init(void);
static void Set_RTC(void);
static void Set_Clock(void);
static void Set_UTC(void);
static void Test_Mode_to_LCD(void);
/* Exported functions --------------------------------------------------------*/
extern STREETCLOCK_HandleGPSTypeDef gps;

STREETCLOCK_HandleSettingsTypeDef sett = {
	Init
};
STREETCLOCK_HandleStoreSettingsTypeDef sett_store;
/*###############################################################*/
/*###############################################################* Delay -->*/
/*###############################################################*/
static void Delay(uint32_t ms)
{
#ifdef USE_RTX
	osDelay(ms);
#elif defined(USE_WINDOWS)
	Sleep(ms);
#else
	HAL_Delay(ms);
#endif /*USE_RTX*/
}

#define APP_SETTINGS_STK_SZ (1024*1)
uint64_t settings_stk[APP_SETTINGS_STK_SZ / 8];
const osThreadAttr_t settings_attr = {
  .stack_mem  = &settings_stk[0],
  .stack_size = sizeof(settings_stk)
};
osThreadId_t Settings_TaskHandle = NULL;
/*###############################################################*/
/*###############################################################* Init -->*/
/*###############################################################*/
static void Stop_Clock(SettingsMsgOutQueueTypeDef *msg_out, SettingsMsgInQueueTypeDef *msg_in)
{
    const char *str = "WAIT STOP HANDS";
    static SettingsMsgOutQueueTypeDef msg;
    msg.str_lcd = (char *)str;
    msg.len = strlen(str);
    msg.id = SettingsMsgID_STRING_STATE_TO_LCD;
    osMessageQueuePut(sett.queue_out, &msg, NULL, 0);   // wait for message
    msg.id = SettingsMsgID_STRING_NOTE_TO_LCD;
    osMessageQueuePut(sett.queue_out, &msg, NULL, 0);   // wait for message

    
    msg_out->id = SettingsMsgID_STOP_CLOCK;
    osMessageQueuePut(sett.queue_out, msg_out, NULL, osWaitForever);
    do
    {
      osMessageQueueGet(sett.queue_in, msg_in, NULL, osWaitForever);
      if(msg_in->id == SettingsMsgID_BUTTON_EVNT){
        msg_out->id = SettingsMsgID_BUTTON_EVNT_PROCESSED_OK;
        osMessageQueuePut(sett.queue_out, msg_out, NULL, osWaitForever); 
      }
    } while (msg_in->id != SettingsMsgID_STOP_CLOCK_OK);   
}
/*###############################################################*/
/*###############################################################* Settings_Thread -->*/
/*###############################################################*/
__NO_RETURN void Settings_Thread (void *argument) {
  osStatus_t status;

  SettingsMsgInQueueTypeDef msg_in;
  SettingsMsgOutQueueTypeDef msg_out;
  //SettingsMsgQueueTypeDef msg; 
	while (1) {
		osDelay(10);
		status = osMessageQueueGet(sett.queue_in, &msg_in, NULL, 0);   // wait for message
		if (status == osOK) {
      if(msg_in.id == SettingsMsgID_BUTTON_EVNT){
        Callback_Evnt_Buttons_Settings(msg_in.button_msg_id);
        msg_out.id = SettingsMsgID_BUTTON_EVNT_PROCESSED_OK;
        osMessageQueuePut(sett.queue_out, &msg_out, NULL, osWaitForever);
        
        if(sett.status == SettingsStatus_SET_RTC){
          Stop_Clock(&msg_out, &msg_in);
          DEBUG("Get SettingsMsgID_STOP_CLOCK_OK end start set RTC\n");          
          
          Set_RTC();
          Init();
          msg_out.id = SettingsMsgID_START_CLOCK;
          osMessageQueuePut(sett.queue_out, &msg_out, NULL, osWaitForever); 
          DEBUG("Send SettingsMsgID_START_CLOCK to Core\n");  
        }

        if(sett.status == SettingsStatus_SET_UTC){
          Stop_Clock(&msg_out, &msg_in);
          DEBUG("Get SettingsMsgID_STOP_CLOCK_OK end start set UTC\n");          
          
          Set_UTC();
          Init();
          msg_out.id = SettingsMsgID_START_CLOCK;
          osMessageQueuePut(sett.queue_out, &msg_out, NULL, osWaitForever); 
          DEBUG("Send SettingsMsgID_START_CLOCK to Core\n");  
        }        
               
        if(sett.status == SettingsStatus_SET_CLOCK){
          Stop_Clock(&msg_out, &msg_in);
          DEBUG("Get SettingsMsgID_STOP_CLOCK_OK end start set clock hands\n");          
                    
          Set_Clock();
          Init();
          msg_out.id = SettingsMsgID_START_CLOCK;
          osMessageQueuePut(sett.queue_out, &msg_out, NULL, osWaitForever); 
          DEBUG("Send SettingsMsgID_START_CLOCK to Core\n");          
        }     

        if(sett.status == SettingsStatus_EXIT_TEST_MODE){         
          Test_Mode_to_LCD(); 
          DEBUG("Send SettingsStatus_EXIT_TEST_MODE to Core\n");          
        }   


				
      }
    }
  }
}
/*###############################################################*/
/*###############################################################* Init -->*/
/*###############################################################*/
static void Init(void)
{
	sett.status = SettingsStatus_READY;
	sett.error = SettingsError_OK;
	sett.position_settime = 0;
	sett.value_settime = 0;
  sett.enable_button_settings = true;
	
	if(sett_store.mode_rtc == MODE_RTC_TEST){
		sett.time_test_mode = TIME_TEST_MODE;
		sett.counter_secundes_between_click = SECUNDES_BETWEEN_CLICK; //счетчик секунд нажатия кнопки выхода из тестового режима
		sett.counter_click_to_exit_test_mode = NUM_CLICKS_EXIT_TEST_MODE;
	}
	
  //Enable_Disable_Buttons(false);
  if (Settings_TaskHandle == NULL) {
		Settings_TaskHandle = osThreadNew(Settings_Thread, NULL, &settings_attr);
	}    
}

/*###############################################################*/
/*###############################################################* SetRTC_to_LCD -->*/
/*###############################################################*/
static void SetRTC_to_LCD(bool enable_blinking)
{
  bool send_mes = false;
  static bool blinking = false; //определяет затухать символу или нет
  static uint32_t time_toggle = 0;
  #define TIME_LEN sizeof("00:00:00\0")
  static char str[TIME_LEN];
  if((uint32_t)(osKernelGetTickCount() - time_toggle) > 300)
  {
    send_mes = true;
    blinking = !blinking;
    time_toggle = osKernelGetTickCount();
  }
  if(enable_blinking) {
    blinking = false;
    send_mes = true;
  }
  char hour[3], min[3], sec[3];
  snprintf(hour, 3, "%.2d", sett.set_time.Hours);
  snprintf(min, 3, "%.2d", sett.set_time.Minutes);
  snprintf(sec, 3, "%.2d", sett.set_time.Seconds);
  if(blinking){
    switch (sett.position_settime)
    {
      case 0: memcpy(&hour, "  ", 2); break;
      case 1: memcpy(&min, "  ", 2); break;
      case 2: memcpy(&sec, "  ", 2); break;
    }
  }
  int len = 0;
  
//  if(sett.set_time.TimeFormat == RTC_HOURFORMAT12_AM)
//    len = snprintf(str, TIME_LEN, "%s:%s:%s AM", hour, min, sec);
//  else
//    len = snprintf(str, TIME_LEN, "%s:%s:%s PM", hour, min, sec);
  len = snprintf(str, TIME_LEN, "%s:%s:%s", hour, min, sec);
  
  static SettingsMsgOutQueueTypeDef msg_out;
  msg_out.id = SettingsMsgID_STRING_RTC_TO_LCD;
  msg_out.str_lcd = str;
  msg_out.len = len;
  if(send_mes){
    osMessageQueuePut(sett.queue_out, &msg_out, NULL, 0);   // wait for message
  }
 }

 /*###############################################################*/
/*###############################################################* SetClock_to_LCD -->*/
/*###############################################################*/
static void SetClock_to_LCD(bool enable_blinking)
{
  bool send_mes = false;
  static bool blinking = false; //определяет затухать символу или нет
  static uint32_t time_toggle = 0;
  static char str[sizeof("00:00:00\0")];
  if((uint32_t)(osKernelGetTickCount() - time_toggle) > 300)
  {
    send_mes = true;
    blinking = !blinking;
    time_toggle = osKernelGetTickCount();
  }
  char hour[3], min[3];
  if(enable_blinking) {
    blinking = false;
    send_mes = true;
  }
  snprintf(hour, 3, "%.2d", sett.set_time.Hours);
  snprintf(min, 3, "%.2d", sett.set_time.Minutes);
  if(blinking){
    switch (sett.position_settime)
    {
      case 0: memcpy(&hour, "  ", 2); break;
      case 1: memcpy(&min, "  ", 2); break;
    }
  }
  int len = snprintf(str, 9, "%s:%s", hour, min);
  static SettingsMsgOutQueueTypeDef msg_out;
  msg_out.id = SettingsMsgID_STRING_CLOCK_TO_LCD;
  msg_out.str_lcd = str;
  msg_out.len = len;
  if(send_mes){
    osMessageQueuePut(sett.queue_out, &msg_out, NULL, 0);   // wait for message
  }
 }

 /*###############################################################*/
/*###############################################################* Test_Mode_to_LCD -->*/
/*###############################################################*/
static void Test_Mode_to_LCD()
{
  static char str[sizeof("0000\0")];
	int len;
	SettingsMsgOutQueueTypeDef msg_out;
	
	if((sett.counter_click_to_exit_test_mode>0)&&(sett.counter_click_to_exit_test_mode<NUM_CLICKS_EXIT_TEST_MODE)){		
		len = snprintf(str, 4, "%d", sett.counter_click_to_exit_test_mode);
	} else if (sett.counter_click_to_exit_test_mode == NUM_CLICKS_EXIT_TEST_MODE){
			if(sett_store.mode_rtc == MODE_RTC_TEST)
				len = sprintf(str, "%s", "T");
			else
				len = sprintf(str, "%s", "X");
	} else {
		len = snprintf(str, 4, "%s", "");
	}
		msg_out.id = SettingsMsgID_EXIT_TEST_MODE;
		msg_out.str_lcd = str;
		msg_out.len = len;
		osMessageQueuePut(sett.queue_out, &msg_out, NULL, 0); 
 }
/*###############################################################*/
/*###############################################################* SetUTC_to_LCD -->*/
/*###############################################################*/
static void SetUTC_to_LCD(bool enable_blinking)
{
  bool send_mes = false;
  static bool blinking = false; //определяет затухать символу или нет
  static uint32_t time_toggle = 0;
  static char str[sizeof("+00\0")];
  if((uint32_t)(osKernelGetTickCount() - time_toggle) > 300)
  {
    send_mes = true;
    blinking = !blinking;
    time_toggle = osKernelGetTickCount();
  }

  if(enable_blinking) {
    blinking = false;
    send_mes = true;
  }
  int len;
  if(blinking) {
    len = snprintf(str, 4, "   ");
  } else {
    if(sett.value_utc >= 0) len = snprintf(str, 4, "+%d", sett.value_utc);
    else len = snprintf(str, 4, "%d", sett.value_utc);
  }
  
  static SettingsMsgOutQueueTypeDef msg_out;
  msg_out.id = SettingsMsgID_STRING_UTC_TO_LCD;
  msg_out.str_lcd = str;
  msg_out.len = len;
  if(send_mes){
    osMessageQueuePut(sett.queue_out, &msg_out, NULL, 0);   // wait for message
  }
 }
 
 
/*###############################################################*/
/*###############################################################* Set_RTC -->*/
/*###############################################################*/
static void Set_RTC(void)
{
  osStatus_t status;
	sett.status = SettingsStatus_SET_RTC;
  SettingsMsgInQueueTypeDef msg_in;
  SettingsMsgOutQueueTypeDef msg_out;
  
  GetRTCTime(&sett.set_time);
  sett.position_settime = 0;
  sett.value_settime = sett.set_time.Hours;
  sett.timeformat_settime = sett.set_time.TimeFormat;
  
	while (1) {
    status = osMessageQueueGet(sett.queue_in, &msg_in, NULL, 0);   // wait for message
		if (status == osOK) {
      if(msg_in.id == SettingsMsgID_BUTTON_EVNT){
        Check_Buttons();
        Callback_Evnt_Buttons_SetRTC(msg_in.button_msg_id);
        msg_out.id = SettingsMsgID_BUTTON_EVNT_PROCESSED_OK;
        osMessageQueuePut(sett.queue_out, &msg_out, NULL, osWaitForever);       
      }
      if(msg_in.button_msg_id == ButtonsMsgID_SAVE_CLICKED){
        Set_RTC_Time(&sett.set_time);
        gps.start = true;
        break;
      }
      if(msg_in.button_msg_id == ButtonsMsgID_RETURN_CLICKED){
        break;
      }    
    }
    SetRTC_to_LCD(false);
    Delay(1);
	}
  SetRTC_to_LCD(true);
	sett.status = SettingsStatus_READY;
}
/*###############################################################*/
/*###############################################################* Set_Clock -->*/
/*###############################################################*/
static void Set_Clock(void)
{
  osStatus_t status;
	sett.status = SettingsStatus_SET_CLOCK;
  SettingsMsgInQueueTypeDef msg_in;
  SettingsMsgOutQueueTypeDef msg_out;
  GetClockTime(&sett.set_time);
  sett.position_settime = 0;
  sett.value_settime = sett.set_time.Hours;
	while (1) {
    status = osMessageQueueGet(sett.queue_in, &msg_in, NULL, 0);   // wait for message
		if (status == osOK) {
      if(msg_in.id == SettingsMsgID_BUTTON_EVNT){
        //Check_Buttons();
        Callback_Evnt_Buttons_SetClock(msg_in.button_msg_id);
        msg_out.id = SettingsMsgID_BUTTON_EVNT_PROCESSED_OK;
        osMessageQueuePut(sett.queue_out, &msg_out, NULL, osWaitForever);       
      }
      if(msg_in.button_msg_id == ButtonsMsgID_SAVE_CLICKED){
        Store_Clock(sett.set_time.Hours, sett.set_time.Minutes);
        break;
      }
      if(msg_in.button_msg_id == ButtonsMsgID_RETURN_CLICKED){
        break;
      }    
    }
    SetClock_to_LCD(false);
    Delay(1);
	}
  SetClock_to_LCD(true);
	sett.status = SettingsStatus_READY;
}
/*###############################################################*/
/*###############################################################* Set_UTC -->*/
/*###############################################################*/
static void Set_UTC(void)
{
  osStatus_t status;
	sett.status = SettingsStatus_SET_UTC;
  SettingsMsgInQueueTypeDef msg_in;
  SettingsMsgOutQueueTypeDef msg_out;
  sett.value_utc = GetUTC();
	while (1) {
    status = osMessageQueueGet(sett.queue_in, &msg_in, NULL, 0);   // wait for message
		if (status == osOK) {
      if(msg_in.id == SettingsMsgID_BUTTON_EVNT){
        //Check_Buttons();
        Callback_Evnt_Buttons_SetUTC(msg_in.button_msg_id);
        msg_out.id = SettingsMsgID_BUTTON_EVNT_PROCESSED_OK;
        osMessageQueuePut(sett.queue_out, &msg_out, NULL, osWaitForever);       
      }
      if(msg_in.button_msg_id == ButtonsMsgID_SAVE_CLICKED){
        SetUTC(sett.value_utc);
        gps.start = true;
        break;
      }
      if(msg_in.button_msg_id == ButtonsMsgID_RETURN_CLICKED){
        break;
      }    
    }
    SetUTC_to_LCD(false);
    Delay(1);
	}
  SetUTC_to_LCD(true);
	sett.status = SettingsStatus_READY;
}
/*###############################################################*/
/*###############################################################* Enable_Disable_Settings -->*/
/*###############################################################*/
void Enable_Disable_Buttons(bool state)
{
	sett.enable_button_up = state;
	sett.enable_button_down = state;
	sett.enable_button_left = state;
	sett.enable_button_right = state;
	sett.enable_button_ok = state;
	sett.enable_button_return = state;
}
/*###############################################################*/
/*###############################################################* Check_Buttons -->*/
/*###############################################################*/
static void Check_Buttons(void)
{
	if (sett.status == SettingsStatus_SET_RTC) {
		if (sett.position_settime == 0) sett.enable_button_right = false;
		else sett.enable_button_right = true;

		if (sett.position_settime == 2) sett.enable_button_left = false;
		else sett.enable_button_left = true;

		sett.enable_button_ok = true;
		sett.enable_button_return = true;
		sett.enable_button_settings = false;
	}
}

/*###############################################################*/
/*###############################################################* Callback_Evnt_Buttons_Settings -->*/
/*###############################################################*/
static void Callback_Evnt_Buttons_Settings(ButtonsMsgID num)
{
  if ((sett.status != SettingsStatus_SET_RTC)&&(sett.status != SettingsStatus_SET_CLOCK)
      &&(sett.status != SettingsStatus_SET_UTC)){  
    if(num == ButtonsMsgID_SET_RTC_CLICKED){
      DEBUG("Set status SET_RTC\n");
      sett.status = SettingsStatus_SET_RTC;
			return;
    }
    if(num == ButtonsMsgID_SET_CLOCK_CLICKED){
      DEBUG("Set status SET_CLOCK\n");
      sett.status = SettingsStatus_SET_CLOCK;
			return;
    }
    if(num == ButtonsMsgID_SET_UTC_CLICKED){
      DEBUG("Set status SET_UTC\n");
      sett.status = SettingsStatus_SET_UTC;
			return;
    }    
			
		
		if((sett_store.mode_rtc == MODE_RTC_TEST)||(sett_store.mode_rtc == MODE_RTC_BLOCK))
			{
			//Если нажали кнопку SAVE пока модуль в тестовом режиме
			if(num == ButtonsMsgID_SAVE_CLICKED)
				{
					//если нажали ее в неправильной последовательности
					if((sett.counter_click_to_exit_test_mode == 6)||(sett.counter_click_to_exit_test_mode == 1))
						{
							sett.counter_click_to_exit_test_mode = NUM_CLICKS_EXIT_TEST_MODE;
						}
					else
						{
							sett.counter_click_to_exit_test_mode--;					
						}
					sett.counter_secundes_between_click = SECUNDES_BETWEEN_CLICK;
					sett.status = SettingsStatus_EXIT_TEST_MODE;
				}
			else if(num == ButtonsMsgID_UP_CLICKED)
				{
					//если нажали ее в правильной последовательности
					if((sett.counter_click_to_exit_test_mode == 6)||(sett.counter_click_to_exit_test_mode == 1))
					{
						sett.counter_click_to_exit_test_mode--;				
						//если нажали нужное кол-во раз. выходим с тестового режима и сохраняем настройки
						if(sett.counter_click_to_exit_test_mode == 0)
							{
								sett_store.mode_rtc = MODE_RTC_ENABLE;
								FLASH_Save_Settings();
							}		
					}
					else//если нажали ее в неправильной последовательности 
					{
						sett.counter_click_to_exit_test_mode = NUM_CLICKS_EXIT_TEST_MODE;
					}
					sett.counter_secundes_between_click = SECUNDES_BETWEEN_CLICK;
					sett.status = SettingsStatus_EXIT_TEST_MODE;	
					//а если другую кнопку нажали, то обнуляем
				}
			else if(sett.counter_click_to_exit_test_mode != NUM_CLICKS_EXIT_TEST_MODE)
				{
					sett.counter_click_to_exit_test_mode = NUM_CLICKS_EXIT_TEST_MODE;
					sett.counter_secundes_between_click = SECUNDES_BETWEEN_CLICK;
					sett.status = SettingsStatus_EXIT_TEST_MODE;
				}
			}		
  }
}

/*###############################################################*/
/*###############################################################* Callback_Evnt_Buttons_SetRTC -->*/
/*###############################################################*/
static void Callback_Evnt_Buttons_SetRTC(ButtonsMsgID num)
{
  int value = sett.value_settime;
  int position = sett.position_settime;
  uint32_t timeformat = sett.set_time.TimeFormat;  
  if (sett.status == SettingsStatus_SET_RTC){
    switch (num) {
      case ButtonsMsgID_UP_CLICKED:
        value++;
#ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT
        if ((value > 12)&&(position == 0)){ 
          value = 1;
          if (timeformat == RTC_HOURFORMAT12_AM) timeformat = RTC_HOURFORMAT12_PM;
          else timeformat = RTC_HOURFORMAT12_AM;
        }
#else
        if ((value > 23)&&(position == 0)){ 
          value = 0;
        }    
#endif
        else if (value > 59) {value = 0;}
        break;
      case ButtonsMsgID_DOWN_CLICKED:
        value--;
#ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT      
        if ((value < 1)&&(position == 0)){
          if (timeformat == RTC_HOURFORMAT12_AM) timeformat = RTC_HOURFORMAT12_PM;
          else timeformat = RTC_HOURFORMAT12_AM;
          value = 12;
        }
#else
        if ((value < 0)&&(position == 0)){ 
          value = 23;
        }    
#endif        
        else if (value < 0) {value = 59;}
        break;
      case ButtonsMsgID_LEFT_CLICKED:
        position--;
        if(position < 0) position = 0;
        switch (position)
        {
          case 0: value = sett.set_time.Hours;  break;
          case 1: value = sett.set_time.Minutes;  break;
          case 2: value = sett.set_time.Seconds;  break;
        }
        break;
      case ButtonsMsgID_RIGHT_CLICKED:
        position++;
        if(position > 2) position = 2;
        switch (position)
        {
          case 0: value = sett.set_time.Hours;  break;
          case 1: value = sett.set_time.Minutes;  break;
          case 2: value = sett.set_time.Seconds;  break;
        }
        break;  
      case ButtonsMsgID_SAVE_CLICKED:
      case ButtonsMsgID_RETURN_CLICKED:       
      default: break;
    }
    sett.value_settime = value;
    switch (position)
    {
      case 0: sett.set_time.Hours = value;  break;
      case 1: sett.set_time.Minutes = value;  break;
      case 2: sett.set_time.Seconds = value;  break;
    }
    sett.position_settime = position;
    sett.set_time.TimeFormat = timeformat;
  }
}

/*###############################################################*/
/*###############################################################* Callback_Evnt_Buttons_SetClock -->*/
/*###############################################################*/
static void Callback_Evnt_Buttons_SetClock(ButtonsMsgID num)
{
  int value = sett.value_settime;
  int position = sett.position_settime;
  if (sett.status == SettingsStatus_SET_CLOCK){
    switch (num) {
      case ButtonsMsgID_UP_CLICKED:
        value++;
        if ((value > 12)&&(position == 0)) value = 1;
        else if (value > 59) value = 0;
        break;
      case ButtonsMsgID_DOWN_CLICKED:
        value--;
        if ((value < 1)&&(position == 0)) value = 12;
        else if (value < 0) value = 59;
        break;
      case ButtonsMsgID_LEFT_CLICKED:
        position--;
        if(position < 0) position = 0;
        switch (position)
        {
          case 0: value = sett.set_time.Hours;  break;
          case 1: value = sett.set_time.Minutes;  break;
        }
        break;
      case ButtonsMsgID_RIGHT_CLICKED:
        position++;
        if(position > 1) position = 1;
        switch (position)
        {
          case 0: value = sett.set_time.Hours;  break;
          case 1: value = sett.set_time.Minutes;  break;
        }
        break;  
      case ButtonsMsgID_SAVE_CLICKED:
      case ButtonsMsgID_RETURN_CLICKED:       
      default: break;
    }
    sett.value_settime = value;
    switch (position)
    {
      case 0: sett.set_time.Hours = value;  break;
      case 1: sett.set_time.Minutes = value;  break;
    }
    sett.position_settime = position;
  }
}

/*###############################################################*/
/*###############################################################* Callback_Evnt_Buttons_SetUTC -->*/
/*###############################################################*/
static void Callback_Evnt_Buttons_SetUTC(ButtonsMsgID num)
{
  int value = sett.value_utc;
  if (sett.status == SettingsStatus_SET_UTC){
    switch (num) {
      case ButtonsMsgID_UP_CLICKED:
        value++;
        if(value > 14) value = -12;
        break;
      case ButtonsMsgID_DOWN_CLICKED:
        value--;
        if (value < -12) value = 14;
        break;
      case ButtonsMsgID_SAVE_CLICKED:
      case ButtonsMsgID_RETURN_CLICKED:       
      default: break;
    }
    sett.value_utc = value;
  }
}

