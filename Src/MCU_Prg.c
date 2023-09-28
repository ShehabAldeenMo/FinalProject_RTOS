/*
*@file       MCU_Prg.c
*@version    2.1.0
*@details    It has the core code of driver
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
#include <Mcu_Interface.h>
#include <Mcu_Private.h>
#include "Mcu_Cfg.h"

/* To used in Mcu_Init */
extern Mcu_ConfigType Config_Clock;
const Mcu_ConfigType* ConfigPtr = & Config_Clock ;

/* TO access on APB1 APB2 AHB registers */
static uint32 PerpherialsAPB1;
static uint32 PerpherialsAPB2;
static uint32 PerpherialsAHB;

/* Create semphores to protect shared resources */
#if MCU_DESIGN == MCU_FREERTOS
SemaphoreHandle_t MCU_SemConfig_Clock    = NULL ;
SemaphoreHandle_t MCU_SemPerpherialsAPB1 = NULL ;
SemaphoreHandle_t MCU_SemPerpherialsAPB2 = NULL ;
SemaphoreHandle_t MCU_SemPerpherialsAHB  = NULL ;
#endif


void   Mcu_Init () {
	/* check on coming pointer */
	if (NULL_PTR == ConfigPtr){
		Det_ReportError(MODULE_ID, Mcu_Init_ID, MCU_E_PARAM_POINTER);
	}

/* To protect shared resources by semphore
 * It's created taken
 **/
#if MCU_DESIGN == MCU_FREERTOS
	MCU_SemConfig_Clock    = xSemaphoreCreateBinary();
	MCU_SemPerpherialsAPB1 = xSemaphoreCreateBinary();
	xSemaphoreGive(MCU_SemPerpherialsAPB1);
	MCU_SemPerpherialsAPB2 = xSemaphoreCreateBinary();
	xSemaphoreGive(MCU_SemPerpherialsAPB2);
	MCU_SemPerpherialsAHB  = xSemaphoreCreateBinary();
	xSemaphoreGive(MCU_SemPerpherialsAHB);
#endif

	/* to select HSE[16] */
	uint32 Local_RCC_Value = ConfigPtr->CLOCK_SYS | ( ConfigPtr->HSITRIM << 3 ) ;
	RCC->CR.Reg      = Local_RCC_Value ;

	/* to check that the clock is stabled  :
	 PLLRDY    or   HSERDY   or   HSIRDY
	 */
	uint32 LOCAL_Clock_Flag = ( ConfigPtr->CLOCK_SYS << 1 ) ;  /*  to check on one in the bit of flag  */
	uint16 LOCAL_Time_limit = TIMEOUT_ROOF ;

	while ( (LOCAL_Clock_Flag & RCC->CR.Reg) && (0 == LOCAL_Time_limit))
	{
		LOCAL_Time_limit--;
	}

	if (0 == LOCAL_Time_limit){
		Det_ReportError(MODULE_ID, Mcu_Init_ID, MCU_E_TIMEOUT);
	}

	/* to turn off VCC in PIN out that used in HSE to get clock */
#if   HSE == HSE_CRSTYAL
	RCC->CR.B.HSEBYP = 0 ;
#elif HSE == HSE_RC
	RCC->CR.B.HSEBYP = 1 ;
#endif

#if CSS_CFG == CSS_OFF
	RCC->CR.B.CSSON = 0 ;
#elif CSS_CFG == CSS_ON
	RCC->CR.B.CSSON = 1 ;
#endif

	/* to select SW[1:0]
	 00: HSI selected as system clock
	 01: HSE selected as system clock
     10: PLL selected as system clock
	  */
	uint32 Local_RCC_CFGR = ConfigPtr->PLL_MUL_NUMBER
			| ConfigPtr->PLL_ENTRY_CLOCK | ConfigPtr->ADC_PRESCALAR_
			| ConfigPtr->APB2_PRESCALAR_ | ConfigPtr->APB1_PRESCALAR_
			| ConfigPtr->AHB_PRESCALAR_ | ConfigPtr->SW_ENABLE_CLOCK;
	RCC->CFGR.Reg    = Local_RCC_CFGR ;

#if MCU_DESIGN == MCU_FREERTOS
	xSemaphoreGive(MCU_SemConfig_Clock);
#endif
}

