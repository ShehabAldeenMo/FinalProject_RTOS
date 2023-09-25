/**
*@file       Port_Interface.h
*@version    1.0.0
*@brief      AUTOSAR Base - MCAL General purpose input output.
*@details    implementation of driver
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

/****************************************************************************
*****************************  Includes   ***********************************
*****************************************************************************/
#include "Port_Interface.h"
#include "Port_Private.h"
#include "Port_Cfg.h"


/* Port_ArrOfPorts store the default values of port that you declare it in cfg.c  */
extern Port_Cfg Port_ArrOfPorts[NUM_OF_PORT] ;


/* Port_ArryOfPins declare in prg.c to access by setter and getter functions only to edit on it storage within
   runtime to use in runnable functions as buffer and implement the runnable using supporting functions  */
static Port_PinModeType  Port_ArryOfPins      [AMOUNT_PINS] ;
static Port_PinModeType  Port_ArryOfActivition[AMOUNT_PINS] ;

/* to Reset register */
static uint8 FlagOfReset = 0 ;

/*
  Input  : Port_PinType , Port_PinDirectionType
  output : void
  description :To select the type of pin input or output
*/
void    Port_SetPinDirection (Port_PinType Pin,Port_PinDirectionType Direction)  {
	if ( Pin >= Port_A0  && Pin <= Port_A15 ){
		if (Pin <= Port_A7){
			GPIOA->CRL &= ~((0xF) << ( Pin * BASE_CRL_CRH));
			GPIOA->CRL |= ( Direction <<(Pin  * BASE_CRL_CRH) ) ;
		}
		else {
			GPIOA->CRH &= ~((0xF) << ( (Pin -8) * 4));
			GPIOA->CRH |= ( Direction <<( (Pin -8)*BASE_CRL_CRH) ) ;
		}
	}
	else if (Pin >= Port_B0  && Pin <= Port_B15){
		/*  to clear the bit that we used in define port */
		CLR_BIT(Pin,4);

		if (Pin  <= Port_B7 ){
			GPIOB->CRL &= ~((0xF) << ( Pin * BASE_CRL_CRH));
			GPIOB->CRL |= ( Direction << (Pin *BASE_CRL_CRH) ) ;
		}
		else {
			GPIOB->CRH &= ~((0xF) << ( (Pin -8) * 4));
			GPIOB->CRH |= ( Direction <<( (Pin -8)*BASE_CRL_CRH) ) ;
		}
	}
	else if (Pin >= Port_C13  && Pin <= Port_C15){
		/*  to clear the bit that we used in define port */
		CLR_BIT(Pin,5);

		GPIOC->CRH &= ~((0xF) << ( (Pin -8) * 4));
		GPIOC->CRH |= ( Direction <<( (Pin -8)*BASE_CRL_CRH) ) ;
	}
	else {
		Det_ReportError(MODULE_ID, Port_SetPinDirection_ID, PORT_E_PARAM_PIN);
	}
}
void    Port_GetVersionInfo  (Std_VersionInfoType* versioninfo){
	versioninfo->moduleID         = MODULE_ID         ;
	versioninfo->sw_major_version = PORT_MAJOR_VERSION ;
	versioninfo->sw_minor_version = PORT_MINOR_VERSION ;
	versioninfo->sw_patch_version = PORT_PATCH_VERSION ;
	versioninfo->vendorID         = VENDOR_ID         ;
}



