/**
  ******************************************************************************
  * File Name          : GPS.c
  ******************************************************************************
  */
  /* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cmsis_os2.h"
#include "Street_Clock.h"
#include "Driver_USART.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//used when debugging, declared in the file Street_Clock.h
#ifdef DEBUG_GPS
#undef DEBUG /* DEBUG */
#undef DEBUG_PRINT /* DEBUG_PRINT */
#define DEBUG(...)		do {debug_header("GPS: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(x)		debug_print x;
#else
#define DEBUG(...)
#define DEBUG_PRINT(...)
#endif // DEBUG_GPS
/* Private variables ---------------------------------------------------------*/
static ARM_DRIVER_USART *USARTdrv;
/* USART Driver */
extern ARM_DRIVER_USART GPS_DRIVER_USART;
extern STREETCLOCK_HandleStoreSettingsTypeDef sett_store;
/* Private function prototypes -----------------------------------------------*/
static void Init(void);
static void debug_gps_packet(char *str);
static void USART_GPS_Callback(uint32_t event);
/* Exported functions --------------------------------------------------------*/
STREETCLOCK_HandleGPSTypeDef gps = {
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

#define GPS_STK_SZ (1024*1)
uint64_t GPS_stk[GPS_STK_SZ / 8];
const osThreadAttr_t GPS_attr = {
  .stack_mem  = &GPS_stk[0],
  .stack_size = sizeof(GPS_stk),
	//.priority = osPriorityHigh
};
osThreadId_t GPS_TaskHandle = NULL;


static int Check_DateTime (RTC_DateTypeDef *date, RTC_TimeTypeDef *time) {

#ifdef STREET_CLOCK_RTC_12_HOUR_FORMAT
  if (time->Hours > 12) {
    time->TimeFormat = RTC_HOURFORMAT12_PM;
    time->Hours -=12;
  } else {
    time->TimeFormat = RTC_HOURFORMAT12_AM;
  }
  
  if (time->Hours > 12) return 1;
  if (time->Minutes > 59) return 1;
  if (time->Seconds > 59) return 1;
  //добавляем часовой пояс
  time->Hours += 3;
  if (time->Hours > 12) {
    if(time->TimeFormat == RTC_HOURFORMAT12_AM) time->TimeFormat = RTC_HOURFORMAT12_PM;
    else time->TimeFormat = RTC_HOURFORMAT12_AM;
    time->Hours -=12;
    if(time->Hours == 0)  time->Hours = 12;
  }
#else
  if (time->Hours > 23) return 1;
  if (time->Minutes > 59) return 1;
  if (time->Seconds > 59) return 1;

  int hours = time->Hours;
  hours += sett_store.UTC;

  if (hours > 23){ 
    hours -= 24;
  }
  if (hours < 0){ 
    hours += 24;
  }
  time->Hours = hours;
  time->TimeFormat = RTC_HOURFORMAT_24;
#endif
  return 0;
}

void GPS_Data_To_DateTime (GPSPacketTypeDef  *packet, RTC_DateTypeDef *date, RTC_TimeTypeDef *time) {
      static char datestr[20]; 
      static char timestr[20]; 
      GPSTimeTypeDef *time_gps = (GPSTimeTypeDef *)packet->UTC_Time;
      GPSDateTypeDef *date_gps = (GPSDateTypeDef *)packet->Date;
      memset(datestr, 0, sizeof(datestr));
      memset(timestr, 0, sizeof(timestr));
      memcpy((void *)&datestr[0], (void *)date_gps->dd, 2);
      memcpy((void *)&datestr[3], (void *)date_gps->mm, 2);
      memcpy((void *)&datestr[6], (void *)date_gps->yy, 2);
  
      memcpy((void *)&timestr[0], (void *)time_gps->hh, 2);
      memcpy((void *)&timestr[3], (void *)time_gps->mm, 2);
      memcpy((void *)&timestr[6], (void *)time_gps->ss, 2);
  
			date->Date = atoi(&datestr[0]);
			date->Month = atoi(&datestr[3]);	
  		date->Year = atoi(&datestr[6]);

			time->Hours = atoi(&timestr[0]);
			time->Minutes = atoi(&timestr[3]);
			time->Seconds = atoi(&timestr[6]);
}

/*###############################################################*/
/*###############################################################* GPS_Thread -->*/
/*###############################################################*/
__NO_RETURN void GPS_Thread (void *argument) {
//  osStatus_t status;
//  GPSMsgInQueueTypeDef msg_in;
//  GPSMsgOutQueueTypeDef msg_out;
//  GPSMsgQueueTypeDef msg; 
  static char buff[200];

  //char *str_test = "$GPRMC053740.000A2503.6319N12136.0099E2.6979.65100106AX\r\n";
	//char *packet_str = str_test;
	//char *packet_st;
  char chr;
  while (1) {
    int count = 0;
    memset(buff, 0, sizeof(buff));
    while (1)
    {
        USARTdrv->Receive(&chr, 1);          /* Get byte from UART */
        osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
        buff[count] = chr;
        count++;
        if (count > (sizeof(buff) - 4)){
          count = 0;
          memset(buff, 0, sizeof(buff));
        }
       if((chr == '\n')&&(buff[count - 2] == '\r')) break;
       //if(chr == '\n') break;
    }
    //получили строку
    //ищем последний символ, это будет длина строки
    //int len = strchr (packet_str,'\n') - packet_str + 1;
    int len = count;
		
		//если размер пакета не равен "правильному", значит сигнала нет
		if((memcmp(buff, "$GPRMC", 6) == 0)&&(len != sizeof(GPSPacketTypeDef))){
				gps.gps_signal = false;
		}
		
    if(len == sizeof(GPSPacketTypeDef)){
      //получили правильный пакет
      #ifdef DEBUG_GPS
				debug_gps_packet(buff);
      #endif // DEBUG_GPS      
      GPSPacketTypeDef  *packet = (GPSPacketTypeDef *)buff;
			if(memcmp(packet->Message_ID, "$GPRMC", 6) != 0) continue;
			if(memcmp(packet->Status, "A", 1) != 0) continue;
			gps.gps_signal = true;
      RTC_TimeTypeDef time;
      RTC_DateTypeDef date;
      GPS_Data_To_DateTime(packet, &date, &time);
      //проверили перевели в часовой пояс
      if(!Check_DateTime(&date, &time)){
        if(!gps.start){
					Set_RTC_Date(&date);
          Set_RTC_Time(&time);
          gps.start = true;
        }else if ((time.Seconds == 00)&&(time.Minutes == 00)&&(time.Hours == 5))
        {
					Set_RTC_Date(&date);
          Set_RTC_Time(&time);
        }
      }
      //Delay(1000);
    }
    //ach = strchr (src,ch);
	}

}

				
/*###############################################################*/
/*###############################################################* Init -->*/
/*###############################################################*/
static void Init(void)
{
  USARTdrv = &GPS_DRIVER_USART;
  /*Initialize the USART driver */
  USARTdrv->Initialize(USART_GPS_Callback);
  /*Power up the USART peripheral */
  USARTdrv->PowerControl(ARM_POWER_FULL);
  /*Configure the USART to 9600 Bits/sec */
  USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                    ARM_USART_DATA_BITS_8 |
                    ARM_USART_PARITY_NONE |
                    ARM_USART_STOP_BITS_1 |
                    ARM_USART_FLOW_CONTROL_NONE, 9600);
   
  /* Enable Receiver and Transmitter lines */
  //USARTdrv->Control (ARM_USART_CONTROL_TX, 1);
  USARTdrv->Control (ARM_USART_CONTROL_RX, 1);
  
  
	gps.status = GPSStatus_READY;
	gps.error = GPSError_OK;
	gps.start = false;
	gps.gps_signal = false;
  if (GPS_TaskHandle == NULL) {
		GPS_TaskHandle = osThreadNew(GPS_Thread, NULL, &GPS_attr);
	}    
}

