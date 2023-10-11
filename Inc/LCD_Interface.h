/*
*@file       LCD_Interface.h
*@version    2.1.0
*@details    It contains all prototypes of used functions and states
*@author     Shehab aldeen mohammed abdalah
*/

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

#include "Port_Interface.h"
#include "DIO_Interface.h"
#include "Std_Types.h"
#include "FreeRTOS.h"
#include "task.h"

/* LCD functions */
void LCD_VidInit();
void LCD_VidSendCommend(uint8 Copy_U8Commend);
void LCD_VidSendChar(uint8 Copy_U8Char);
void LCD_VidSendString(uint8 Copy_U8String[]);
void LCD_VidPrintVar(uint32 Copy_U8Var);
uint32 LCD_U32Pow(uint8 Copy_U8Base, uint8 Copy_U8Bow);
void LCD_VidPosCur(uint8 Copy_U8Row, uint8 Copy_U8Col);
void LCD_VidClear();

#endif /* LCD_INTERFACE_H_ */
