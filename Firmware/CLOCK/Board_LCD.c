/*------------------------------------------------------------------------------
 * Name:    LCD.c
 * Purpose: 
 * Rev.:    V0
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "Street_Clock.h"
#include "GUI.h"
#include "WindowDLG.h"
//#include "WindowDLG.c"
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void);
/* Exported functions --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
STREETCLOCK_HandleLCDTypeDef lcd = {
	Init
};

// LCD stack size must be multiple of 8 Bytes
#define LCD_STK_SZ (1024*32)
uint64_t lcd_stk[LCD_STK_SZ / 8];
const osThreadAttr_t lcd_attr = {
  .stack_mem  = &lcd_stk[0],
  .stack_size = sizeof(lcd_stk)
};
osThreadId_t LCD_TaskHandle = NULL;
/*###############################################################*/
/*###############################################################* Init -->*/
/*###############################################################*/
static void Init(void)
{
  if (LCD_TaskHandle == NULL) {
		LCD_TaskHandle = osThreadNew(LCD_Thread, NULL, &lcd_attr);
	}    
}

extern STREETCLOCK_HandleGPSTypeDef gps;
/*###############################################################*/
/*###############################################################* Init -->*/
/*###############################################################*/
__NO_RETURN void LCD_Thread (void *argument) {
  osStatus_t status;
  static LCDMsgInQueueTypeDef msg_in;  
  WM_HWIN hWin;
  #if GUI_WINSUPPORT
    WM_SetCreateFlags(WM_CF_MEMDEV);
  #endif
  GUI_Init();
  #if GUI_WINSUPPORT
    WM_MULTIBUF_Enable(1);
  #endif
	
  hWin = CreateWindow();
	WM_SelectWindow(hWin);
	GUI_Exec();
  bool guiexec = false;
	while (1) {
		osDelay(20);
   
    do
    {
      status = osMessageQueueGet(lcd.queue_in, &msg_in, NULL, 0);   // wait for message
      if (status == osOK) {
        
        if(msg_in.id == LCDMsgID_STR_RTC){
          TEXT_SetText(hItem_TEXT_RTC, msg_in.string);
        }
        if(msg_in.id == LCDMsgID_STR_CLOCK){
          TEXT_SetText(hItem_TEXT_Clock, msg_in.string);
        }
        if(msg_in.id == LCDMsgID_STR_STATE){
          TEXT_SetText(hItem_TEXT_State, msg_in.string);
        }
        if(msg_in.id == LCDMsgID_STR_NOTE){
          TEXT_SetText(hItem_TEXT_Note, msg_in.string);
        }
        if(msg_in.id == LCDMsgID_STR_UTC){
          TEXT_SetText(hItem_TEXT_UTC, msg_in.string);
        }      
        if(msg_in.id == LCDMsgID_STR_TEST_MODE){
          TEXT_SetText(hItem_Text_Block, msg_in.string);
        }    				
        
        guiexec = true;
      }   	
    } while (status == osOK);
       
    if(guiexec){
      guiexec = false;
      GUI_Exec();
    }
		
		if(gps.gps_signal == false){
			TEXT_SetText(hItem_TEXT_GPS_Signal, "NO GPS Signal");
		} else {
			TEXT_SetText(hItem_TEXT_GPS_Signal, "YES GPS Signal");
		}

	}

}





/*****************************END OF FILE****/

