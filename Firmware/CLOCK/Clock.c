/**
  ******************************************************************************
  * File Name          : Clock.c
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "Street_Clock.h"
#include "Board_LED.h"
#include "Driver_USART.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//used when debugging, declared in the file Street_Clock.h
#ifdef DEBUG_CLOCK
#undef DEBUG /* DEBUG */
#undef DEBUG_PRINT /* DEBUG_PRINT */
#define DEBUG(...)		do {debug_header("CLOCK: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(x)		debug_print x;
#else
#define DEBUG(...)
#define DEBUG_PRINT(...)
#endif // DEBUG_TERMINAL_SETTINGS
/* Private variables ---------------------------------------------------------*/
extern RTC_HandleTypeDef RtcHandle;
static ARM_DRIVER_USART *USARTdrv;
/* USART Driver */
extern ARM_DRIVER_USART SIGNAL_DRIVER_USART;
/* Private function prototypes -----------------------------------------------*/
static void Init(void);
static void Tick_Clock_Hands(void);
void Get_Store_Clock(void);
static void USART_Signal_Callback(uint32_t event);
static void Signal_Message (uint8_t *message, int num);

extern STREETCLOCK_HandleStoreSettingsTypeDef sett_store;
/* Exported functions --------------------------------------------------------*/
// LCD stack size must be multiple of 8 Bytes
#define APP_CLOCK_STK_SZ (1024*1)
uint64_t clock_stk[APP_CLOCK_STK_SZ / 8];
const osThreadAttr_t clock_attr = {
  .stack_mem  = &clock_stk[0],
  .stack_size = sizeof(clock_stk)
};
osThreadId_t Clock_TaskHandle = NULL;
osThreadId_t Signal_1_TaskHandle = NULL;
osThreadId_t Signal_2_TaskHandle = NULL;
osThreadId_t Signal_3_TaskHandle = NULL;
osThreadId_t Signal_LED_TaskHandle = NULL;

STREETCLOCK_HandleClockTypeDef clock = {
	Init
};
static char Str_State[100];
static char Str_Note[100];
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

/*###############################################################*/
/*###############################################################* Clock_GPIO_MspInit -->*/
/*###############################################################*/
void Clock_GPIO_MspInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
#if (MOTOR_11_Pin_En	== 1)
  HAL_GPIO_DeInit(MOTOR_11_GPIO_Port, MOTOR_11_Pin);
	HAL_GPIO_WritePin(MOTOR_11_GPIO_Port, MOTOR_11_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = MOTOR_11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOTOR_11_GPIO_Port, &GPIO_InitStruct);
#endif

#if (MOTOR_12_Pin_En	== 1)
  HAL_GPIO_DeInit(MOTOR_12_GPIO_Port, MOTOR_12_Pin);
	HAL_GPIO_WritePin(MOTOR_12_GPIO_Port, MOTOR_12_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = MOTOR_12_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOTOR_12_GPIO_Port, &GPIO_InitStruct);
#endif

#if (MOTOR_13_Pin_En	== 1)
  HAL_GPIO_DeInit(MOTOR_13_GPIO_Port, MOTOR_13_Pin);
	HAL_GPIO_WritePin(MOTOR_13_GPIO_Port, MOTOR_13_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = MOTOR_13_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOTOR_13_GPIO_Port, &GPIO_InitStruct);
#endif  
  
#if (MOTOR_21_Pin_En	== 1)  
  HAL_GPIO_DeInit(MOTOR_21_GPIO_Port, MOTOR_21_Pin);
	HAL_GPIO_WritePin(MOTOR_21_GPIO_Port, MOTOR_21_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = MOTOR_21_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOTOR_21_GPIO_Port, &GPIO_InitStruct);
#endif  

#if (MOTOR_22_Pin_En	== 1)  
  HAL_GPIO_DeInit(MOTOR_22_GPIO_Port, MOTOR_22_Pin);
	HAL_GPIO_WritePin(MOTOR_22_GPIO_Port, MOTOR_22_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = MOTOR_22_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOTOR_22_GPIO_Port, &GPIO_InitStruct);
