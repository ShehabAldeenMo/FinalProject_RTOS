/*
*@file       Mcu_Private.h
*@version    2.0.0
*@details    It has the private data like address and bits
*@author     Shehab aldeen mohammed abdalah
*/

/*===========================================================================
*   Platform         : ARM
*   Peripherial      : STM32F103C8T6
*   SW Version       : 2.0.0
============================================================================*/

#ifndef MCU_PRIVATE_H_
#define MCU_PRIVATE_H_

#include "Std_Types.h"

/****************************************************************************
************************  Version info  *************************************
*****************************************************************************/
#define VENDOR_ID                 0x50
#define MODULE_ID                 0x65
#define MCU_MAJOR_VERSION         0x01
#define MCU_MINOR_VERSION         0x01
#define MCU_PATCH_VERSION         0x00



/****************************************************************************
************************  Development Errors Id   ***************************
*****************************************************************************/
#define MCU_E_PARAM_CONFIG        0x0A      /* API service called with wrong parameter */
#define MCU_E_PARAM_CLOCK         0x0B      /* API service called with wrong parameter */
#define MCU_E_PARAM_MODE          0x0c      /* API service called with wrong parameter */
#define MCU_E_PARAM_RAMSECTION    0x0D      /* API service called with wrong parameter */
#define MCU_E_PLL_NOT_LOCKED      0x0E      /* API service called with wrong parameter */
#define MCU_E_UNINIT              0x0F      /* API service called with wrong parameter */
#define MCU_E_PARAM_POINTER       0x10      /* API service called with wrong parameter */
#define MCU_E_INIT_FAILED         0x11      /* API service called with wrong parameter */
#define MCU_E_TIMEOUT             0x12      /* API service called with wrong parameter */


/****************************************************************************
***************************  define API IDs  ********************************
*****************************************************************************/
#define Mcu_Init_ID                 0x01
#define Mcu_InitClock_ID            0x02
#define Mcu_DistributePllClock_ID   0x03
#define Mcu_GetPllStatus_ID         0x04
#define Mcu_SetMode_ID              0x08
#define Mcu_GetVersionInfo_ID       0x09


