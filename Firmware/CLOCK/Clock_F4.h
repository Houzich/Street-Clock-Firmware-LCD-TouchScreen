/**
******************************************************************************
* File Name          : Clock_F4.h
******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLOCK_H
#define __CLOCK_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "stm32f4xx_hal.h" 


/* Exported types ------------------------------------------------------------*/
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#define GPIO_PORT0      GPIOA
#define GPIO_PORT1      GPIOB
#define GPIO_PORT2      GPIOC
#define GPIO_PORT3      GPIOD
#define GPIO_PORT4      GPIOE
#define GPIO_PORT5      GPIOF
#define GPIO_PORT6      GPIOG
#define GPIO_PORT7      GPIOH
#define GPIO_PORT8      GPIOI
#define GPIO_PORT9      GPIOJ
#define GPIO_PORT10     GPIOK

#define GPIO_PORT(num)  			GPIO_PORT##num
#define GPIO_PIN(num)  				(uint16_t)(1 << num)

//   <o> Delay Tick Clock (ms) <1-1000000>
//   <i> Default: 4000 (4000 ms tick)
#ifndef CLOCK_TICK_DELAY
#define CLOCK_TICK_DELAY               3000
#endif

//   <o> Delay After Tick Clock (ms) <0-1000000>
//   <i> Default: 10 (10 ms after tick)
#ifndef CLOCK_AFTER_TICK_DELAY
#define CLOCK_AFTER_TICK_DELAY         10
#endif

// <e> MOTOR_11 Pin
#define MOTOR_11_Pin_En	1

#if (MOTOR_11_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define MOTOR_11_Pin_Num	13
#define MOTOR_11_Pin	GPIO_PIN(MOTOR_11_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define MOTOR_11_GPIO_Port GPIO_PORT(6)

#else
#define MOTOR_11_Pin_Num 0xFF
#endif
//</e>

// <e> MOTOR_12 Pin
#define MOTOR_12_Pin_En	1

#if (MOTOR_12_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define MOTOR_12_Pin_Num	2
#define MOTOR_12_Pin	GPIO_PIN(MOTOR_12_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define MOTOR_12_GPIO_Port GPIO_PORT(4)

#else
#define MOTOR_12_Pin_Num 0xFF
#endif
//</e>

// <e> MOTOR_13 Pin
#define MOTOR_13_Pin_En	1

#if (MOTOR_13_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define MOTOR_13_Pin_Num	4
#define MOTOR_13_Pin	GPIO_PIN(MOTOR_13_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define MOTOR_13_GPIO_Port GPIO_PORT(4)

#else
#define MOTOR_13_Pin_Num 0xFF
#endif
//</e>


// <e> MOTOR_21 Pin
#define MOTOR_21_Pin_En	1
#if (MOTOR_21_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define MOTOR_21_Pin_Num	14
#define MOTOR_21_Pin	GPIO_PIN(MOTOR_21_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define MOTOR_21_GPIO_Port GPIO_PORT(6)


#else
#define MOTOR_21_Pin_Num 0xFF
#endif
//</e>

// <e> MOTOR_22 Pin
#define MOTOR_22_Pin_En	1
#if (MOTOR_22_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define MOTOR_22_Pin_Num	3
#define MOTOR_22_Pin	GPIO_PIN(MOTOR_22_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define MOTOR_22_GPIO_Port GPIO_PORT(4)


#else
#define MOTOR_22_Pin_Num 0xFF
#endif
//</e>

// <e> MOTOR_23 Pin
#define MOTOR_23_Pin_En	1
#if (MOTOR_23_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define MOTOR_23_Pin_Num	5
#define MOTOR_23_Pin	GPIO_PIN(MOTOR_23_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define MOTOR_23_GPIO_Port GPIO_PORT(4)


#else
#define MOTOR_23_Pin_Num 0xFF
#endif
//</e>


//   <o> Signal 9_21 Delay (ms) <0-1000000>
//   <i> Default: 5000 (5000 ms tick)
#ifndef SIGNAL_9_21_DELAY
#define SIGNAL_9_21_DELAY               5000
#endif

