/**
  ******************************************************************************
  * File Name          : Flash.c
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "FLASH.h"
#include "RTC.h"
#include "Street_Clock.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#ifdef DEBUG_FLASH
#undef DEBUG
#undef DEBUG_PRINTF
#undef DEBUG_PUTCHAR
#define DEBUG(...)		do {debug_header("FLASH: ", __VA_ARGS__);} while (0)
#define DEBUG_PRINT(...)		    do {debug_print(__VA_ARGS__);} while (0)
#define DEBUG_PUTCHAR(x)	ITM_SendChar x ;
#else
#define DEBUG(...)
#define DEBUG_PRINTF(...)
#define DEBUG_PUTCHAR(...)
#endif // DEBUG_FLASH
/* Private variables ---------------------------------------------------------*/
extern STREETCLOCK_HandleStoreSettingsTypeDef sett_store;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/*###############################################################*/
/*###############################################################* FLASH_Save_Settings -->*/
/*###############################################################*/
/**
  * @brief  Save Settings structure in flash.
  * @param  None
  * @retval write status:
  *           - false: error write
  *           - true: if Settings structure is write 
  */
HAL_StatusTypeDef FLASH_Save_Settings(void)
{
  HAL_StatusTypeDef FlashStatus = HAL_OK;
  uint32_t SectorError = 0;
  
  FLASH_EraseInitTypeDef pEraseInit;
  pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;  
  pEraseInit.Sector = DATA_FLASH_SECTOR;
  pEraseInit.NbSectors = 1;
  pEraseInit.VoltageRange = VOLTAGE_RANGE;
  
  FlashStatus = HAL_FLASH_Unlock();
  if (FlashStatus != HAL_OK)
  {
    HAL_FLASH_Lock();
    DEBUG("ERROR: Flash Unlock!!!\n");
    return FlashStatus;
  }
//  FlashStatus = HAL_FLASH_OB_Unlock();
//  if (FlashStatus != HAL_OK)
//  {
//    HAL_FLASH_Lock();
//    DEBUG("ERROR: Flash OB Unlock!!!\n");
//    return FlashStatus;
//  }  
  
  FlashStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError); 
  /* If erase operation was failed, a Flash error code is returned */
  if (FlashStatus != HAL_OK)
  {
    HAL_FLASH_Lock();
    DEBUG("ERROR: Erase Sector!!!\n");
    return FlashStatus;
  }
  
  uint32_t *pFlash = (uint32_t *)DATA_FLASH_START_ADDRESS;
  uint32_t *pData = (uint32_t *)&sett_store;
	for (int i = 0; i < sizeof(sett_store)/4; i++)
  {
    /* Set variable data */
    FlashStatus = HAL_FLASH_Program(TYPEPROGRAM_WORD,(uint32_t)pFlash++, *(pData++));       
    /* If program operation was failed, a Flash error code is returned */
    if (FlashStatus != HAL_OK)
    {
      HAL_FLASH_Lock();
      DEBUG("ERROR: Program word num %d!!!\n", i);
      return FlashStatus;
    }
  }
  
  HAL_FLASH_Lock();
	return FlashStatus; 
}
/*###############################################################*/
/*###############################################################* FLASH_Get_Settings -->*/
/*###############################################################*/
/**
  * @brief  Get Settings structure from flash.
  * @param  None
	* @retval read status:
  *           - false: error read
  *           - true: if Settings structure is read and fill 
  */
bool FLASH_Get_Settings(void)
{
  uint32_t *pFlash = (uint32_t *)DATA_FLASH_START_ADDRESS;
  uint32_t *pData = (uint32_t *)&sett_store;
	for (int i = 0; i < sizeof(sett_store)/4; i++)
  {
    *(pData++) = *(pFlash++);
  }
	return true;
}