#endif  

#if (MOTOR_23_Pin_En	== 1)  
  HAL_GPIO_DeInit(MOTOR_23_GPIO_Port, MOTOR_23_Pin);
	HAL_GPIO_WritePin(MOTOR_23_GPIO_Port, MOTOR_23_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = MOTOR_23_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOTOR_23_GPIO_Port, &GPIO_InitStruct);
#endif

#if (SIGNAL_9_21_Pin_En	== 1)  
  HAL_GPIO_DeInit(SIGNAL_9_21_GPIO_Port, SIGNAL_9_21_Pin);
	HAL_GPIO_WritePin(SIGNAL_9_21_GPIO_Port, SIGNAL_9_21_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = SIGNAL_9_21_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(SIGNAL_9_21_GPIO_Port, &GPIO_InitStruct);
#endif

#if (SIGNAL_2_Pin_En	== 1)  
  HAL_GPIO_DeInit(SIGNAL_2_GPIO_Port, SIGNAL_2_Pin);
	HAL_GPIO_WritePin(SIGNAL_2_GPIO_Port, SIGNAL_2_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = SIGNAL_2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(SIGNAL_2_GPIO_Port, &GPIO_InitStruct);
#endif

#if (SIGNAL_3_Pin_En	== 1)  
  HAL_GPIO_DeInit(SIGNAL_3_GPIO_Port, SIGNAL_3_Pin);
	HAL_GPIO_WritePin(SIGNAL_3_GPIO_Port, SIGNAL_3_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = SIGNAL_3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(SIGNAL_3_GPIO_Port, &GPIO_InitStruct);
#endif

#if (SIGNAL_LED_Pin_En	== 1)  
  HAL_GPIO_DeInit(SIGNAL_LED_GPIO_Port, SIGNAL_LED_Pin);
	HAL_GPIO_WritePin(SIGNAL_LED_GPIO_Port, SIGNAL_LED_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = SIGNAL_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(SIGNAL_LED_GPIO_Port, &GPIO_InitStruct);
#endif

#if (HIGHT_LIGHT_Pin_En	== 1)  
  HAL_GPIO_DeInit(HIGHT_LIGHT_GPIO_Port, HIGHT_LIGHT_Pin);
	HAL_GPIO_WritePin(HIGHT_LIGHT_GPIO_Port, HIGHT_LIGHT_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = HIGHT_LIGHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(HIGHT_LIGHT_GPIO_Port, &GPIO_InitStruct);
#endif


}
/*###############################################################*/
/*###############################################################* Init -->*/
/*###############################################################*/
static void Init(void)
{
  Clock_GPIO_MspInit();
  
  USARTdrv = &SIGNAL_DRIVER_USART;
  /*Initialize the USART driver */
  USARTdrv->Initialize(USART_Signal_Callback);
  /*Power up the USART peripheral */
  USARTdrv->PowerControl(ARM_POWER_FULL);
  /*Configure the USART to 9600 Bits/sec */
  USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                    ARM_USART_DATA_BITS_8 |
                    ARM_USART_PARITY_NONE |
                    ARM_USART_STOP_BITS_1 |
                    ARM_USART_FLOW_CONTROL_NONE, 9600);
   
  /* Enable Receiver and Transmitter lines */
  USARTdrv->Control (ARM_USART_CONTROL_TX, 1);
  //USARTdrv->Control (ARM_USART_CONTROL_RX, 1);
  
	clock.status = ClockStatus_READY;
	clock.error = ClockError_OK;
    /* Read the BackUp Register 2 Data */
  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR2) != 0x0)
  {  
    Get_Store_Clock();
  } else {
    clock.state_pin = false;
    Store_Clock(12, 00);
    clock.curr_state_time.Seconds = 00;
  }
  if (Clock_TaskHandle == NULL) {
		Clock_TaskHandle = osThreadNew(Clock_Thread, NULL, &clock_attr);
	}   
  if (Signal_1_TaskHandle == NULL) {
		Signal_1_TaskHandle = osThreadNew(Signal_1_Thread, NULL, NULL);
	}   
  if (Signal_2_TaskHandle == NULL) {
		Signal_2_TaskHandle = osThreadNew(Signal_2_Thread, NULL, NULL);
	}   
   if (Signal_3_TaskHandle == NULL) {
		Signal_3_TaskHandle = osThreadNew(Signal_3_Thread, NULL, NULL);
	}   
   if (Signal_LED_TaskHandle == NULL) {
		Signal_LED_TaskHandle = osThreadNew(Signal_LED_Thread, NULL, NULL);
	}     
	
}
/*###############################################################*/
/*###############################################################* Send_Strings -->*/
/*###############################################################*/
static void Send_Strings_To_LCD (char *str1, char *str2){
  ClockMsgOutQueueTypeDef msg_out;
  int len1 = snprintf(Str_State, sizeof(Str_State), "%s", str1);
  int len2 = snprintf(Str_Note, sizeof(Str_Note), "%s", str2);
  msg_out.id = ClockMsgID_STRING_STATE_TO_LCD;
  msg_out.str_lcd = Str_State;
  msg_out.len = len1;
  //DEBUG("Send ClockMsgID_STRING_STATE_TO_LCD to Core\n");
  osMessageQueuePut(clock.queue_out, &msg_out, NULL, 0);
  msg_out.id = ClockMsgID_STRING_NOTE_TO_LCD;
  msg_out.str_lcd = Str_Note;
  msg_out.len = len2;
  //DEBUG("Send ClockMsgID_STRING_NOTE_TO_LCD to Core\n");
  osMessageQueuePut(clock.queue_out, &msg_out, NULL, 0);
}
/*###############################################################*/
/*###############################################################* Send_Clock_To_LCD -->*/
/*###############################################################*/
static void Send_Clock_To_LCD (void){
  ClockMsgOutQueueTypeDef msg_out;
  static char str[sizeof("00:00:00\0")];
  TimeStruct_To_ClockHandsString(&clock.curr_state_time, str);
  msg_out.id = ClockMsgID_CLOCK_HANDS_POSITION_TO_LCD;
  msg_out.str_lcd = str;
  msg_out.len = 6;
  DEBUG("Send ClockMsgID_CLOCK_HANDS_POSITION_TO_LCD to Core\n");
  osMessageQueuePut(clock.queue_out, &msg_out, NULL, 0); 
}

/*###############################################################*/
/*###############################################################* Signal_Message -->*/
/*###############################################################*/
static void Signal_Message (uint8_t *message, int num){
  uint16_t checksum = 0;
  message[0] = 0x7E; 
  message[1] = 0xFF; //VER
  message[2] = 0x06; //Len
  message[3] = 0x03; //CMD
  message[4] = 0x00; //Command Feedback
  message[5] = 0x00; //Parametr 1
  message[6] = num;  //Parametr 2
  checksum = 0;
  for(int i = 1; i < 7; i++) checksum += message[i];
  checksum = -checksum;
//  message[7] = (uint8_t)(checksum >> 8);
//  message[8] = (uint8_t)checksum;
  message[7] = 0xEF; 
//  for(int i = 0; i < 10; i++)
//  {
//    printf("%02X ", message[i]);
//  }
}
/*###############################################################*/
/*###############################################################* Clock_Thread -->*/
/*###############################################################*/
__NO_RETURN void Signal_1_Thread (void *argument) {
  static RTC_TimeTypeDef sTime = {0}; 
  static uint8_t message[12] = {0}; 
	while (1) {
   osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
   GetRTCTime(&sTime);
   DEBUG("Signal 9_21\n");
		if (sTime.Hours == 0)  Signal_Message(message, 24); else
			Signal_Message(message, sTime.Hours);
   USARTdrv->Send(message, 8);
    
#if (SIGNAL_9_21_Pin_En	== 1)  
  if ((sTime.Hours == 0)  
			||(sTime.Hours == 1)
			||(sTime.Hours == 2)
      ||(sTime.Hours == 3)
			||(sTime.Hours == 4)
			||(sTime.Hours == 5)
			||(sTime.Hours == 6)
			||(sTime.Hours == 7)
      ||(sTime.Hours == 8)
			||(sTime.Hours == 9)
			||(sTime.Hours == 10)
			||(sTime.Hours == 11)
      ||(sTime.Hours == 12)
      ||(sTime.Hours == 13)
      ||(sTime.Hours == 14)
      ||(sTime.Hours == 15)
      ||(sTime.Hours == 16)
      ||(sTime.Hours == 17)
      ||(sTime.Hours == 18)
      ||(sTime.Hours == 19)
      ||(sTime.Hours == 20)
			||(sTime.Hours == 21)
			||(sTime.Hours == 22)
			||(sTime.Hours == 23))
  {
    HAL_GPIO_WritePin(SIGNAL_9_21_GPIO_Port, SIGNAL_9_21_Pin, GPIO_PIN_SET);
    osDelay(SIGNAL_9_21_DELAY);
    HAL_GPIO_WritePin(SIGNAL_9_21_GPIO_Port, SIGNAL_9_21_Pin, GPIO_PIN_RESET);
  }  
#endif    	
  }
}



/*###############################################################*/
/*###############################################################* Clock_Thread -->*/
/*###############################################################*/
__NO_RETURN void Signal_LED_Thread (void *argument) {
	while (1) {
   osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
   DEBUG("Signal LED\n");
      
//Сигнал для светодиодов
#if (SIGNAL_LED_Pin_En	== 1)  
    HAL_GPIO_WritePin(SIGNAL_LED_GPIO_Port, SIGNAL_LED_Pin, GPIO_PIN_SET);
    osDelay(SIGNAL_LED_DELAY);
    HAL_GPIO_WritePin(SIGNAL_LED_GPIO_Port, SIGNAL_LED_Pin, GPIO_PIN_RESET);
#endif   
	
  }
}


/*###############################################################*/
/*###############################################################* Signal_2_Thread -->*/
/*###############################################################*/
__NO_RETURN void Signal_2_Thread (void *argument) {
  static RTC_TimeTypeDef sTime = {0};    
	while (1) {
   osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
   GetRTCTime(&sTime);
   DEBUG("Signal 2\n");
#if (SIGNAL_2_Pin_En	== 1)
  if (  ((sTime.Hours == 0) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
			  ((sTime.Hours == 1) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 2) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 3) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 4) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 5) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 6) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 7) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 8) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 9) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 10) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 11) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 12) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 13) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 14) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 15) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 16) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 17) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 18) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 19) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 20) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 21) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
			  ((sTime.Hours == 22) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
			  ((sTime.Hours == 23) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 24) && (sTime.Minutes == 59) && (sTime.Seconds == 50))
	     	)
    {
      HAL_GPIO_WritePin(SIGNAL_2_GPIO_Port, SIGNAL_2_Pin, GPIO_PIN_SET);
      osDelay(SIGNAL_2_DELAY);
      HAL_GPIO_WritePin(SIGNAL_2_GPIO_Port, SIGNAL_2_Pin, GPIO_PIN_RESET);
    } 
