/**
*@file       MCU_Interface.h
*@version    1.1.0
*@brief      AUTOSAR Base - MCAL General purpose input output.
*@details    It contains all prototypes of used functions and states
*@author     Shehab aldeen mohammed abdalah
*/

/*===========================================================================
*   Project          : AUTOSAR  4.3.1 MCAL
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
*   AUTOSAR Version  : R20-11
*   AUTOSAR Revision : ASR_REL_4_1_REV_0001
*   SW Version       : 1.0.0
============================================================================*/
#ifndef MCU_INTERFACE_H_
#define MCU_INTERFACE_H_


/****************************************************************************
*****************************  Includes   ***********************************
*****************************************************************************/
#include <Det.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "MCU_Cfg.h"


/****************************************************************************
****************************  typedef   *************************************
*****************************************************************************/

/**
 * Cover_req_[SWS_Mcu_00250]
 * Cover_req_[SWS_Mcu_00230]
 * Cover_req_[SWS_Mcu_00231]
 */
typedef uint8 Mcu_PllStatusType;

#define MCU_PLL_LOCKED                0x00
#define MCU_PLL_UNLOCKED              0x01
#define MCU_PLL_STATUS_UNDEFINED      0x02


/**
 * Cover_req_[SWS_Mcu_00252]
 * Cover_req_[SWS_Mcu_00234]
 * Cover_req_[SWS_Mcu_00134]
 */
typedef uint32 Mcu_ResetType;

#define MCU_POWER_ON_RESET          0x00
#define MCU_WATCHDOG_RESET          0x01
#define MCU_SW_RESET                0x02
#define MCU_RESET_UNDEFINED         0x03


/**
 * Cover_req_[SWS_Mcu_00251]
 */
typedef  uint32 Mcu_ClockType   ;  /* used any enum except bus peripherals  */


/**
 * Cover_req_[SWS_Mcu_00254]
 */
typedef uint32 Mcu_ModeType ; /* used bus peripherals enum */




/****************************************************************************
****************************    Enums  **************************************
*****************************************************************************/

typedef enum {
	HSE_CLOCK  = 0x00010000,
	HSI_CLOCK  = 0x00000001,
	PLL_CLOCK  = 0x01000000
}CLOCK;

typedef enum {
	HSE_SW     = 0x00000001,
	HSI_SW     = 0x00000000,
	PLL_SW     = 0x00000002
}SW_ENABLE;

typedef enum {
	PLL_MUL_2  = 0x00000000,
	PLL_MUL_3  = 0x00010000,
	PLL_MUL_4  = 0x00020000,
	PLL_MUL_5  = 0x00030000,
	PLL_MUL_6  = 0x00100000,
	PLL_MUL_7  = 0x00110000,
	PLL_MUL_8  = 0x00120000,
	PLL_MUL_9  = 0x00130000,
	PLL_MUL_10 = 0x00200000,
	PLL_MUL_11 = 0x00210000,
	PLL_MUL_12 = 0x00220000,
	PLL_MUL_13 = 0x00230000,
	PLL_MUL_14 = 0x00300000,
	PLL_MUL_15 = 0x00310000,
	PLL_MUL_16 = 0x00320000,
	PLL_NONE_0 = 0x00000000
}PLL_MUL;

typedef enum {
	PLL_HSI_DIVIDED = 0x00000000,
	PLL_HSE_NOT_DIV = 0x00010000,
	PLL_HSE_DIVIDED = 0x00030000,
	PLL_NONE_ENTRY  = 0x00000000
}PLL_ENTRY;

typedef enum {
	ADC_PRE_DIVIDED_2 = 0x00000000,
	ADC_PRE_DIVIDED_4 = 0x00004000,
	ADC_PRE_DIVIDED_6 = 0x00008000,
	ADC_PRE_DIVIDED_8 = 0x0000C000
}ADC_PRESCALAR;

