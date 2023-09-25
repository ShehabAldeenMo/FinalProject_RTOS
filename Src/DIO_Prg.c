/**
*@file       MGPIO_Prg.c
*@version    1.0.0
*@brief      AUTOSAR Base - Platform dependend data type defination.
*@details    It has the core code of driver
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


#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "Dio_Cfg.h"


/*====================================================================================*/
State_Machine Global_State = Uninitialized ;


/* ArrOfPorts store the default values of port that you declare it in cfg.c  */
extern Dio_Cfg ArrOfPorts[NUM_OF_PORT] ;


/* ArryOfPins declare in prg.c to access by setter and getter functions only to edit on it storage within
   runtime to use in runnable functions as buffer and implement the runnable using supporting functions  */
static Dio_PortLevelType  ArryOfPins      [NUM_OF_PINS]    ;
static Dio_PortLevelType  ArryOfActivition[NUM_OF_PINS][2] ;


/* to initialize all pins with the configured types that used in Cfg.c */
void Dio_VidInit(void){
	Global_State = Initialized ;
	for (uint8 i = 0 ; i < NUM_OF_PORT ; i++){
		switch (ArrOfPorts[i].Port_Num){
		case Dio_GPIO_A :
			if (ArrOfPorts[i].Level == STD_ON){
				GPIOA->ODR = 0x0000FFFF ;
			}
			else if (ArrOfPorts[i].Level == STD_OFF){
				GPIOA->ODR = 0x00000000 ;
			}
			else {
				/* Invalid input */
			}
			break ;
		case Dio_GPIO_B :
			if (ArrOfPorts[i].Level == STD_ON){
				GPIOB->ODR = 0x0000FFFF ;
			}
			else if (ArrOfPorts[i].Level == STD_OFF){
				GPIOB->ODR = 0x00000000 ;
			}
			else {
				/* Invalid input */
			}
			break ;
		case Dio_GPIO_C :
			if (ArrOfPorts[i].Level == STD_ON){
				GPIOC->ODR = 0x0000FFFF ;
			}
			else if (ArrOfPorts[i].Level == STD_OFF){
				GPIOC->ODR = 0x00000000 ;
			}
			else {
				/* Invalid input */
			}
			break ;
		}
	}
	Global_State = Idle ;
}

/* if you want to set some thing in buffer call Dio_EnuSetterPin then Dio_VidRunnable will
    execute the configured data
  */
Error_State       Dio_EnumSetterPin(Dio_ChannelType Copy_ChannelId ,Dio_LevelType Copy_Level){
	if ( Copy_ChannelId >=Dio_A0 && Copy_ChannelId <=Dio_C15 && (Copy_Level == STD_HIGH || Copy_Level == STD_LOW) ){
		ArryOfPins[Copy_ChannelId]       = Copy_Level         ;
		ArryOfActivition[Copy_ChannelId][0] = STD_ON          ;
		ArryOfActivition[Copy_ChannelId][1] = DIO_OUT         ;
		return E_OK            ;
	}
	return E_INVALID_PARAMETER ;
}

/* if you want to get some thing from buffer call Dio_VidRunnable then Dio_EnuSetterPin will
    return the updated data
  */
Error_State Dio_EnumGetterPin(Dio_ChannelType Copy_ChannelId ,Dio_LevelType* Copy_Level){
	if (Copy_Level == NULL_PTR ){
		return E_NOT_OK ;
	}

	if ( Copy_ChannelId >=Dio_A0 && Copy_ChannelId <=Dio_C15 ){
		*Copy_Level    = ArryOfPins[Copy_ChannelId]          ;
		ArryOfActivition[Copy_ChannelId][0] = STD_ON         ;
		ArryOfActivition[Copy_ChannelId][1] = DIO_IN    ;
		return E_OK ;
	}
	return E_INVALID_PARAMETER ;
}

/* to execute the desired configuration within runtime "refresh pins" */
void   Dio_VidRunnable (void){
	switch (Global_State){
	case Uninitialized :
		/* no action */
		break ;
	case Initialized :
		/* no action */
		break ;
	case Idle  :
		Global_State = Busy ;

		uint8 i = 0 ;
		while(  i < NUM_OF_PINS ){
			if (ArryOfActivition[i][0] != STD_OFF && ArryOfActivition[i][1] == DIO_OUT ){
				Dio_WriteChannel(i,  ArryOfPins[i]);
			}
			i++;
		}

		i = 0 ;
		while(  i < NUM_OF_PINS ){
			if (ArryOfActivition[i][0] != STD_OFF && ArryOfActivition[i][1] == DIO_IN  ){
				ArryOfPins[i] = Dio_ReadChannel(i);
			}
			i++;
		}

		Global_State = Idle ;
		break ;
	case Busy  :
		/* No action */
		break ;
	}
}
/*====================================================================================*/


