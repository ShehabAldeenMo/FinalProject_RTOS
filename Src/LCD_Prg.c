/*
*@file       LCD_Prg.c
*@version    2.1.0
*@details    It has the core code of driver
*@author     Shehab aldeen mohammed abdalah
*            Ali Mamdouh
*/

#include "LCD_Interface.h"
#include "LCD_Cfg.h"
#include "LCD_Private.h"


extern uint8 ArrOfPins[8];


/**
 * \section Service_Name
 * LCD_VidSendCommend
 *
 * \section Description
 * To Send command to LCD.
 *
 * \section Service_ID
 * 0x00
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] uint8 Copy_U8Commend
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * To Send command to LCD.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start 
 * 
 * #green:Disable Rs;
 * #green:Out the value of commend on data port;
 * #green:make pulse for enable; 
 * end
 *
 * @enduml
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == Disable Rs ==
 *
 * "LCD_VidSendCommend\nFunction" -> "Dio_EnumSetterPin\nFunction": **RS, LOW**
 * note right #green: Disable RS in **__Dio buffer__**. 
 *
 * == Out the value of commend on data port ==
 *
 * "LCD_VidSendCommend\nFunction" -> "Dio_EnumSetterPin\nFunction": **ArrOfPins[i] , ArrOfPins_Level[i] **
 * note right #green: Out the value of the command code to the pins of LCD in **__Dio buffer__**. 
 *
 * == Make pulse for enable ==
 *
 * "LCD_VidSendCommend\nFunction" -> "Dio_EnumSetterPin\nFunction": **E, HIGH **
 * note right #green: Make enable High in **__Dio buffer__**. 
 *
 * "LCD_VidSendCommend\nFunction" -> "Dio_VidRunnable\nFunction":
 * note right #green: Apply on Dio pins values in **__Dio buffer__**. 
 *
 * "LCD_VidSendCommend\nFunction" -> "Dio_EnumSetterPin\nFunction": **E, Low **
 * note right #green: Make enable Low in **__Dio buffer__**. 
 *
 * "LCD_VidSendCommend\nFunction" -> "Dio_VidRunnable\nFunction":
 * note right #green: Apply on Dio pins values in **__Dio buffer__**. 
 *
 * @enduml
 *
 *
 *
 */
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



/**
 * \section Service_Name
 * LCD_VidInit
 *
 * \section Description
 * To Initialize LCD.
 *
 * \section Service_ID
 * 0x01
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] None
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * To Initialize LCD.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start 
 * 
 * :Set mode of LCD pins to be output;
 * #green:send function set commend;
 * :Delay for more than 39 us;
 * #green:Send Display_ON & Cursor_OFF Command;
 * :Delay for more than 39 us; 
 * #green:Send Entry Mode Set Command;
 * :Delay for more than 39 us; 
 * #green:Send Clear Display Command;
 * :Delay for more than 39 us; 
 * end
 *
 * @enduml
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == Set modes of LCD pins ==
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D0,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D1,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D2,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D3,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D4,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D5,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D6,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_EnumSetterPin\nFunction": **D7,PORT_PIN_OUT|G_PUSH_PULL*
 * note right #green: Set dedicated mode of pins of LCD in **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "Dio_VidRunnable\nFunction":
 * note right #green: Apply on Dio pins values in **__Dio buffer__**.  
 *
 * == Send commands ==
 *
 * "LCD_VidInit\nFunction" -> "LCD_VidSendCommend\nFunction": **FUNC_SET **
 * note right #green: send function set commend . 
 *
 * "LCD_VidInit\nFunction" -> "LCD_VidSendCommend\nFunction": **DISPLAY_ON_CURSOR_OFF **
 * note right #green: Send Display_ON & Cursor_OFF Command . 
 *
 * "LCD_VidInit\nFunction" -> "LCD_VidSendCommend\nFunction": **ENTERY_MODE_RIGHT**
 * note right #green: Send Entry Mode Set Command **__Dio buffer__**. 
 *
 * "LCD_VidInit\nFunction" -> "LCD_VidSendCommend\nFunction": **DISPLAY_CLEAR **
 * note right #green: Send Clear Display Command. 
 *
 * @enduml
 *
 *
 *
 */
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


/**
 * \section Service_Name
 * LCD_VidSendChar
 *
 * \section Description
 * To send char to LCD.
 *
 * \section Service_ID
 * 0x02
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] uint8 Copy_U8Char
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * To send char to LCD.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start 
 * 
 * #green:Define RS / RW **(to write on LCD)**;
 * #green:Make enable pulse;
 * end
 *
 * @enduml
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == RS / RW ==
 *
 * "LCD_VidSendChar\nFunction" -> "Dio_EnumSetterPin\nFunction": **RS, HIGH**
 * note right #green: to define RS / RW **__Dio buffer__**. 
 *
 * "LCD_VidSendChar\nFunction" -> "Dio_EnumSetterPin\nFunction": **ArrOfPins[i] , ArrOfPins_Level[i]**
 * note right #green: to define value of character on port in **__Dio buffer__**. 
 *
 * == Make enable pulse ==
 *
 * "LCD_VidSendChar\nFunction" -> "Dio_EnumSetterPin\nFunction": **E, HIGH**
 * note right #green: to make enable high in **__Dio buffer__**.
 *
 * "LCD_VidSendChar\nFunction" -> "Dio_VidRunnable\nFunction":
 * note right #green: Apply on Dio pins values in **__Dio buffer__**.  
 *
 * "LCD_VidSendChar\nFunction" -> "Dio_EnumSetterPin\nFunction": **E, Low **
 * note right #green: Make enable Low in **__Dio buffer__**. 
 *
 * "LCD_VidSendChar\nFunction" -> "Dio_VidRunnable\nFunction":
 * note right #green: Apply on Dio pins values in **__Dio buffer__**. 
 *
 * @enduml
 *
 *
 *
 */
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


