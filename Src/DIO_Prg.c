/*
*@file       Dio_Prg.c
*@version    1.0.0
*@details    It has the core code of driver
*@author     Shehab aldeen mohammed abdalah
*/

/*===========================================================================
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
*   SW Version       : 1.0.0
============================================================================*/

/****************************************************************************
 *****************************  Includes   ***********************************
 *****************************************************************************/
#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "Dio_Cfg.h"

/* ArrOfPorts store the default values of port that you declare it in cfg.c  */
extern Dio_Cfg ArrOfPorts[NUM_OF_PORT];

/* ArryOfPins declare in prg.c to access by setter and getter functions only to edit on it storage within
 runtime to use in runnable functions as buffer and implement the runnable using supporting functions  */
static Dio_PortLevelType ArryOfPins[NUM_OF_PINS];
static Dio_PortLevelType ArryOfActivition[NUM_OF_PINS][2];

/* Create semphores to protect shared resources */
#if DIO_DESIGN == DIO_FREERTOS
SemaphoreHandle_t DIO_SemArrOfPorts       = NULL ;
SemaphoreHandle_t DIO_SemArryOfPins       = NULL ;
SemaphoreHandle_t DIO_SemArryOfActivition = NULL ;
#endif

/* to initialize all pins with the configured types that used in Cfg.c */
void Dio_VidInit(void) {
	/* To protect shared resources by semphore
	 * It's created taken
	 **/
#if DIO_DESIGN == DIO_FREERTOS
	DIO_SemArrOfPorts = xSemaphoreCreateBinary();
	DIO_SemArryOfPins = xSemaphoreCreateBinary();
	xSemaphoreGive(DIO_SemArryOfPins);
	DIO_SemArryOfActivition  = xSemaphoreCreateBinary();
	xSemaphoreGive(DIO_SemArryOfActivition);
#endif
	for (uint8 i = 0; i < NUM_OF_PORT; i++) {
		switch (ArrOfPorts[i].Port_Num) {
		case Dio_GPIO_A:
			if (ArrOfPorts[i].Level == STD_ON) {
				GPIOA->ODR.Reg = 0x0000FFFF;
			} else if (ArrOfPorts[i].Level == STD_OFF) {
				GPIOA->ODR.Reg = 0x00000000;
			} else {
				/* Invalid input */
			}
			break;
		case Dio_GPIO_B:
			if (ArrOfPorts[i].Level == STD_ON) {
				GPIOB->ODR.Reg = 0x0000FFFF;
			} else if (ArrOfPorts[i].Level == STD_OFF) {
				GPIOB->ODR.Reg = 0x00000000;
			} else {
				/* Invalid input */
			}
			break;
		case Dio_GPIO_C:
			if (ArrOfPorts[i].Level == STD_ON) {
				GPIOC->ODR.Reg = 0x0000FFFF;
			} else if (ArrOfPorts[i].Level == STD_OFF) {
				GPIOC->ODR.Reg = 0x00000000;
			} else {
				/* Invalid input */
			}
			break;
		}
	}
#if DIO_DESIGN == DIO_FREERTOS
	xSemaphoreGive(DIO_SemArrOfPorts);
#endif
}

/* if you want to set some thing in buffer call Dio_EnuSetterPin then Dio_VidRunnable will
 execute the configured data
 */
Error_State Dio_EnumSetterPin(Dio_ChannelType Copy_ChannelId,
		Dio_LevelType Copy_Level) {
#if DIO_DESIGN == DIO_FREERTOS
	xSemaphoreTake(DIO_SemArryOfPins,portMAX_DELAY);
	xSemaphoreTake(DIO_SemArryOfActivition,portMAX_DELAY);
#endif
	if (Copy_ChannelId >= Dio_A0 && Copy_ChannelId <= Dio_C14) {
		ArryOfPins[Copy_ChannelId] = Copy_Level;
		ArryOfActivition[Copy_ChannelId][0] = STD_ON;
		ArryOfActivition[Copy_ChannelId][1] = DIO_OUT;
		return E_OK;
	}
#if DIO_DESIGN == DIO_FREERTOS
	xSemaphoreGive(DIO_SemArryOfPins);
	xSemaphoreGive(DIO_SemArryOfActivition);
#endif
	return E_INVALID_PARAMETER;
}

/* if you want to get some thing from buffer call Dio_VidRunnable then Dio_EnuSetterPin will
 return the updated data
 */
