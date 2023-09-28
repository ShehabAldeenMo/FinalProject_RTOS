/**
*@file       Port_Interface.h
*@version    2.1.0
*@details    implementation of driver
*@author     Shehab aldeen mohammed abdalah
*/

/*===========================================================================
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
*   SW Version       : 2.1.0
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

/* Create semphores to protect shared resources */
#if PORT_DESIGN == PORT_FREERTOS
SemaphoreHandle_t PORT_SemArrOfPorts       = NULL ;
SemaphoreHandle_t PORT_SemArryOfPins       = NULL ;
SemaphoreHandle_t PORT_SemArryOfActivition = NULL ;
#endif

void    Port_GetVersionInfo  (Std_VersionInfoType* versioninfo){
	versioninfo->moduleID         = MODULE_ID         ;
	versioninfo->sw_major_version = PORT_MAJOR_VERSION ;
	versioninfo->sw_minor_version = PORT_MINOR_VERSION ;
	versioninfo->sw_patch_version = PORT_PATCH_VERSION ;
	versioninfo->vendorID         = VENDOR_ID         ;
}



void Port_VidInit (void){
	/* To protect shared resources by semphore
	 * It's created taken
	 **/
#if PORT_DESIGN == PORT_FREERTOS
	PORT_SemArrOfPorts = xSemaphoreCreateBinary();
	PORT_SemArryOfPins = xSemaphoreCreateBinary();
	xSemaphoreGive(PORT_SemArryOfPins);
	PORT_SemArryOfActivition  = xSemaphoreCreateBinary();
	xSemaphoreGive(PORT_SemArryOfActivition);
#endif
	for (uint8 i = 0 ; i < NUM_OF_PORT ; i++){
		switch (Port_ArrOfPorts[i].Port_Num){
		case Port_GPIO_A :
			/* to Reset register */
			GPIOA->CRL.Reg = 0x00000000 ;
			GPIOA->CRH.Reg = 0x00000000 ;

			/* to set mode of pins */
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOA->CRL.Reg |= ( Port_ArrOfPorts[Port_GPIO_A].Mode <<(i  * BASE_CRL_CRH) ) ;
			}
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOA->CRH.Reg |= ( Port_ArrOfPorts[Port_GPIO_A].Mode <<(i  * BASE_CRL_CRH) ) ;
			}
			break ;
		case Port_GPIO_B :
			/* to Reset register */
			GPIOB->CRL.Reg = (0x00000000);
			GPIOB->CRH.Reg = (0x00000000);
			/* to set mode of pins */
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOB->CRL.Reg |= ( Port_ArrOfPorts[Port_GPIO_B].Mode <<(i  * BASE_CRL_CRH) ) ;
			}
			for (uint8 i = 0 ; i < 8 ; i++ ){
				GPIOB->CRH.Reg |= ( Port_ArrOfPorts[Port_GPIO_B].Mode <<(i  * BASE_CRL_CRH) ) ;
			}
			break ;
		case Port_GPIO_C :
			/* to Reset register */
			GPIOC->CRL.Reg = (0x00000000);
			GPIOC->CRH.Reg = (0x00000000);

			/* to set mode of pins of port C */
			GPIOC->CRH.Reg |= ( Port_ArrOfPorts[Port_GPIO_C].Mode <<(Port_C13  * BASE_CRL_CRH) ) ;
			GPIOC->CRH.Reg |= ( Port_ArrOfPorts[Port_GPIO_C].Mode <<(Port_C14  * BASE_CRL_CRH) ) ;
		}
	}
#if PORT_DESIGN == PORT_FREERTOS
	xSemaphoreGive(PORT_SemArrOfPorts);
#endif
}


Error_State       Port_EnumSetterPin(Port_PinType Copy_ChannelId ,Port_PinModeType Copy_Mode){
	if ( Copy_ChannelId >=Port_A0 && Copy_ChannelId <=Port_C14){
#if PORT_DESIGN == PORT_FREERTOS
	xSemaphoreTake(PORT_SemArryOfPins,portMAX_DELAY);
	xSemaphoreTake(PORT_SemArryOfActivition,portMAX_DELAY);
#endif
		Port_ArryOfPins[Copy_ChannelId]       = Copy_Mode      ;
		Port_ArryOfActivition[Copy_ChannelId] = STD_ON         ;
		return E_OK ;
	}
#if PORT_DESIGN == PORT_FREERTOS
	xSemaphoreGive(PORT_SemArryOfPins);
	xSemaphoreGive(PORT_SemArryOfActivition);
#endif
	return E_INVALID_PARAMETER ;
}

void  Port_VidRunnable (void) {
	uint8 i = 0 ;
#if PORT_DESIGN == PORT_FREERTOS
	xSemaphoreTake(PORT_SemArryOfPins,portMAX_DELAY);
	xSemaphoreTake(PORT_SemArryOfActivition,portMAX_DELAY);
#endif
	while(  i <= Port_A15 ){
		if (Port_ArryOfActivition[i] != STD_OFF){
			Port_SetPinMode(GPIOA,i, Port_ArryOfPins[i]);
		}
		i++;
	}

	while(  i <= Port_B15 ){
		if (Port_ArryOfActivition[i] != STD_OFF){
			Port_SetPinMode(GPIOB,(i & ~(0x10)), Port_ArryOfPins[i]);
		}
		i++;
	}

	if (Port_ArryOfActivition[Port_C13] != STD_OFF){
		Port_SetPinMode(GPIOC,(Port_C13 & ~(0x10)), Port_ArryOfPins[Port_C13]);
	}

	if (Port_ArryOfActivition[Port_C14] != STD_OFF){
		Port_SetPinMode(GPIOC,(Port_C14 & ~(0x10)), Port_ArryOfPins[Port_C13]);
	}
#if PORT_DESIGN == PORT_FREERTOS
	xSemaphoreGive(PORT_SemArryOfPins);
	xSemaphoreGive(PORT_SemArryOfActivition);
#endif
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
void    Port_SetPinMode      (GPIOX_REG *GPIOX,Port_PinType Pin,Port_PinModeType Mode){
	if (Pin <= LOW_PIN) {
		GPIOX->CRL.Reg |= (Mode << (Pin * BASE_CRL_CRH));
	} else {
		GPIOX->CRH.Reg |= (Mode << ((Pin - 8) * BASE_CRL_CRH));
	}
}