#endif    
  }
}

/*###############################################################*/
/*###############################################################* Signal_3_Thread -->*/
/*###############################################################*/
__NO_RETURN void Signal_3_Thread (void *argument) {
  static RTC_TimeTypeDef sTime = {0};    
	while (1) {
   osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
   GetRTCTime(&sTime);
   DEBUG("Signal 3\n");
#if (SIGNAL_3_Pin_En	== 1)
  if (  ((sTime.Hours == 0) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
			  ((sTime.Hours == 1) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 2) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 3) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 4) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 5) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 6) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 7) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 8) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 9) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 10) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 11) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 12) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 13) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 14) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 15) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 16) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 17) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 18) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 19) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 20) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 21) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
			  ((sTime.Hours == 22) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
			  ((sTime.Hours == 23) && (sTime.Minutes == 59) && (sTime.Seconds == 50))||
				((sTime.Hours == 24) && (sTime.Minutes == 59) && (sTime.Seconds == 50))
	     	)
    {
      HAL_GPIO_WritePin(SIGNAL_3_GPIO_Port, SIGNAL_3_Pin, GPIO_PIN_SET);
      osDelay(SIGNAL_3_DELAY);
      HAL_GPIO_WritePin(SIGNAL_3_GPIO_Port, SIGNAL_3_Pin, GPIO_PIN_RESET);
    } 
