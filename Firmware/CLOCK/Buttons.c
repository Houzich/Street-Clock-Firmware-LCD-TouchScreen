/**
  ******************************************************************************
  * File Name          : Buttons.c
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
#ifdef DEBUG_BUTTONS
#undef DEBUG /* DEBUG */
#undef DEBUG_PRINT /* DEBUG_PRINT */
#define DEBUG(...)		do {debug_header("BUTTONS: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(x)		debug_print x;
#else
#define DEBUG(...)
#define DEBUG_PRINT(...)
#endif // DEBUG_TERMINAL_BUTTONS
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static void Check_Buttons(void);
static void Enable_Disable_Buttons(bool state);
static void Init(void);
/* Exported functions --------------------------------------------------------*/
STREETCLOCK_HandleButtonsTypeDef buttons = {
	Init
};
/*###############################################################*/
/*###############################################################* Delay -->*/
/*###############################################################*/
//static void Delay(uint32_t ms)
//{
//#ifdef USE_RTX
//	osDelay(ms);
//#elif defined(USE_WINDOWS)
//	Sleep(ms);
//#else
//	HAL_Delay(ms);
//#endif /*USE_RTX*/
//}

#define BUTTONS_STK_SZ (1024*1)
uint64_t buttons_stk[BUTTONS_STK_SZ / 8];
const osThreadAttr_t buttons_attr = {
  .stack_mem  = &buttons_stk[0],
  .stack_size = sizeof(buttons_stk)
};
osThreadId_t Buttons_TaskHandle = NULL;
/*###############################################################*/
/*###############################################################* Buttons_Thread -->*/
/*###############################################################*/
__NO_RETURN void Buttons_Thread (void *argument) {
  osStatus_t status;
  ButtonsMsgInQueueTypeDef msg_in;
  ButtonsMsgOutQueueTypeDef msg_out;
  ButtonsMsgQueueTypeDef msg; 
  
	while (1) {
		osDelay(10);
    status = osMessageQueueGet(buttons.queue, &msg, NULL, 0);   // wait for message
		if (status == osOK) {
      msg_out.id = msg.id;
      osMessageQueuePut(buttons.queue_out, &msg_out, NULL, osWaitForever);
      while(1){
        osMessageQueueGet(buttons.queue_in, &msg_in, NULL, osWaitForever);
        if(msg_in.id == ButtonsMsgID_BUTTON_PROCESSED_OK){
          break;
        }
      }
    }   
		status = osMessageQueueGet(buttons.queue_in, &msg, NULL, 0);   // wait for message
		if (status == osOK) {
   
    }
	}

}
/*###############################################################*/
/*###############################################################* Init -->*/
/*###############################################################*/
static void Init(void)
{
	buttons.status = ButtonsStatus_BTN_READY;
	buttons.error = ButtonsError_BTN_OK;
  buttons.enable_button_settings = true;
  Enable_Disable_Buttons(false);

  if (Buttons_TaskHandle == NULL) {
		Buttons_TaskHandle = osThreadNew(Buttons_Thread, NULL, &buttons_attr);
	}    
}


/*###############################################################*/
/*###############################################################* Enable_Disable_Buttons -->*/
/*###############################################################*/
static void Enable_Disable_Buttons(bool state)
{
	buttons.enable_button_up = state;
	buttons.enable_button_down = state;
	buttons.enable_button_left = state;
	buttons.enable_button_right = state;
	buttons.enable_button_ok = state;
	buttons.enable_button_return = state;
}
///*###############################################################*/
///*###############################################################* Enable_Disable_Buttons -->*/
///*###############################################################*/
//static void Check_Buttons(void)
//{
//	if (sett.status == ButtonsStatus_SET_RTC) {
//		if (sett.position_settime == 0) sett.enable_button_right = false;
//		else sett.enable_button_right = true;

//		if (sett.position_settime == 2) sett.enable_button_left = false;
//		else sett.enable_button_left = true;

//		sett.enable_button_ok = true;
//		sett.enable_button_return = true;
//		sett.enable_button_settings = false;
//	}
//}
/*###############################################################*/
/*###############################################################* Callback_Buttons_Buttons -->*/
/*###############################################################*/
void Callback_Buttons(Buttons num)
{
  ButtonsMsgQueueTypeDef msg;
  bool send_msg = true;
    switch (num) {
      case Buttons_UP:
        msg.id = ButtonsMsgID_UP_CLICKED; break;
      case Buttons_DOWN:
        msg.id = ButtonsMsgID_DOWN_CLICKED; break;
      case Buttons_LEFT:
        msg.id = ButtonsMsgID_LEFT_CLICKED; break;
      case Buttons_RIGHT:
        msg.id = ButtonsMsgID_RIGHT_CLICKED; break;
      case Buttons_SAVE:
        msg.id = ButtonsMsgID_SAVE_CLICKED; break;
      case Buttons_RETURN:       
        msg.id = ButtonsMsgID_RETURN_CLICKED; break;
      case Buttons_SET_RTC:       
        msg.id = ButtonsMsgID_SET_RTC_CLICKED; break;
      case Buttons_SET_CLOCK:       
        msg.id = ButtonsMsgID_SET_CLOCK_CLICKED; break;
      case Buttons_SET_UTC_DOWN:
        buttons.button_utc_count = 0;
        buttons.button_utc_delay_start = true;
        send_msg = false;
        break;
        //msg.id = ButtonsMsgID_SET_UTC_CLICKED; break;
        
      case Buttons_SET_UTC_UP:
        buttons.button_utc_count = 0;
        buttons.button_utc_delay_start = false;
        send_msg = false;
        break;
      default:
        send_msg = false;
        break;
    }
    if(send_msg)
      osMessageQueuePut(buttons.queue, &msg, NULL, 0);
}