/*
  Input  : Dio_ChannelType
  output : Dio_LevelType
  description :
*/
Dio_LevelType     Dio_ReadChannel (Dio_ChannelType ChannelId){
	if ( ChannelId >= Dio_A0  && ChannelId <= Dio_A15 ){
		return ( (GPIOA->IDR >> (ChannelId ) )  & STD_HIGH ) ;
	}
	else if (ChannelId >= Dio_B0  && ChannelId <= Dio_B15){
		return ( (GPIOB->IDR >> (ChannelId ) )  & STD_HIGH ) ;
	}
	else if (ChannelId > Dio_B15  && ChannelId <= Dio_C15){
		return ( (GPIOC->IDR >> (ChannelId ) )  & STD_HIGH ) ;
	}
	else {
		Det_ReportError(MODULE_ID, Dio_ReadChannel_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	}
	return E_NOT_OK;
}



/*
  Input  : Dio_ChannelType
  output : Dio_LevelType
  description :
*/
void              Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level){
	if ( ChannelId >= Dio_A0  && ChannelId <= Dio_A15 ){

		if (Level == STD_HIGH){
			GPIOA->BSRR = ( 1 << ChannelId ) ;
		}
		else if (Level == STD_LOW){
			GPIOA->BRR  = ( 1 << ChannelId ) ;
		}
		else {
			Det_ReportError(MODULE_ID,  Dio_WriteChannel_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
		}
	}
	else if (ChannelId >= Dio_B0  && ChannelId <= Dio_B15){
		/*  to clear the bit that we used in define port */
		CLR_BIT(ChannelId,4);

		if (Level == STD_HIGH){
			GPIOB->BSRR = ( 1 << ChannelId ) ;
		}
		else if (Level == STD_LOW){
			GPIOB->BRR  = ( 1 << ChannelId ) ;
		}
		else {
			Det_ReportError(MODULE_ID,  Dio_WriteChannel_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
		}
	}
	else if (ChannelId > Dio_B15  && ChannelId <= Dio_C15){
		/*  to clear the bit that we used in define port */
		CLR_BIT(ChannelId,5);

		if (Level == STD_HIGH){
			GPIOC->BSRR = ( 1 << ChannelId ) ;
		}
		else if (Level == STD_LOW){
			GPIOC->BRR  = ( 1 << ChannelId ) ;
		}
		else {
			Det_ReportError(MODULE_ID,  Dio_WriteChannel_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
		}
	}
	else {
		Det_ReportError(MODULE_ID,  Dio_WriteChannel_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	}
}



/*
  Input  : Dio_PortType
  output : Dio_LevelType
  description :
*/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId){
	switch (PortId){
	case Dio_GPIO_A :
		return GPIOA->IDR;
	case Dio_GPIO_B :
		return GPIOB->IDR;
	case Dio_GPIO_C :
		return GPIOC->IDR;
	default :
		Det_ReportError(MODULE_ID,  Dio_ReadPort_ID, DIO_E_PARAM_INVALID_PORT_ID);
	}
	return STD_IDLE;
}


/*
  Input  : Dio_PortType , Dio_PortLevelType
  output : None
  description :
*/
void              Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level){
	switch (PortId){
	case Dio_GPIO_A :
		GPIOA->ODR = Level ;
		break ;
	case Dio_GPIO_B :
		GPIOA->ODR = Level ;
		break ;
	case Dio_GPIO_C :
		GPIOA->ODR = Level ;
		break ;
	default :
		Det_ReportError(MODULE_ID,   Dio_WritePort_ID, DIO_E_PARAM_INVALID_PORT_ID);
	}
}


/*
  Input  : Std_VersionInfoType
  output : None
  description :
*/
void              Dio_GetVersionInfo(Std_VersionInfoType* versioninfo){
	versioninfo->moduleID         = MODULE_ID         ;
	versioninfo->sw_major_version = PORT_MAJOR_VERSION ;
	versioninfo->sw_minor_version = PORT_MINOR_VERSION ;
	versioninfo->sw_patch_version = PORT_PATCH_VERSION ;
	versioninfo->vendorID         = VENDOR_ID         ;
}


/*
  Input  : Dio_ChannelType
  output : Dio_LevelType
  description :
*/
Dio_LevelType     Dio_FlipChannel(Dio_ChannelType ChannelId){
	if ( ChannelId >= Dio_A0  && ChannelId <= Dio_A15 ){
		TOGGLE_BIT(GPIOA->ODR,ChannelId);
	}
	else if (ChannelId >= Dio_B0  && ChannelId <= Dio_B15){
		/*  to clear the bit that we used in define port */
		CLR_BIT(ChannelId,5);

		TOGGLE_BIT(GPIOB->ODR,ChannelId);
	}
	else if (ChannelId >  Dio_B15  && ChannelId <= Dio_C15){
		/*  to clear the bit that we used in define port */
		CLR_BIT(ChannelId,5);

		TOGGLE_BIT(GPIOC->ODR,ChannelId);
	}
	else {
		Det_ReportError(MODULE_ID,   Dio_FlipChannel_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	}
	return STD_IDLE;
}