void Port_VidInit (void){
	FlagOfReset++;
	for (uint8 i = 0 ; i < NUM_OF_PORT ; i++){

		switch (Port_ArrOfPorts[i].Port_Num){

		case Port_GPIO_A :

			/* to Reset register */
			GPIOA->CRL &= ~(0xFFFFFFFF);
			GPIOA->CRH &= ~(0xFFFFFFFF);

			/* to set mode of pins */
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOA->CRL |= ( Port_ArrOfPorts[Port_GPIO_A].Mode <<(i  * BASE_CRL_CRH) ) ;
			}
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOA->CRH |= ( Port_ArrOfPorts[Port_GPIO_A].Mode <<(i  * BASE_CRL_CRH) ) ;
			}

			break ;

		case Port_GPIO_B :

			/* to Reset register */
			GPIOB->CRL &= ~(0xFFFFFFFF);
			GPIOB->CRH &= ~(0xFFFFFFFF);

			/* to set mode of pins */
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOB->CRL |= ( Port_ArrOfPorts[Port_GPIO_B].Mode <<(i  * BASE_CRL_CRH) ) ;
			}
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOB->CRH |= ( Port_ArrOfPorts[Port_GPIO_B].Mode <<(i  * BASE_CRL_CRH) ) ;
			}

			break ;

		case Port_GPIO_C :

			/* to Reset register */
			GPIOC->CRL &= ~(0xFFFFFFFF);
			GPIOC->CRH &= ~(0xFFFFFFFF);

			/* to set mode of pins */
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOC->CRL |= ( Port_ArrOfPorts[Port_GPIO_C].Mode <<(i  * BASE_CRL_CRH) ) ;
			}
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOC->CRH |= ( Port_ArrOfPorts[Port_GPIO_C].Mode <<(i  * BASE_CRL_CRH) ) ;
			}

			break ;

		}
	}
}
Error_State       Port_EnumSetterPin(Port_PinType Copy_ChannelId ,Port_PinModeType Copy_Mode){
	if ( Copy_ChannelId >=Port_A0 && Copy_ChannelId <=Port_C15){
		Port_ArryOfPins[Copy_ChannelId]       = Copy_Mode      ;
		Port_ArryOfActivition[Copy_ChannelId] = STD_ON         ;
		return E_OK ;
	}
	return E_INVALID_PARAMETER ;
}

void              Port_VidRunnable (void) {
	uint8 i = 0 ;
	while(  i < AMOUNT_PINS ){
		if (Port_ArryOfActivition[i] != STD_OFF){
			Port_SetPinMode(i, Port_ArryOfPins[i]);
		}
		i++;
	}
}

/*
  Input  : Port_PinType , Port_PinModeType
  output : void
  description :To select the type of pin input or output
  =================================================
	In input mode (MODE[1:0]=00):
	00: Analog mode
	01: Floating input (reset state)
	10: Input with pull-up / pull-down
	11: Reserved

	In output mode (MODE[1:0] > 00):
	00: General purpose output push-pull
	01: General purpose output Open-drain
	10: Alternate function output Push-pull
	11: Alternate function output Open-drain

	MODEy[1:0]
	00: Input mode (reset state)
	01: Output mode, max speed 10 MHz.
	10: Output mode, max speed 2 MHz.
	11: Output mode, max speed 50 MHz
=============================================== */
void    Port_SetPinMode      (Port_PinType Pin,Port_PinModeType Mode){

	if (FlagOfReset == 0){
		GPIOA->CRL &= ~(0xFFFFFFFF);
		GPIOB->CRL &= ~(0xFFFFFFFF);
		GPIOC->CRL &= ~(0xFFFFFFFF);
		GPIOA->CRH &= ~(0xFFFFFFFF);
		GPIOB->CRH &= ~(0xFFFFFFFF);
		GPIOC->CRH &= ~(0xFFFFFFFF);
		FlagOfReset++;
	}

	if ( Pin >= Port_A0  && Pin <= Port_A15 ){
		if (Pin <= Port_A7){
			GPIOA->CRL |= ( Mode <<(Pin  * BASE_CRL_CRH) ) ;
		}
		else {
			GPIOA->CRH |= ( Mode <<( (Pin -8)*BASE_CRL_CRH) ) ;
		}
	}
	else if (Pin >= Port_B0  && Pin <= Port_B15){

		if (Pin  <= Port_B7 ){
			/*  to clear the bit that we used in define port */
			CLR_BIT(Pin,4);

			GPIOB->CRL |= ( Mode << (Pin *BASE_CRL_CRH) ) ;
		}
		else {
			/*  to clear the bit that we used in define port */
			CLR_BIT(Pin,4);

			GPIOB->CRH |= ( Mode <<( (Pin -8)*BASE_CRL_CRH) ) ;
		}
	}
	else if (Pin >= Port_C0  && Pin <= Port_C15){
		/*  to clear the bit that we used in define port */
		CLR_BIT(Pin,5);
		GPIOB->CRH |= ( Mode <<( (Pin -8)*BASE_CRL_CRH) ) ;
	}
	else {
		Det_ReportError(MODULE_ID, Port_SetPinMode_ID, PORT_E_PARAM_PIN);
	}
}
