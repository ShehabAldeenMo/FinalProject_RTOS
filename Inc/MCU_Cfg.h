/*
*@file       MCU_Cfg.h
*@version    2.0.0
*@details    It has the specific configurations of Mcu driver.
*@author     Shehab aldeen mohammed abdalah.
*/

/*===========================================================================
*   Layer            : MCAL
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
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