Error_State Dio_EnumGetterPin(Dio_ChannelType Copy_ChannelId,
		Dio_LevelType *Copy_Level) {
#if DIO_DESIGN == DIO_FREERTOS
	xSemaphoreTake(DIO_SemArryOfPins,portMAX_DELAY);
	xSemaphoreTake(DIO_SemArryOfActivition,portMAX_DELAY);
#endif
	if (Copy_Level == NULL_PTR) {
		return E_NOT_OK;
	}

	if (Copy_ChannelId >= Dio_A0 && Copy_ChannelId <= Dio_C14) {
		*Copy_Level = ArryOfPins[Copy_ChannelId];
		ArryOfActivition[Copy_ChannelId][0] = STD_ON;
		ArryOfActivition[Copy_ChannelId][1] = DIO_IN;
		return E_OK;
	}
#if DIO_DESIGN == DIO_FREERTOS
	xSemaphoreGive(DIO_SemArryOfPins);
	xSemaphoreGive(DIO_SemArryOfActivition);
#endif
	return E_INVALID_PARAMETER;
}

/* to execute the desired configuration within runtime "refresh pins" */
void Dio_VidRunnable(void) {
	/* GPIOA */
	uint8 i = Dio_A0;
#if DIO_DESIGN == DIO_FREERTOS
	xSemaphoreTake(DIO_SemArryOfPins,portMAX_DELAY);
	xSemaphoreTake(DIO_SemArryOfActivition,portMAX_DELAY);
#endif
	while (i <= Dio_A12) {
		if (ArryOfActivition[i][0] != STD_OFF) {
			if (ArryOfActivition[i][1] == DIO_OUT) {
				Dio_WriteChannel(GPIOA, i, ArryOfPins[i]);
			} else {
				ArryOfPins[i] = Dio_ReadChannel(GPIOA,i);
			}
		}
		i++;
	}

	if (ArryOfActivition[Dio_A15][0] != STD_OFF) {
		if (ArryOfActivition[Dio_A15][1] == DIO_OUT) {
			Dio_WriteChannel(GPIOA, Dio_A15, ArryOfPins[Dio_A15]);
		} else {
			ArryOfPins[Dio_A15] = Dio_ReadChannel(GPIOA, Dio_A15);
		}
	}

	/* GPIOB */
	i = Dio_B0;
	while (i <= Dio_B15) {
		if (ArryOfActivition[i][0] != STD_OFF) {
			if (ArryOfActivition[i][1] == DIO_OUT) {
				Dio_WriteChannel(GPIOB, (i & ~(0x10)) , ArryOfPins[i]);
			} else {
				ArryOfPins[i] = Dio_ReadChannel(GPIOB, (i & ~(0x10)));
			}
		}
		i++;
	}

	i = Dio_C13;
	while (i <= Dio_C14) {
		if (ArryOfActivition[i][0] != STD_OFF) {
			if (ArryOfActivition[i][1] == DIO_OUT) {
				Dio_WriteChannel(GPIOC, (i & ~(0x20)), ArryOfPins[i]);
			} else {
				ArryOfPins[i] = Dio_ReadChannel(GPIOC, (i & ~(0x20)));
			}
		}
		i++;
	}
#if DIO_DESIGN == DIO_FREERTOS
	xSemaphoreGive(DIO_SemArryOfPins);
	xSemaphoreGive(DIO_SemArryOfActivition);
#endif
}

/*
 Input  : Dio_ChannelType
 output : Dio_LevelType
 description :
 */
Dio_LevelType Dio_ReadChannel(volatile GPIOX_REG *GPIOX, Dio_ChannelType ChannelId) {
	uint32 Local_IDR = GPIOX->IDR.Reg;
	Local_IDR = ((Local_IDR >> (ChannelId)) & STD_HIGH);
	return Local_IDR;
}

/*
 Input  : Dio_ChannelType
 output : Dio_LevelType
 description :
 */
void Dio_WriteChannel(volatile GPIOX_REG *GPIOX, Dio_ChannelType ChannelId,
		Dio_LevelType Level) {
	uint32 Local_Pin = (1 << ChannelId);

	if (Level == STD_HIGH) {
		GPIOX->BSRR.Reg = Local_Pin;
	} else if (Level == STD_LOW) {
		GPIOX->BRR.Reg = Local_Pin;
	}
	else {
		Det_ReportError(MODULE_ID, Dio_WriteChannel_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	}
}

/*
 Input  : Std_VersionInfoType
 output : None
 description :
 */
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo) {
	versioninfo->moduleID = MODULE_ID;
	versioninfo->sw_major_version = DIO_MAJOR_VERSION;
	versioninfo->sw_minor_version = DIO_MINOR_VERSION;
	versioninfo->sw_patch_version = DIO_PATCH_VERSION;
	versioninfo->vendorID = VENDOR_ID;
}
