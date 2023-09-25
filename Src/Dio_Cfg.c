/*
 * Dio_Cfg.c
 *
 *  Created on: Aug 5, 2023
 *      Author: HP
 */

#include "DIO_Interface.h"
#include "Dio_Cfg.h"

Dio_Cfg ArrOfPorts[NUM_OF_PORT] = {
		{
				.Port_Num       = Dio_GPIO_A      ,
                .Level          = STD_OFF         ,
		},
		{
				.Port_Num       = Dio_GPIO_B      ,
                .Level          = STD_OFF         ,
		},
		{
				.Port_Num       = Dio_GPIO_C      ,
                .Level          = STD_OFF         ,
		}
};
