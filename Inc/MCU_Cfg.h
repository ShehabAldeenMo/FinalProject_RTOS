/**
*@file       MCU_Cfg.h
*@version    1.0.0
*@brief      AUTOSAR Base - Platform dependend data type defination.
*@details    It has the specific configurations of Mcu driver.
*@author     Shehab aldeen mohammed abdalah.
*/

/*===========================================================================
*   Project          : AUTOSAR  4.3.1 MCAL
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
*   AUTOSAR Version  : R20-11
*   AUTOSAR Revision : ASR_REL_4_1_REV_0001
*   SW Version       : 1.0.0
============================================================================*/
#ifndef MCU_CFG_H_
#define MCU_CFG_H_

#include "Std_Types.h"

#define McuInitClock             FALSE
#define McuNoPll                 FALSE

/* if you used HSE define it otherwise neglect it*/
#define HSE             HSE_CRSTYAL
#define CSS_CFG         CSS_OFF

#endif /* MCU_CFG_H_ */