//   <o> Signal 2 Delay (ms) <0-1000000>
//   <i> Default: 5000 (5000 ms tick)
#ifndef SIGNAL_2_DELAY
#define SIGNAL_2_DELAY               5000
#endif

//   <o> Signal 3 Delay (ms) <0-1000000>
//   <i> Default: 5000 (5000 ms tick)
#ifndef SIGNAL_3_DELAY
#define SIGNAL_3_DELAY               5000
#endif

//   <o> Signal LED Delay (ms) <0-1000000>
//   <i> Default: 500 (500 ms tick)
#ifndef SIGNAL_LED_DELAY
#define SIGNAL_LED_DELAY               5000
#endif 

// <e> SIGNAL_9_21 Pin
#define SIGNAL_9_21_Pin_En	1

#if (SIGNAL_9_21_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define SIGNAL_9_21_Pin_Num	11
#define SIGNAL_9_21_Pin	GPIO_PIN(SIGNAL_9_21_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define SIGNAL_9_21_GPIO_Port GPIO_PORT(2)

#else
#define SIGNAL_9_21_Pin_Num 0xFF
#endif
//</e>


// <e> SIGNAL_2 Pin
#define SIGNAL_2_Pin_En	1

#if (SIGNAL_2_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define SIGNAL_2_Pin_Num	4
#define SIGNAL_2_Pin	GPIO_PIN(SIGNAL_2_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define SIGNAL_2_GPIO_Port GPIO_PORT(3)

#else
#define SIGNAL_2_Pin_Num 0xFF
#endif
//</e>

// <e> SIGNAL_3 Pin
#define SIGNAL_3_Pin_En	1

#if (SIGNAL_3_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define SIGNAL_3_Pin_Num	7
#define SIGNAL_3_Pin	GPIO_PIN(SIGNAL_3_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define SIGNAL_3_GPIO_Port GPIO_PORT(3)

#else
#define SIGNAL_3_Pin_Num 0xFF
#endif
//</e>

// <e> SIGNAL_LED Pin
#define SIGNAL_LED_Pin_En	1

#if (SIGNAL_LED_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define SIGNAL_LED_Pin_Num	7
#define SIGNAL_LED_Pin	GPIO_PIN(SIGNAL_LED_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define SIGNAL_LED_GPIO_Port GPIO_PORT(1)

#else
#define SIGNAL_LED_Pin_Num 0xFF
#endif
//</e>

// <e> HIGHT_LIGHT Pin
#define HIGHT_LIGHT_Pin_En	1

#if (HIGHT_LIGHT_Pin_En	== 1)
//   <o> Pin <0=>P0 <1=>P1 <2=>P2 <3=>P3 <4=>P4 <5=>P5 <6=>P6
//       <7=>P7   <8=>P8   <9=>P9 <10=>P10 <11=>P11 <12=>P12 <13=>P13 <14=>P14 <15=>P15
#define HIGHT_LIGHT_Pin_Num	8
#define HIGHT_LIGHT_Pin	GPIO_PIN(HIGHT_LIGHT_Pin_Num)


//   <o> Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//   <7=>GPIOH <8=>GPIOI <9=>GPIOJ <10=>GPIOK
#define HIGHT_LIGHT_GPIO_Port GPIO_PORT(2)

#else
#define HIGHT_LIGHT_Pin_Num 0xFF
#endif

//</e>


#define USART(num)  			Driver_USART##num
//   <o> USART Signal <1=>USART1 <2=>USART2 <3=>USART3 <4=>UART4 <5=>UART5 <6=>USART6 <7=>UART7
//   <8=>UART8 <9=>UART9 <10=>UART10
#define SIGNAL_DRIVER_USART	USART(5)