#endif    
  }
}



/*###############################################################*/
/*###############################################################* Clock_Thread -->*/
/*###############################################################*/
__NO_RETURN void Clock_Thread (void *argument) {
  osStatus_t status;


  ClockMsgInQueueTypeDef msg_in;
  ClockMsgOutQueueTypeDef msg_out;
  //ClockMsgQueueTypeDef msg; 
  int cnt = 1000;
  //Delay(1000);
  Send_Clock_To_LCD();
	while (1) {
    Delay(10);
		
    if(clock.status == ClockStatus_STOP){
      //записываем в строки состояний на дисплее
      Send_Strings_To_LCD("Stop hands","Stop hands");
      msg_out.id = ClockMsgID_STOP_OK;
      DEBUG("Send ClockMsgID_STOP_OK to Core\n");
      osMessageQueuePut(clock.queue_out, &msg_out, NULL, osWaitForever);
      osThreadYield();
      while (1)
      {
        if (osMessageQueueGet(clock.queue_in, &msg_in, NULL, osWaitForever) == osOK) {
          if(msg_in.id == ClockMsgID_START){
            //отчищаем строки состояния на дисплее
            Send_Strings_To_LCD("","");
            clock.status = ClockStatus_READY;
            DEBUG("Start Clock\n");
            break;
          }
        } 
      }    
    }
    
    
    if(++cnt > 20){
      cnt = 0;
      Send_Clock_To_LCD();
      osThreadYield();      
    }

    
		status = osMessageQueueGet(clock.queue_in, &msg_in, NULL, 0);   // wait for message
		if (status == osOK) {
      if(msg_in.id == ClockMsgID_MOVE_CLOCK_HANDS){
        //получили время
        memcpy(&clock.end_state_time, &msg_in.time, sizeof(msg_in.time));
        clock.status = ClockStatus_MOVE_CLOCK_HANDS;
        msg_out.id = ClockMsgID_MOVE_CLOCK_HANDS_START;
        DEBUG("Send ClockMsgID_MOVE_CLOCK_HANDS_START to Core\n");
        osMessageQueuePut(clock.queue_out, &msg_out, NULL, osWaitForever);
        if (Move_Clock_Hands() != 0) {
          DEBUG("Receive command Stop Clock while move clock hands\n");
        } else {
          //отчищаем строки состояния на дисплее
          Send_Strings_To_LCD("","");
          //reset status
          clock.status = ClockStatus_READY; 
        }
      }
      if(msg_in.id == ClockMsgID_STOP){
          clock.status = ClockStatus_STOP;
          DEBUG("Stop Clock\n");
      }
    }
	}

}
/*###############################################################*/
/*###############################################################* Move_Clock_Hands -->*/
/*###############################################################*/
int Move_Clock_Hands(void) {
  //считаем на сколько надо сдвинуть стрелки
  #define MIN 60
  #define SEC 60
  int len;
  ClockMsgOutQueueTypeDef msg_out;
  ClockMsgOutQueueTypeDef msg_in;
  int curr = clock.curr_state_time.Hours*MIN + clock.curr_state_time.Minutes;
  int end = clock.end_state_time.Hours*MIN + clock.end_state_time.Minutes;
  int tick_forth = ((end - curr) >= 0)?(end - curr):((end + 12*MIN) - curr);
  int tick_back = ((end - curr) > 0)?(12*MIN - tick_forth):(curr - end);
  if(curr == end) return 0;

//  DEBUG("curr: %d\n", curr);
//  DEBUG("end: %d\n", end);
//  DEBUG("tick_forth: %d\n", tick_forth);
//  DEBUG("tick_back: %d\n", tick_back);  
  if((float)(tick_back * SEC) <= (float)tick_forth * ((float)CLOCK_TICK_DELAY/1000.0f + (float)CLOCK_AFTER_TICK_DELAY/1000.0f)) {
    DEBUG("WAIT CLOCK HANDS, %d minutes\n", tick_back);  
    int cnt = 2000;
    while(1){
      if(cnt++ > 1000){
        Send_Clock_To_LCD();
        Send_Strings_To_LCD("Waiting RTC","Waiting RTC");
        cnt = 0;
      }
      RTC_TimeTypeDef time;
      GetRTCTime(&time);
      #ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT      
      #else
      if(time.Hours == 0)  time.Hours = 12;
      if(time.Hours > 12) time.Hours -= 12;
      #endif 
      if((time.Hours == clock.curr_state_time.Hours) &&
        (time.Minutes == clock.curr_state_time.Minutes)){
          return 0;
        }
      if (osMessageQueueGet(clock.queue_in, &msg_in, NULL, 0) == osOK) {
        if(msg_in.id == ClockMsgID_STOP){
          clock.status = ClockStatus_STOP;
          DEBUG("Stop Clock\n");
          return 1;
        }
      }         
      Delay(1);
    }
  } else {
    memcpy(&clock.start_state_time, &clock.curr_state_time, sizeof(clock.curr_state_time));
    
    DEBUG("MOVE CLOCK HANDS, %d minutes\n", tick_forth);
    len = snprintf(Str_State, sizeof(Str_State), "MOVE CLOCK HANDS");
    msg_out.id = ClockMsgID_STRING_STATE_TO_LCD;
    msg_out.str_lcd = Str_State;
    msg_out.len = len;
    DEBUG("Send ClockMsgID_STRING_STATE_TO_LCD to Core\n");
    osMessageQueuePut(clock.queue_out, &msg_out, NULL, 0);

    int hours = clock.curr_state_time.Hours;
    int minutes = clock.curr_state_time.Minutes;
    for(int i = 0; i < tick_forth; i++)
    { 
      //DEBUG("Tick Clock Hands, num %d, left %d minutes\n", i, tick_forth - i);
      len = snprintf(Str_Note, sizeof(Str_Note), "Tick num %d, left %d", i, tick_forth - i);
      msg_out.id = ClockMsgID_STRING_NOTE_TO_LCD;
      msg_out.str_lcd = Str_Note;
      msg_out.len = len;
      DEBUG("Send ClockMsgID_STRING_NOTE_TO_LCD to Core\n");
      osMessageQueuePut(clock.queue_out, &msg_out, NULL, 0);
			//если не заблокирован модуль, то тикаем стрелками
			if(sett_store.mode_rtc != MODE_RTC_BLOCK){
				Tick_Clock_Hands();
			}
      
      minutes++;
      if (minutes > 59){
        minutes = 0;
        hours++;
        if (hours > 12) hours = 1;
      }
      Store_Clock(hours, minutes); //сохранаяем положение часов
      Send_Clock_To_LCD();
      if (osMessageQueueGet(clock.queue_in, &msg_in, NULL, 0) == osOK) {
        if(msg_in.id == ClockMsgID_STOP){
          clock.status = ClockStatus_STOP;
          DEBUG("Stop Clock\n");
          return 1;
        }
      }
    }
  }
  return 0;
}

