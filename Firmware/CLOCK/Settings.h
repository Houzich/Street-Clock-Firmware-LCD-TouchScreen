/**
  ******************************************************************************
  * File Name          : Settings.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SETTINGS_H
#define __SETTINGS_H
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
#include "Buttons.h"
/* Exported constants --------------------------------------------------------*/
/**
* @brief  Settings acceptor status
*/
typedef enum __SettingsMsgIDEnumTypeDef{
  SettingsMsgID_BUTTON_EVNT = 0x01,
  SettingsMsgID_RETURN_EVNT = 0x02,
  SettingsMsgID_BUTTON_EVNT_PROCESSED_OK = 0x03,
  SettingsMsgID_STRING_RTC_TO_LCD = 0x04,
  SettingsMsgID_STRING_CLOCK_TO_LCD = 0x05,
  SettingsMsgID_STOP_CLOCK = 0x06,
  SettingsMsgID_STOP_CLOCK_OK = 0x07,
  SettingsMsgID_START_CLOCK = 0x08,
  SettingsMsgID_STRING_UTC_TO_LCD = 0x09,
  SettingsMsgID_STRING_STATE_TO_LCD = 0x10,
  SettingsMsgID_STRING_NOTE_TO_LCD = 0x11,
  SettingsMsgID_EXIT_TEST_MODE = 0x12,
}SettingsMsgID;
/**
* @brief  Settings acceptor status
*/
typedef enum __SettingsStatusEnumTypeDef{
	SettingsStatus_READY = 0x00,
	SettingsStatus_SET_RTC = 0x01,
	SettingsStatus_SAVE_RTC = 0x02,
	SettingsStatus_SET_CLOCK = 0x03,
	SettingsStatus_SAVE_CLOCK = 0x04,
  SettingsStatus_SET_UTC = 0x05,
  SettingsStatus_SAVE_UTC = 0x06,
	SettingsStatus_EXIT_TEST_MODE = 0x07,
}SettingsStatus;
/**
* @brief  Terminal bill acceptor error
*/
typedef enum __SettingsErrorEnumTypeDef{
	SettingsError_OK = 0x00,
}SettingsError;


#define SETTINGS_MSG_IN_QUEUE_OBJECTS 10
typedef struct {                                // object data type
  SettingsMsgID id;
  ButtonsMsgID button_msg_id;
} SettingsMsgInQueueTypeDef;

#define SETTINGS_MSG_OUT_QUEUE_OBJECTS 20
typedef struct {                                // object data type
  SettingsMsgID id;
  char *str_lcd;
  int len;
} SettingsMsgOutQueueTypeDef;

#define SETTINGS_MSG_QUEUE_OBJECTS 4
typedef struct {                                // object data type
  SettingsMsgID id;
} SettingsMsgQueueTypeDef;

/**
  * @brief  Terminal main structure
  */
typedef struct __STREETCLOCK_HandleSettingsTypeDef
{
	void(*init)(void);
	SettingsStatus status;
	SettingsError error;
	int32_t	position_settime;
	int32_t	value_settime;
	int32_t	value_utc;
  uint32_t timeformat_settime;
  RTC_TimeTypeDef set_time;
  
  osMessageQueueId_t queue_in;
  osMessageQueueId_t queue_out;
  osMessageQueueId_t queue;
  
  SettingsMsgInQueueTypeDef msg_in;
  SettingsMsgOutQueueTypeDef msg_out;
  SettingsMsgQueueTypeDef msg;
  
  bool enable_button_up;
	bool enable_button_down;
	bool enable_button_left;
	bool enable_button_right;
	bool enable_button_ok;
	bool enable_button_settings;
	bool enable_button_return;
	
	int32_t	time_test_mode;	//счетчик секунд тестового режима
	int32_t	counter_secundes_between_click; //счетчик секунд нажатия кнопки выхода из тестового режима
	int32_t	counter_click_to_exit_test_mode;
} STREETCLOCK_HandleSettingsTypeDef;

typedef struct __STREETCLOCK_HandleStoreSettingsTypeDef
{
  int UTC;
	int mode_rtc; //
} STREETCLOCK_HandleStoreSettingsTypeDef;

#define NUM_CLICKS_EXIT_TEST_MODE 10 //кол-во нажатий на кнопку чтобы убрать тестовый режим
#define SECUNDES_BETWEEN_CLICK 3 //секунды между нажатиями
#define MODE_RTC_ENABLE 0
#define MODE_RTC_BLOCK 1
#define MODE_RTC_TEST -1
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

//void Callback_Buttons_Settings(TERMINAL_ButtonsTypeDef num);
//void Set_Settings_Manual(void);
//void Check_Receive_Settings(TERMINAL_SettingsTypeDef* receive);

#ifdef __cplusplus
}
#endif
#endif /*__SETTINGS_H*/
