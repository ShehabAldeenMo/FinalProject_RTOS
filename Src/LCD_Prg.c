/*
*@file       LCD_Prg.c
*@version    2.1.0
*@details    It has the core code of driver
*@author     Shehab aldeen mohammed abdalah
*/

#include "LCD_Interface.h"
#include "LCD_Cfg.h"
#include "LCD_Private.h"


extern uint8 ArrOfPins[8];

void LCD_VidSendCommend(uint8 Copy_U8Commend){
/**************to disable RS *** *********************/
	Dio_EnumSetterPin(RS, LOW);
/******to out the value of commend on data port *****/
	uint8 ArrOfPins_Level[8];

	for (uint8 i=0;i<=7;i++){
		ArrOfPins_Level[i] = GET_BIT(Copy_U8Commend,i);
	}

	for (uint8 i=0;i<=7;i++){
		Dio_EnumSetterPin ( ArrOfPins[i] , ArrOfPins_Level[i] );
	}
/*************to make pulse for enable **************/
	Dio_EnumSetterPin(E, HIGH);
	Dio_VidRunnable();
	for (uint32 i = 0 ; i < 2000 ; i++);
	Dio_EnumSetterPin(E,LOW);
	Dio_VidRunnable();
	for (uint32 i = 0 ; i < 4000 ; i++);
}

void LCD_VidInit (void) {
	/***/
	Port_EnumSetterPin(D0,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(D1,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(D2,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(D3,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(D4,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(D5,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(D6,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(D7,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(RS,PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(E,PORT_PIN_OUT|G_PUSH_PULL);
	Port_VidRunnable();

	/********************************/
	for (uint16 i = 0 ; i < 320 ; i++);
	//send function set commend
	LCD_VidSendCommend (FUNC_SET);
	// Delay for more than 39 us
	for (uint16 i = 0 ; i < 320 ; i++);
	// Send Display_ON & Cursor_OFF Command
	LCD_VidSendCommend(DISPLAY_ON_CURSOR_OFF);
	// Delay for more than 39 us
	for (uint16 i = 0 ; i < 320 ; i++);
	// Send Entry Mode Set Command
	LCD_VidSendCommend(ENTERY_MODE_RIGHT);
	// Delay for more than 39 us
	for (uint16 i = 0 ; i < 320 ; i++);
	// Send Clear Display Command
	LCD_VidSendCommend(DISPLAY_CLEAR);
	// Delay for more than 1.53 ms
	for (uint32 i = 0 ; i < 16000 ; i++);
}

void LCD_VidSendChar(uint8 Copy_U8Char){
	uint8 ArrOfPins_Level[8];
	/******* to define RS / RW ***********/
	Dio_EnumSetterPin(RS,HIGH);
	/*to define value of character on port*/
	for (uint8 i=0;i<=7;i++){
		ArrOfPins_Level[i] = GET_BIT(Copy_U8Char,i);
	}

	for (uint8 i=0;i<=7;i++){
		Dio_EnumSetterPin ( ArrOfPins[i] , ArrOfPins_Level[i] );
	}
	/*********to make enable pulse*********/
	Dio_EnumSetterPin(E,HIGH);
	Dio_VidRunnable();
	for (uint32 i = 0 ; i < 2000 ; i++);
	Dio_EnumSetterPin(E,LOW);
	Dio_VidRunnable();
	for (uint32 i = 0 ; i < 4000 ; i++);
}

void LCD_VidSendString(uint8 Copy_U8String[] ){
	uint8 i=0;
	while( Copy_U8String[i] != '\0' ){
		if (15 == i){
			LCD_VidSendCommend(LINE_2_START);
		}
		LCD_VidSendChar(Copy_U8String[i]);
		for (uint32 i = 0 ; i < 8000 ; i++);
		i++;
	}//end while
}//end FUNC LCD_VidSendString

uint32 LCD_U32Pow(uint8 Copy_U8Base , uint8 Copy_U8Bow ){
  uint32 local_res = 1 ;
  while (Copy_U8Bow >0 ){
	  local_res *= Copy_U8Base ;
	  Copy_U8Bow--;
  }
  return  local_res ;
}

void LCD_VidPrintVar(uint32 Copy_U8Var){
	//to write 0 if it send directly
	if (0 == Copy_U8Var){
		LCD_VidSendChar('0');
		return ;
	}
	uint32 local_num = 0 ;
	uint8 i = 0 ;
	while (  Copy_U8Var/ (LCD_U32Pow(10,i) )  != 0 ){
		i++ ;
	}
	sint8 copy_i = i-1 ;
	while ( copy_i >= 0 ){
		local_num += (Copy_U8Var%10) * ( LCD_U32Pow(10,copy_i) ) ;
		Copy_U8Var/=10 ;
		copy_i-- ;
	}
	while( i > 0 ){
		LCD_VidSendChar( (local_num % 10) +'0' ) ;
		local_num /= 10 ;
		i--;
	}
}
void LCD_VidPosCur(uint8 Copy_U8Row , uint8 Copy_U8Col ) {
	uint8 Local_Address = 0 ;
	if (Copy_U8Row == 0)
		Local_Address =0x80 ;
	else if (Copy_U8Row == 1)
		Local_Address =0xC0 ;
	if (Copy_U8Col < 16 )
		Local_Address += Copy_U8Col ;
	LCD_VidSendCommend ( Local_Address );
}

void LCD_VidClear (){
	LCD_VidSendCommend(DISPLAY_CLEAR) ;
}