/*###############################################################*/
/*###############################################################* Tick_Clock_Hands -->*/
/*###############################################################*/
static void Tick_Clock_Hands(void) {
  if(clock.state_pin){
    #if (MOTOR_11_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_11_GPIO_Port, MOTOR_11_Pin, GPIO_PIN_SET);
    #endif  
    #if (MOTOR_12_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_12_GPIO_Port, MOTOR_12_Pin, GPIO_PIN_SET);
    #endif      
    #if (MOTOR_13_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_13_GPIO_Port, MOTOR_13_Pin, GPIO_PIN_SET);
    #endif  
  } else {
    #if (MOTOR_21_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_21_GPIO_Port, MOTOR_21_Pin, GPIO_PIN_SET);
    #endif  
    #if (MOTOR_22_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_22_GPIO_Port, MOTOR_22_Pin, GPIO_PIN_SET);
    #endif      
    #if (MOTOR_23_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_23_GPIO_Port, MOTOR_23_Pin, GPIO_PIN_SET);
    #endif  
  }   
  osDelay(CLOCK_TICK_DELAY);
  if(clock.state_pin){
    #if (MOTOR_11_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_11_GPIO_Port, MOTOR_11_Pin, GPIO_PIN_RESET);
    #endif  
    #if (MOTOR_12_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_12_GPIO_Port, MOTOR_12_Pin, GPIO_PIN_RESET);
    #endif      
    #if (MOTOR_13_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_13_GPIO_Port, MOTOR_13_Pin, GPIO_PIN_RESET);
    #endif  
  } else {
    #if (MOTOR_21_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_21_GPIO_Port, MOTOR_21_Pin, GPIO_PIN_RESET);
    #endif  
    #if (MOTOR_22_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_22_GPIO_Port, MOTOR_22_Pin, GPIO_PIN_RESET);
    #endif      
    #if (MOTOR_23_Pin_En	== 1)
    HAL_GPIO_WritePin(MOTOR_23_GPIO_Port, MOTOR_23_Pin, GPIO_PIN_RESET);
    #endif  
  }  
  clock.state_pin = !clock.state_pin;
  #if (CLOCK_AFTER_TICK_DELAY	!= 0)
    osDelay(CLOCK_AFTER_TICK_DELAY);
  #endif 
}
//Каждая минута импульс длительностью 4 с поочередно то на одном, то на другом Пине. 
//Запоминать надо не только текущее положение стрелок но и состояние пинов. 
//Для проигрывания мелодий использовать один пин для подачи импульса на плеер каждый час в период с 9 до 21 час, 
//другой пин только в 9,12,15,18 и 21 час.
/*###############################################################*/
/*###############################################################* Store_Clock -->*/
/*###############################################################*/
/**
  * @brief 
  */