Error_State       Mcu_EnumSetPerAPB1(BUS_PERPHERIALS Mcu_Per ) {
#if MCU_DESIGN == MCU_FREERTOS
	xSemaphoreTake(MCU_SemPerpherialsAPB1,portMAX_DELAY);
#endif
	if ( Mcu_Per >= APB1_PER_TIM2  && Mcu_Per <= APB1_PER_DAC ){
		PerpherialsAPB1 |= Mcu_Per ;
		return E_OK ;
	}
#if MCU_DESIGN == MCU_FREERTOS
	xSemaphoreGive(MCU_SemPerpherialsAPB1);
#endif
	return E_NOT_OK ;
}

Error_State       Mcu_EnumSetPerAPB2(BUS_PERPHERIALS Mcu_Per ) {
#if MCU_DESIGN == MCU_FREERTOS
	xSemaphoreTake(MCU_SemPerpherialsAPB2,portMAX_DELAY);
#endif
	if (Mcu_Per >= APB2_PER_AFIO  && Mcu_Per <= APB2_PER_TIM11){
		PerpherialsAPB2 |= Mcu_Per ;
		return E_OK;
	}
#if MCU_DESIGN == MCU_FREERTOS
	xSemaphoreGive(MCU_SemPerpherialsAPB2);
#endif
	return E_NOT_OK ;
}

Error_State       Mcu_EnumSetPerAHB (BUS_PERPHERIALS Mcu_Per ){
#if MCU_DESIGN == MCU_FREERTOS
	xSemaphoreTake(MCU_SemPerpherialsAHB,portMAX_DELAY);
#endif
	if (Mcu_Per >= AHB_PER_DMA_1  && Mcu_Per <= AHB_PER_SDIO){
		PerpherialsAHB  |= Mcu_Per ;
		return E_OK;
	}
#if MCU_DESIGN == MCU_FREERTOS
	xSemaphoreGive(MCU_SemPerpherialsAHB);
#endif
	return E_NOT_OK ;
}

void  Mcu_VidRunnable (void){
	if (PerpherialsAPB1 != NULL){
		Mcu_SetMode(PerpherialsAPB1);
	}

	if (PerpherialsAPB2 != NULL){
		Mcu_SetMode(PerpherialsAPB2);
	}

	if (PerpherialsAHB  != NULL){
		Mcu_SetMode(PerpherialsAHB) ;
	}
}

void Mcu_SetMode (Mcu_ModeType McuMode){
	McuMode |= McuMode;
	if ( McuMode >= APB1_PER_TIM2  && McuMode <= APB1_PER_DAC ){
		RCC->APB1ENR.Reg  = McuMode ;
	}
	else if (McuMode >= APB2_PER_AFIO  && McuMode <= APB2_PER_TIM11){
		McuMode = McuMode & ~(1<<(31));
		RCC->APB2ENR.Reg  = McuMode ;
	}
	else if (McuMode >= AHB_PER_DMA_1  && McuMode <= AHB_PER_SDIO){
		McuMode = McuMode & ~(1<<(30));
		RCC->AHBENR.Reg  = McuMode ;
	}
	else {
		Det_ReportError(MODULE_ID, Mcu_SetMode_ID, MCU_E_PARAM_MODE);
	}
}


void Mcu_GetVersionInfo(Std_VersionInfoType* versioninfo) {
	versioninfo->moduleID         = MODULE_ID         ;
	versioninfo->sw_major_version = MCU_MAJOR_VERSION ;
	versioninfo->sw_minor_version = MCU_MINOR_VERSION ;
	versioninfo->sw_patch_version = MCU_PATCH_VERSION ;
	versioninfo->vendorID         = VENDOR_ID         ;
}