//   <o> Time test mode (s) <0-2147483647>
//   <i> Default: 120 (120 s tick). Max value: 2147483647. 
#ifndef TIME_TEST_MODE
#define TIME_TEST_MODE               1209000 //количество секунд тестового режима до блокировки модулю
#endif
/**
* @brief  Clock acceptor status
*/
typedef enum __ClockMsgIDEnumTypeDef{
  ClockMsgID_MOVE_CLOCK_HANDS_START = 0x01,
  ClockMsgID_MOVE_CLOCK_HANDS = 0x02,
  ClockMsgID_MOVE_CLOCK_HANDS_END = 0x03,
  ClockMsgID_CLOCK_HANDS_POSITION_TO_LCD = 0x04,
  ClockMsgID_STRING_STATE_TO_LCD = 0x05,
  ClockMsgID_STRING_NOTE_TO_LCD = 0x06,
  ClockMsgID_STOP = 0x07,
  ClockMsgID_STOP_OK = 0x08,
  ClockMsgID_START = 0x09,
  ClockMsgID_START_OK = 0x10,
}ClockMsgID;
/**
* @brief  Clock acceptor status
*/
typedef enum __ClockStatusEnumTypeDef{
	ClockStatus_READY = 0x00,
	ClockStatus_MOVE_CLOCK_HANDS = 0x01,
	ClockStatus_STOP = 0x02,
}ClockStatus;
/**
* @brief  Terminal bill acceptor error
*/
typedef enum __ClockErrorEnumTypeDef{
	ClockError_OK = 0x00,
}ClockError;


#define CLOCK_MSG_IN_QUEUE_OBJECTS 10
typedef struct {                                // object data type
  ClockMsgID id;
  RTC_TimeTypeDef time;
} ClockMsgInQueueTypeDef;

#define CLOCK_MSG_OUT_QUEUE_OBJECTS 20
typedef struct {                                // object data type
  ClockMsgID id;
  char *str_lcd;
  int len;
} ClockMsgOutQueueTypeDef;

#define CLOCK_MSG_QUEUE_OBJECTS 10
typedef struct {                                // object data type
  ClockMsgID id;
} ClockMsgQueueTypeDef;

/** 
  * @brief  Clock state structure
  */
typedef struct __STREETCLOCK_HandleClockStatekTypeDef
{
	int32_t position_hour_hand;
	int32_t position_minute_hand;

} STREETCLOCK_HandleClockStateTypeDef;
/** 
  * @brief  Clock structure
  */
typedef struct __STREETCLOCK_HandleClockTypeDef
{
  void(*init)(void);
	ClockStatus status;
	ClockError error;
  
	STREETCLOCK_HandleClockStateTypeDef curr_state;
	STREETCLOCK_HandleClockStateTypeDef start_state;
  STREETCLOCK_HandleClockStateTypeDef end_state;

  RTC_TimeTypeDef curr_state_time;
  RTC_TimeTypeDef start_state_time;
  RTC_TimeTypeDef end_state_time;
  
  osMessageQueueId_t queue_in;
  osMessageQueueId_t queue_out;
  osMessageQueueId_t queue;
  
  bool state_pin;
  ClockMsgInQueueTypeDef msg_in;
  ClockMsgOutQueueTypeDef msg_out;
  ClockMsgQueueTypeDef msg;
  
} STREETCLOCK_HandleClockTypeDef;
/* Exported constants --------------------------------------------------------*/
extern osThreadId_t Signal_1_TaskHandle;
extern osThreadId_t Signal_2_TaskHandle;
extern osThreadId_t Signal_3_TaskHandle;
extern osThreadId_t Signal_LED_TaskHandle;
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
__NO_RETURN void Clock_Thread (void *argument);
__NO_RETURN void Signal_1_Thread (void *argument);
__NO_RETURN void Signal_2_Thread (void *argument);
__NO_RETURN void Signal_3_Thread (void *argument);
__NO_RETURN void Signal_LED_Thread (void *argument);
int Move_Clock_Hands(void);  
void GetClockTime(RTC_TimeTypeDef *time);
void Store_Clock(uint8_t hours, uint8_t minutes); 
#ifdef __cplusplus
}
#endif

#endif /*__CLOCK_H*/
