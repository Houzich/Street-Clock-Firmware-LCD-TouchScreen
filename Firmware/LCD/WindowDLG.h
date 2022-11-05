/**
  ******************************************************************************
  * File Name          : WindowDLG.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOWDLG
#define __WINDOWDLG
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "DIALOG.h"
/* Exported constants --------------------------------------------------------*/
extern WM_HWIN hItem_TEXT_RTC;
extern WM_HWIN hItem_TEXT_Clock;
extern WM_HWIN hItem_TEXT_Note;
extern WM_HWIN hItem_TEXT_State;
extern WM_HWIN hItem_TEXT_GPS_Signal;
extern WM_HWIN hItem_TEXT_UTC;
extern WM_HWIN hItem_Text_Block;
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
WM_HWIN CreateWindow(void);
#endif /*__WINDOWDLG*/
