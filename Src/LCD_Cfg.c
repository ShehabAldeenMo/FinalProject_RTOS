/*
 * LCD_Cfg.c
 *
 *  Created on: Sep 1, 2023
 *      Author: HP
 */

#include "Std_Types.h"
#include "LCD_Interface.h"
#include "LCD_Cfg.h"

#ifdef EIGHT_BIT

uint8 ArrOfPins[8] = {D0,D1,D2,D3,D4,D5,D6,D7} ;

#endif

#ifndef EIGHT_BIT

uint8 ArrOfPins[4] = {D4,D5,D6,D7} ;

#endif
