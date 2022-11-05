/**
  ******************************************************************************
  * File Name          : GPS.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H
#define __GPS_H
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
//B4,F6

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#define USART(num)  			Driver_USART##num


//   <o> USART <1=>USART1 <2=>USART2 <3=>USART3 <4=>UART4 <5=>UART5 <6=>USART6 <7=>UART7
//   <8=>UART8 <9=>UART9 <10=>UART10
#define GPS_DRIVER_USART	USART(7)


/* Exported constants --------------------------------------------------------*/
/**
* @brief  GPS message
*/
typedef enum __GPSMsgIDEnumTypeDef{
	GPSMsgID_UP_RECEIVE = 1 << 1,
}GPSMsgID;
/**
* @brief  GPS status
*/
typedef enum __GPSStatusEnumTypeDef{
	GPSStatus_READY = 0x00,
}GPSStatus;
/**
* @brief  GPS error
*/
typedef enum __GPSErrorEnumTypeDef{
	GPSError_OK = 0x00,
}GPSError;


#define GPS_MSG_IN_QUEUE_OBJECTS 4
typedef struct {                                // object data type
  GPSMsgID id;
} GPSMsgInQueueTypeDef;

#define GPS_MSG_OUT_QUEUE_OBJECTS 4
typedef struct {                                // object data type
  GPSMsgID id;
} GPSMsgOutQueueTypeDef;

#define GPS_MSG_QUEUE_OBJECTS 4
typedef struct {                                // object data type
  GPSMsgID id;
} GPSMsgQueueTypeDef;

#pragma pack(1)
typedef struct __GPSTimeTypeDef
{
  char hh[2];
  char mm[2];
  char ss[2];
  char dot[1];
  char sss[3];
} GPSTimeTypeDef;
#pragma pack()

#pragma pack(1)
typedef struct __GPSDateTypeDef
{
  char dd[2];
  char mm[2];
  char yy[2];
} GPSDateTypeDef;
#pragma pack()
/**
  * @brief  GPS packet structure
  */
#pragma pack(1)
typedef struct __GPSPacketTypeDef
{
  char Message_ID[6];
  char Message_ID_Comma[1];
  char UTC_Time[9];
  char UTC_Time_Comma[1];
  char Status[1];
  char Status_Comma[1];
  char Latitude[10];
  char Latitude_Comma[1];
  char N_S_Indicator[1];
  char N_S_Indicator_Comma[1];
  char Longitude[11];
  char Longitude_Comma[1];
  char E_W_Indicator[1];
  char E_W_Indicator_Comma[1];
  char Speed_over_ground[5];
  char Speed_over_ground_Comma[1];
  //char Course_over_ground[5];
  char Course_over_ground_Comma[1];
  char Date[6];
  char Date_Comma[1];
//  char Magnetic_variation[0];
	char Magnetic_variation_Comma[1];
//  char Variation_sense[0];	
	char Variation_sense_Comma[1];
  char Mode[1];
  char Checksum[3];
  char End_Packet[2];
} GPSPacketTypeDef;
#pragma pack()


/**
  * @brief  GPS structure
  */
typedef struct __STREETCLOCK_HandleGPSTypeDef
{
	void(*init)(void);
	GPSStatus status;
	GPSError error;
  
  osMessageQueueId_t queue_in;
  osMessageQueueId_t queue_out;
  osMessageQueueId_t queue;
  
  GPSMsgInQueueTypeDef msg_in;
  GPSMsgOutQueueTypeDef msg_out;
  GPSMsgQueueTypeDef msg;
  bool gps_signal;
  bool start;
} STREETCLOCK_HandleGPSTypeDef;



/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /*__GPS_H*/
