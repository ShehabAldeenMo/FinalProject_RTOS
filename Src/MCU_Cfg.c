/**
*@file       MCU_Cfg.c
*@version    1.0.0
*@brief      AUTOSAR Base - MCAL General purpose input output.
*@details    It contains all prototypes of used functions and states
*@author     Shehab aldeen mohammed abdalah
*/

/*===========================================================================
*   Project          : AUTOSAR  4.3.1 MCAL
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
*   AUTOSAR Version  : R20-11
*   AUTOSAR Revision : ASR_REL_4_1_REV_0001
*   SW Version       : 1.0.0
============================================================================*/
#include <Mcu_Interface.h>


Mcu_ConfigType Config_Clock = { HSE_CLOCK, PLL_NONE_0, PLL_NONE_ENTRY,
		ADC_PRE_DIVIDED_2, APB2__PRE_NOT_DIVIDED, APB1_PRE_NOT_DIVIDED,
		AHB_PRE_NOT_DIVIDED, HSE_SW, 0 };