typedef enum {
	APB2__PRE_NOT_DIVIDED = 0x00000000,
	APB2__PRE_DIVIDED_2   = 0x00002000,
	APB2__PRE_DIVIDED_4   = 0x00002800,
	APB2__PRE_DIVIDED_8   = 0x00003000,
	APB2__PRE_DIVIDED_16  = 0x00003800,

	APB1_PRE_NOT_DIVIDED = 0x00000000,
	APB1_PRE_DIVIDED_2   = 0x00002000,
	APB1_PRE_DIVIDED_4   = 0x00002800,
	APB1_PRE_DIVIDED_8   = 0x00003000,
	APB1_PRE_DIVIDED_16  = 0x00003800,

	AHB_PRE_NOT_DIVIDED = 0x00000000,
	AHB_PRE_DIVIDED_2   = 0x00000080,
	AHB_PRE_DIVIDED_4   = 0x00000090,
	AHB_PRE_DIVIDED_8   = 0x000000A0,
	AHB_PRE_DIVIDED_16  = 0x000000B0,
	AHB_PRE_DIVIDED_64  = 0x000000C0,
	AHB_PRE_DIVIDED_128 = 0x000000D0,
	AHB_PRE_DIVIDED_256 = 0x000000E0,
	AHB_PRE_DIVIDED_512 = 0x000000F0
}BUS_PRESCALAR;

typedef enum {

	APB1_PER_TIM2  = 0x00000001,
	APB1_PER_TIM3  = 0x00000002,
	APB1_PER_TIM4  = 0x00000004,
	APB1_PER_TIM5  = 0x00000008,
	APB1_PER_TIM6  = 0x00000010,
	APB1_PER_TIM7  = 0x00000020,
	APB1_PER_TIM12 = 0x00000040,
	APB1_PER_TIM13 = 0x00000080,
	APB1_PER_TIM14 = 0x00000100,
	APB1_PER_WWD   = 0x00000800,
	APB1_PER_SPI2  = 0x00004000,
	APB1_PER_SPI3  = 0x00008000,
	APB1_PER_USART2= 0x00020000,
	APB1_PER_USART3= 0x00040000,
	APB1_PER_USART4= 0x00080000,
	APB1_PER_USART5= 0x00100000,
	APB1_PER_I2C_1 = 0x00200000,
	APB1_PER_I2C_2 = 0x00400000,
	APB1_PER_USB   = 0x00800000,
	APB1_PER_CAN   = 0x02000000,
	APB1_PER_BKP   = 0x08000000,
	APB1_PER_PWR   = 0x10000000,
	APB1_PER_DAC   = 0x20000000,

	APB2_PER_AFIO  = 0x80000001,
	APB2_PER_GPIOA = 0x80000004,
	APB2_PER_GPIOB = 0x80000008,
	APB2_PER_GPIOC = 0x80000010,
	APB2_PER_GPIOD = 0x80000020,
	APB2_PER_GPIOE = 0x80000040,
	APB2_PER_GPIOF = 0x80000080,
	APB2_PER_GPIOG = 0x80000100,
	APB2_PER_ADC1  = 0x80000200,
	APB2_PER_ADC2  = 0x80000400,
	APB2_PER_TIM1  = 0x80000800,
	APB2_PER_SPI1  = 0x80001000,
	APB2_PER_TIM8  = 0x80002000,
	APB2_PER_USART1= 0x80004000,
	APB2_PER_ADC3  = 0x80008000,
	APB2_PER_TIM9  = 0x80080000,
	APB2_PER_TIM10 = 0x80100000,
	APB2_PER_TIM11 = 0x80200000,

	AHB_PER_DMA_1 = 0x40000001,
	AHB_PER_DMA_2 = 0x40000002,
	AHB_PER_SRAM  = 0x40000004,
	AHB_PER_FLITF = 0x40000010,
	AHB_PER_CRCE  = 0x40000040,
	AHB_PER_FSMC  = 0x40000080,
	AHB_PER_SDIO  = 0x40000400,

}BUS_PERPHERIALS;




/****************************************************************************
**************************** Structure **************************************
*****************************************************************************/

/* Description:
 * The Function to initialize the Timer according to passed configuration struct
 * 1.select which clock that you need
 * 2.select PLL multiplication Number
 * 3.select PLL entry clock
 * 4.select ADC_PRESCALAR
 * 5.select APB2_PRESCALAR
 * 6.select APB1_PRESCALAR
 * 7.select AHB_PRESCALAR
 * 8.select SW_ENABLE_clock
 * 9.define the value of calibrated clock in HSITRIM
 */
typedef struct {
	CLOCK              CLOCK_SYS      ;
	PLL_MUL            PLL_MUL_NUMBER ;
	PLL_ENTRY          PLL_ENTRY_CLOCK;
	ADC_PRESCALAR      ADC_PRESCALAR_ ;
	BUS_PRESCALAR      APB2_PRESCALAR_;
	BUS_PRESCALAR      APB1_PRESCALAR_;
	BUS_PRESCALAR      AHB_PRESCALAR_ ;
	SW_ENABLE          SW_ENABLE_CLOCK;
	uint8              HSITRIM        ;
}Mcu_ConfigType;

