/*
 * LCD_Cfg.h
 *
 *  Created on: Aug 25, 2023
 *      Author: HP
 */

#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#include "LCD_Interface.h"

/* define EIGHT_BIT or FOUR_BIT mode */
#define EIGHT_BIT

#define D0        Dio_A0
#define D1        Dio_A1
#define D2        Dio_A2
#define D3        Dio_A3
#define D4        Dio_A4
#define D5        Dio_A5
#define D6        Dio_A6
#define D7        Dio_A7

#define RS        Dio_B0
#define E         Dio_B1


#define DATA_PORT     Dio_GPIO_A
#define CONTROL_PORT  Dio_GPIO_B

#endif /* LCD_CFG_H_ */
