/**
  ******************************************************************************
  * File Name          : FLASH.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#ifdef STM32F7
#include "stm32f7xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#endif 

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef FLASH_Save_Settings(void);
bool FLASH_Get_Settings(void);
bool FLASH_Save_Data(uint16_t virt_address, uint8_t *data, uint32_t size);
bool FLASH_Get_Data(uint16_t virt_address, uint8_t *data, uint32_t size);
bool FLASH_Save_Settings_Data(uint8_t *data, uint32_t size);
bool FLASH_Get_Settings_Data(uint8_t *data, uint32_t size);

/* Device voltage range supposed to be [2.7V to 3.6V], the operation will 
   be done by word  */
#define VOLTAGE_RANGE           (uint8_t)VOLTAGE_RANGE_3

#ifdef STM32F7
#define DATA_FLASH_START_ADDRESS  ((uint32_t)0x081C0000)
#define DATA_FLASH_SECTOR  FLASH_SECTOR_11
#elif defined(STM32F4)
#define DATA_FLASH_START_ADDRESS  ((uint32_t)0x08060000)
#define DATA_FLASH_SECTOR  FLASH_SECTOR_7
#define BLOCK_DATA_FLASH_START_ADDRESS
#endif 


#ifdef __cplusplus
}
#endif
#endif /*__FLASH_H*/