/*###############################################################*/
/*###############################################################* debug_gps_packet_field -->*/
/*###############################################################*/
static void debug_gps_packet_field(char *name, char *field, uint32_t len)
{
  DEBUG("%s", name);
  for(int i = 0; i < len; i++) putchar(field[i]);
  putchar('\n');
}
/*###############################################################*/
/*###############################################################* debug_gps_packet -->*/
/*###############################################################*/
static void debug_gps_packet(char *str)
{
  GPSPacketTypeDef  *packet = (GPSPacketTypeDef *)str;
  debug_gps_packet_field("Message_ID: ", packet->Message_ID, sizeof(packet->Message_ID));
  debug_gps_packet_field("UTC_Time: ", packet->UTC_Time, sizeof(packet->UTC_Time));
  debug_gps_packet_field("Status: ", packet->Status, sizeof(packet->Status));
  debug_gps_packet_field("Latitude: ", packet->Latitude, sizeof(packet->Latitude));
  debug_gps_packet_field("N_S_Indicator: ", packet->N_S_Indicator, sizeof(packet->N_S_Indicator));
  debug_gps_packet_field("Longitude: ", packet->Longitude, sizeof(packet->Longitude));
  debug_gps_packet_field("E_W_Indicator: ", packet->E_W_Indicator, sizeof(packet->E_W_Indicator));
  debug_gps_packet_field("Speed_over_ground: ", packet->Speed_over_ground, sizeof(packet->Speed_over_ground));
//  debug_gps_packet_field("Course_over_ground: ", packet->Course_over_ground, sizeof(packet->Course_over_ground));
  debug_gps_packet_field("Date: ", packet->Date, sizeof(packet->Date));
//  debug_gps_packet_field("Magnetic_variation: ", packet->Magnetic_variation, sizeof(packet->Magnetic_variation));
//  debug_gps_packet_field("Variation_sense: ", packet->Variation_sense, sizeof(packet->Variation_sense));
  debug_gps_packet_field("Mode: ", packet->Mode, sizeof(packet->Mode));
  debug_gps_packet_field("Checksum: ", packet->Checksum, sizeof(packet->Checksum));
  debug_gps_packet_field("End_Packet: ", packet->End_Packet, sizeof(packet->End_Packet)); 
}
/*###############################################################*/
/*###############################################################* debug_gps_packet -->*/
/*###############################################################*/
static void USART_GPS_Callback(uint32_t event)
{
  uint32_t mask;
//  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
//         ARM_USART_EVENT_TRANSFER_COMPLETE |
//         ARM_USART_EVENT_RX_TIMEOUT        |
//         ARM_USART_EVENT_RX_OVERFLOW       |
//         ARM_USART_EVENT_TX_UNDERFLOW      |
//         ARM_USART_EVENT_SEND_COMPLETE     |
//         ARM_USART_EVENT_TX_COMPLETE       ;
	
	  mask = ARM_USART_EVENT_RECEIVE_COMPLETE  |
         ARM_USART_EVENT_TRANSFER_COMPLETE |
         ARM_USART_EVENT_SEND_COMPLETE     |
         ARM_USART_EVENT_TX_COMPLETE       ;
	
	
	
  if (event & mask) {
    /* Success: Wakeup Thread */
    osThreadFlagsSet(GPS_TaskHandle, 0x00000001U);
  }
//  if (event & ARM_USART_EVENT_RECEIVE_COMPLETE) {
//    osThreadFlagsSet(GPS_TaskHandle, ARM_USART_EVENT_RECEIVE_COMPLETE);
//  }
//  if (event & ARM_USART_EVENT_TRANSFER_COMPLETE) {
//    osThreadFlagsSet(GPS_TaskHandle, ARM_USART_EVENT_TRANSFER_COMPLETE);
//  }
//  if (event & ARM_USART_EVENT_RX_TIMEOUT) {
//    osThreadFlagsSet(GPS_TaskHandle, ARM_USART_EVENT_RX_TIMEOUT);  /* Error: Call debugger or replace with custom error handling */
//  }
//  if (event & (ARM_USART_EVENT_RX_OVERFLOW | ARM_USART_EVENT_TX_UNDERFLOW)) {
//    osThreadFlagsSet(GPS_TaskHandle, ARM_USART_EVENT_RX_OVERFLOW | ARM_USART_EVENT_TX_UNDERFLOW);  /* Error: Call debugger or replace with custom error handling */
//  }
}