/**
 * \section Service_Name
 * LCD_VidSendString
 *
 * \section Description
 * To send string to LCD.
 *
 * \section Service_ID
 * 0x03
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] uint8 *Copy_U8Char
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * To send string to LCD.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start 
 * while (**Copy_U8String[i] != '\0'?**) 
 * :if i =15 start new line in LCD;	 
 * :send Copy_U8String[i] **(send char)**;	
 * :delay for 8000 clk cycles;	 
 * :i++;	 
 * endwhile
 * end
 *
 * @enduml
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == LCD funcyions ==
 *
 * "LCD_VidSendString\nFunction" -> "LCD_VidSendCommend\nFunction": **LINE_2_START**
 * note right #green: start new line in LCD. 
 *
 * "LCD_VidSendString\nFunction" -> "LCD_VidSendChar\nFunction": **Copy_U8String[i]**
 * note right #green: start new Char to LCD. 
 *
 *
 * @enduml
 *
 *
 *
 */
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


/**
 * \section Service_Name
 * LCD_U32Pow
 *
 * \section Description
 * To power a number.
 *
 * \section Service_ID
 * 0x04
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] uint8 Copy_U8Base , uint8 Copy_U8Bow
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * To power a number.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start  
 * #green:loop until multiplying the number by its self **Copy_U8Bow** times;	 
 * #blue:return the number after multiplying the number by its self **Copy_U8Bow** times;	 
 * end
 *
 * @enduml
 *
 *
 *
 *
 */
uint32 LCD_U32Pow(uint8 Copy_U8Base , uint8 Copy_U8Bow ){
  uint32 local_res = 1 ;
  while (Copy_U8Bow >0 ){
	  local_res *= Copy_U8Base ;
	  Copy_U8Bow--;
  }
  return  local_res ;
}


/**
 * \section Service_Name
 * LCD_VidPrintVar
 *
 * \section Description
 * To print variable to LCD.
 *
 * \section Service_ID
 * 0x05
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] uint32 Copy_U8Var
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * To print variable to LCD.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start 
 * while (**Copy_U8String[i] != '\0'?**) 
 * :if Var = 0 print 0 on LCD;	 
 * :Convert Var to its correspondin characters;
 * #green:Send its correspondin characters to LCD; 
 * end
 *
 * @enduml
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == LCD functions ==
 *
 * "LCD_VidPrintVar\nFunction" -> "LCD_VidSendChar\nFunction": **'0'**
 * note right #green: send dedicated char to LCD. 
 *
 * "LCD_VidPrintVar\nFunction" -> "LCD_U32Pow\nFunction": **10,i**
 * note right #green: Power dedicated variable. 
 *
 * "LCD_VidPrintVar\nFunction" -> "LCD_U32Pow\nFunction": **10,copy_i**
 * note right #green: Power dedicated variable. 
 *
 * "LCD_VidPrintVar\nFunction" -> "LCD_VidSendChar\nFunction": **(local_num % 10) +'0' **
 * note right #green: send dedicated char to LCD. 
 *
 *
 *
 * @enduml
 *
 *
 *
 */
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


/**
 * \section Service_Name
 * LCD_VidPosCur
 *
 * \section Description
 * To Position cursor of LCD.
 *
 * \section Service_ID
 * 0x07
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] uint8 Copy_U8Row , uint8 Copy_U8Col
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * To print variable to LCD.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start 
 * :determine which row to put the cursor in;	
 * #blue:put cursor in dedicated position in that row;
 * end
 *
 * @enduml
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == LCD functions ==
 *
 * "LCD_VidPosCur\nFunction" -> "LCD_VidSendCommend\nFunction": **Local_Address**
 * note right #green: send dedicated position address to move the cursor to it. 
 *
 *
 *
 * @enduml
 *
 *
 *
 */
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


/**
 * \section Service_Name
 * LCD_VidClear
 *
 * \section Description
 * Clear LCD.
 *
 * \section Service_ID
 * 0x08
 *
 * \section Scope
 * Public
 *
 * \section Re-entrancy
 * Reentrant
 *
 *
 * \section Sync_Async
 * Synchronous
 *
 * 
 * \section Parameters
 * \param[in] uint8 Copy_U8Row , uint8 Copy_U8Col
 * \param[inout] None
 * \param[out] None
 * \return None
 * 
 *
 * \section Rational
 * Clear LCD.
 *
 *
 *
 *
 *
 *
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start 	
 * #blue:send command to clear lcd;
 * end
 *
 * @enduml
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == LCD functions ==
 *
 * "LCD_VidClear\nFunction" -> "LCD_VidSendCommend\nFunction": **DISPLAY_CLEAR**
 * note right #green: Clear screen of LCD. 
 *
 *
 *
 * @enduml
 *
 *
 *
 */
void LCD_VidClear (){
	LCD_VidSendCommend(DISPLAY_CLEAR) ;
}