/****************************************************************************
********************* Function definitions **********************************
*****************************************************************************/

/**
 * Cover_req_[SWS_Mcu_00153]
 * Cover_req_[SWS_Mcu_00026]
 */

/* Input       : void
 * Return      : void
 * Description : This service initializes the MCU driver
 * */
void              Mcu_Init (void);

#if McuInitClock  == TRUE
/**
 * Cover_req_[SWS_Mcu_00137]
 * Cover_req_[SWS_Mcu_00155]
 * Cover_req_[SWS_Mcu_00138]
 * Cover_req_[SWS_Mcu_00139]
 */

/* Input       : ClockSetting
 * Return      : Std_ReturnType - E_OK: Command has been accepted
 *                              - E_NOT_OK: Command has not been accepted
 *
 * Description : The function Mcu_InitClock shall initialize the PLL and other MCU specific
 *               clock options. The clock configuration parameters are provided via the configuration
 *               structure (as identify the pll multiplication or HSE speed or HSI speed)
 *
 *               The function Mcu_InitClock shall start the PLL lock procedure (if PLL shall be initialized)
 *               so you can,t change any configurations in clock
 *
 *               The MCU module’s environment shall only call the function Mcu_InitClock after the MCU
 *               module has been initialized using the function Mcu_Init
 * */
Std_ReturnType    Mcu_InitClock (Mcu_ClockType ClockSetting)                          ;

#endif

#if McuNoPll  == TRUE
/**
 * Cover_req_[SWS_Mcu_00156]
 * Cover_req_[SWS_Mcu_00140]
 * - The function Mcu_DistributePllClock shall activate the PLL clock to the MCU clock distribution.
 *
 * Cover_req_[SWS_Mcu_00141]
 * - The function Mcu_DistributePllClock shall remove the
 * current clock source (for example internal oscillator clock) from MCU clock  distribution
 * - The MCU module’s environment shall only call the function
 * Mcu_DistributePllClock after the status of the PLL has been detected as
 * locked by the function Mcu_GetPllStatus.
 *
 * Cover_req_[SWS_Mcu_00142]
 * - If the function Mcu_DistributePllClock is called before
 * PLL has locked, this function shall return E_NOT_OK immediately, without any further action
 *
 * Cover_req_[SWS_Mcu_00142]
 * The function Mcu_DistributePllClock shall return without affecting the MCU hardware if the PLL clock
 * has been automatically activated by the MCU hardware
 *
 */

/* Input       : ClockSetting
 * Return      : Std_ReturnType - E_OK: Command has been accepted
 *                              - E_NOT_OK: Command has not been accepted
 *
 * Description : This service activates the PLL clock to the MCU clock distribution
 * */
Std_ReturnType    Mcu_DistributePllClock (void)                                       ;



/**
 * Cover_req_[SWS_Mcu_00157]
 * Cover_req_[SWS_Mcu_00008]
 * Cover_req_[SWS_Mcu_00132]
 * - The function Mcu_GetPllStatus shall return
 * MCU_PLL_STATUS_UNDEFINED if this function is called prior to calling of the function Mcu_Init
 */

/* Input       : ClockSetting
 * Return      : Std_ReturnType - E_OK: Command has been accepted
 *                              - E_NOT_OK: Command has not been accepted
 *
 * Description : This service initializes the PLL and other MCU specific clock options
 * */
Mcu_PllStatusType Mcu_GetPllStatus (void)                                             ;

#endif

/*
 * To Follow CCD rules in access resources.
 * */
Error_State       Mcu_EnumSetPerAPB1(BUS_PERPHERIALS Mcu_Per )                        ;
Error_State       Mcu_EnumSetPerAPB2(BUS_PERPHERIALS Mcu_Per )                        ;
Error_State       Mcu_EnumSetPerAHB (BUS_PERPHERIALS Mcu_Per )                        ;
void              Mcu_VidRunnable (void)                                              ;


/**
 * Cover_req_[SWS_Mcu_00161]
 * Cover_req_[SWS_Mcu_00147]
 * Cover_req_[SWS_Mcu_00148]
 */

/* Input       : Pointer to where to store the version information of this module.
 * Return      : None
 * Description : This service activates the MCU power modes.
 * */
void             Mcu_GetVersionInfo     (Std_VersionInfoType* versioninfo)            ;

#endif /* MCU_INTERFACE_H_ */