/****************************************************************************
***************************  Structure address  *****************************
*****************************************************************************/
typedef struct {

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 HSION :1;
			volatile uint32 HSIRDY :1;
			volatile uint32 RES1 :1;
			volatile uint32 HSITRIM :5;
			volatile uint32 HSICAL :8;
			volatile uint32 HSEON :1;
			volatile uint32 HSERDY :1;
			volatile uint32 HSEBYP :1;
			volatile uint32 CSSON :1;
			volatile uint32 RES2 :4;
			volatile uint32 PLLON :1;
			volatile uint32 PLLRDY :1;
			volatile uint32 RES3 :6;
		} B;
	} CR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 SW0 :1;
			volatile uint32 SW1 :1;
			volatile uint32 SWS0 :1;
			volatile uint32 SWS1 :1;
			volatile uint32 HPRE :4;
			volatile uint32 PPRE1 :3;
			volatile uint32 PPRE2 :3;
			volatile uint32 ADCPRE :2;
			volatile uint32 PLLSRC :1;
			volatile uint32 PLLXTPRE :1;
			volatile uint32 PLLMUL :4;
			volatile uint32 USBPRE :1;
			volatile uint32 RES0 :1;
			volatile uint32 MCO :3;
			volatile uint32 RES1 :5;
		} B;
	} CFGR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 LSIRDYF :1;
			volatile uint32 LSERDYF :1;
			volatile uint32 HSIRDYF :1;
			volatile uint32 HSEPDYF :1;
			volatile uint32 PLLRDYF :1;
			volatile uint32 RES0 :2;
			volatile uint32 CSSF :1;
			volatile uint32 LSIRDYIE :1;
			volatile uint32 LSERDYIE :1;
			volatile uint32 HSIRDYIE :1;
			volatile uint32 HSERDYIE :1;
			volatile uint32 PLLRDYIE :1;
			volatile uint32 RES1 :3;
			volatile uint32 LSIRDYC :1;
			volatile uint32 LSERDYC :1;
			volatile uint32 HSIRDYC :1;
			volatile uint32 HSERDYC :1;
			volatile uint32 PLLRDYC :1;
			volatile uint32 RES2 :2;
			volatile uint32 CSSC :1;
			volatile uint32 RES3 :8;
		} B;
	} CIR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 AFIORST :1;
			volatile uint32 RSE0 :1;
			volatile uint32 IOPARST :1;
			volatile uint32 IOPBRST :1;
			volatile uint32 IOPCRST :1;
			volatile uint32 IOPDRST :1;
			volatile uint32 IOPERST :1;
			volatile uint32 IOPFRST :1;
			volatile uint32 IOPGRST :1;
			volatile uint32 ADC1RST :1;
			volatile uint32 ADC2RST :1;
			volatile uint32 TIM1RST :1;
			volatile uint32 SPI1RST :1;
			volatile uint32 TIM8RST :1;
			volatile uint32 USART1RST :1;
			volatile uint32 ADC3RST :1;
			volatile uint32 RES1 :3;
			volatile uint32 TIM9RST :1;
			volatile uint32 TIM10RST :1;
			volatile uint32 TIM11RST :1;
			volatile uint32 RES2 :10;
		} B;
	} APB2RSTR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 TIM2RST :1;
			volatile uint32 TIM3RST :1;
			volatile uint32 TIM4RST :1;
			volatile uint32 TIM5RST :1;
			volatile uint32 TIM6RST :3;
			volatile uint32 TIM7RST :1;
			volatile uint32 TIM12RST :1;
			volatile uint32 TIM13RST :1;
			volatile uint32 TIM14RST :1;
			volatile uint32 RES0 :2;
			volatile uint32 WWDGRST :1;
			volatile uint32 RSE1 :2;
			volatile uint32 SPI2RST :1;
			volatile uint32 SPI3RST :1;
			volatile uint32 RES3 :1;
			volatile uint32 USART2RST :1;
			volatile uint32 USART3RST :1;
			volatile uint32 USART4RST :1;
			volatile uint32 USART5RST :1;
			volatile uint32 I2C1RST :1;
			volatile uint32 I2C2RST :1;
			volatile uint32 USBRST :1;
			volatile uint32 RES4 :1;
			volatile uint32 CANRST :1;
			volatile uint32 RES5 :1;
			volatile uint32 BKPRST :1;
			volatile uint32 PWRRST :1;
			volatile uint32 DACRST :1;
			volatile uint32 RSE6 :2;
		} B;
	} APB1RSTR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 DMA1EN :1;
			volatile uint32 DMA2EN :1;
			volatile uint32 SRAMEN :1;
			volatile uint32 RES0 :1;
			volatile uint32 FLITF :1;
			volatile uint32 RES1 :1;
			volatile uint32 CRCEN :1;
			volatile uint32 RES2 :1;
			volatile uint32 FSMCEN :1;
			volatile uint32 RES3 :1;
			volatile uint32 SDIOEN :1;
			volatile uint32 RES4 :21;
		} B;
	} AHBENR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 AFIOEN :1;
			volatile uint32 RES0 :1;
			volatile uint32 IOPAEN :1;
			volatile uint32 IOPBEN :1;
			volatile uint32 IOPCEN :1;
			volatile uint32 IOPDEN :1;
			volatile uint32 IOPEEN :1;
			volatile uint32 IOPFEN :1;
			volatile uint32 IOPGEN :1;
			volatile uint32 ADC1EN :1;
			volatile uint32 ADC2EN :1;
			volatile uint32 TIM1EN :1;
			volatile uint32 SPI1EN :1;
			volatile uint32 TIM8EN :1;
			volatile uint32 USART1EN :1;
			volatile uint32 ADC3EN :1;
			volatile uint32 RES1 :3;
			volatile uint32 TIM9EN :1;
			volatile uint32 TIM10EN :1;
			volatile uint32 TIM11EN :1;
			volatile uint32 RES2 :10;
		} B;
	} APB2ENR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 TIM2EN :1;
			volatile uint32 TIM3EN :1;
			volatile uint32 TIM4EN :1;
			volatile uint32 TIM5EN :1;
			volatile uint32 TIM6EN :3;
			volatile uint32 TIM7EN :1;
			volatile uint32 TIM12EN :1;
			volatile uint32 TIM13EN :1;
			volatile uint32 TIM14EN :1;
			volatile uint32 RES0 :2;
			volatile uint32 WWDGEN :1;
			volatile uint32 RSE1 :2;
			volatile uint32 SPI2EN :1;
			volatile uint32 SPI3EN :1;
			volatile uint32 RES3 :1;
			volatile uint32 USART2EN :1;
			volatile uint32 USART3EN :1;
			volatile uint32 USART4EN :1;
			volatile uint32 USART5EN :1;
			volatile uint32 I2C1EN :1;
			volatile uint32 I2C2EN :1;
			volatile uint32 USBEN :1;
			volatile uint32 RES4 :1;
			volatile uint32 CANEN :1;
			volatile uint32 RES5 :1;
			volatile uint32 BKPEN :1;
			volatile uint32 PWREN :1;
			volatile uint32 DACEN :1;
			volatile uint32 RSE6 :2;
		} B;
	} APB1ENR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 LSEON :1;
			volatile uint32 LSERDY :1;
			volatile uint32 LSEBYP :1;
			volatile uint32 RES0 :5;
			volatile uint32 RTCEL :2;
			volatile uint32 RES1 :5;
			volatile uint32 RTCEN :1;
			volatile uint32 BDRST :1;
			volatile uint32 RES2 :15;
		} B;
	} BDCR;

	union {
		volatile uint32 Reg;
		struct {
			volatile uint32 LSION :1;
			volatile uint32 LSIRDY :1;
			volatile uint32 RES0 :14;
			volatile uint32 RES1 :8;
			volatile uint32 RMVF :1;
			volatile uint32 RES2 :1;
			volatile uint32 PINRSTF :1;
			volatile uint32 PORSTF :15;
			volatile uint32 RES3 :4;
		} B;
	} CSR;
} RCC_REG;

#define RCC          ((volatile RCC_REG *)  (0x40021000))

#define NULL           0

#define HSE_CRSTYAL    0
#define HSE_RC         1
#define CSS_ON         1
#define CSS_OFF        0

#define TIMEOUT_ROOF   1000

/* Input       : McuMode
 * Return      : None
 * Description : This service activates the MCU power modes.
 * */
void              Mcu_SetMode            (Mcu_ModeType McuMode)                       ;

#endif /* MCU_PRIVATE_H_ */