void Store_Clock(uint8_t hours, uint8_t minutes)
{
  uint32_t store = 0;
  clock.curr_state_time.Hours = hours;
  clock.curr_state_time.Minutes = minutes;
  store = (uint32_t)(clock.curr_state_time.Hours << 8)|(clock.curr_state_time.Minutes)|(clock.state_pin << 16);
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR2, store);
}

/*###############################################################*/
/*###############################################################* Get_Store_Clock -->*/
/*###############################################################*/
/**
  * @brief 
  */
void Get_Store_Clock(void)
{
  uint32_t store = 0; 
  store = HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR2);
  clock.curr_state_time.Hours = (store >> 8)&0x000000FF;
  clock.curr_state_time.Minutes = store & 0x000000FF;
  clock.state_pin = (store >> 16)&0x000000FF;
}

/*###############################################################*/
/*###############################################################* GetClockTime -->*/
/*###############################################################*/
void GetClockTime(RTC_TimeTypeDef *time)
{ 
  memcpy(time, &clock.curr_state_time, sizeof(RTC_TimeTypeDef));
}

/*###############################################################*/
/*###############################################################* debug_gps_packet -->*/
/*###############################################################*/
static void USART_Signal_Callback(uint32_t event)
{
  uint32_t mask;	
	  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
         ARM_USART_EVENT_TRANSFER_COMPLETE |
         ARM_USART_EVENT_SEND_COMPLETE     |
         ARM_USART_EVENT_TX_COMPLETE       ;
	
	
	
  if (event & mask) {
    /* Success: Wakeup Thread */
    //osThreadFlagsSet(GPS_TaskHandle, 0x00000001U);
  }
}
