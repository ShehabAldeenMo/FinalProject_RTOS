/**
*@file       Port_Cfg.c
*@version    1.0.0
*@brief      AUTOSAR Base - MCAL General purpose input output.
*@details    It contains all prototypes of used functions and states in port driver
*@author     Shehab aldeen mohammed abdalah
*/

/*===========================================================================
*   Project          : AUTOSAR  4.3.1 MCAL
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
*   AUTOSAR Version  : 4.3.1
*   AUTOSAR Revision : ASR_REL_4_1_REV_0001
*   SW Version       : 1.0.0
============================================================================*/

#include "Port_Interface.h"
#include "Port_Cfg.h"

Port_Cfg Port_ArrOfPorts[NUM_OF_PORT]= {
		{
				.Port_Num       = Port_GPIO_A                   ,
                .Mode           = OUTPUT_2|G_PUSH_PULL          ,
		},
		{
				.Port_Num       = Port_GPIO_B                    ,
                .Mode           = OUTPUT_2|G_PUSH_PULL           ,
		},
		{
				.Port_Num       = Port_GPIO_C                    ,
                .Mode           = OUTPUT_2|G_PUSH_PULL           ,
		}
};
