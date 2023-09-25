/**
*@file       Port_Private.h
*@version    1.0.0
*@brief      AUTOSAR Base - MCAL General purpose input output.
*@details    It contains all prototypes of used functions and states in port driver
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

#ifndef PORT_PRIVATE_H_
#define PORT_PRIVATE_H_



/****************************************************************************
************************  Version info  *************************************
*****************************************************************************/
#define VENDOR_ID                  150
#define MODULE_ID                  124
#define PORT_MAJOR_VERSION         1
#define PORT_MINOR_VERSION         0
#define PORT_PATCH_VERSION         0


/*
 * Cover_req_[SWS_Port_00051]
 * */
#define PORT_E_PARAM_PIN                      0x0A       /* Invalid Port Pin ID requested                               */
#define PORT_E_DIRECTION_UNCHANGEABLE         0x0B       /* Port Pin not configured as changeable                       */
#define PORT_E_PARAM_INVALID_MODE             0x0D       /* API Port_SetPinMode service called when mode is unchangeable*/
#define PORT_E_MODE_UNCHANGEABLE              0x0E       /* API Port_SetPinMode service called when mode is unchangeable*/
#define PORT_E_UNINIT                         0x0F       /* API service called without module initialization            */
#define PORT_E_PARAM_POINTER                  0x10       /* APIs called with a Null Pointer                             */






/****************************************************************************
***************************  define API IDs  ********************************
*****************************************************************************/
#define Port_SetPinDirection_ID     0x01
#define Port_GetVersionInfo_ID      0x03
#define Port_SetPinMode_ID          0x04




#define BASE_CRL_CRH          4
#define BASE_BSRR             16



typedef struct {
	uint32 CRL;              /* configuration register low */
	uint32 CRH;              /* configuration register High */
	uint32 IDR;              /* input data register */
	uint32 ODR;              /* output data register */
	uint32 BSRR;             /* bit set/reset register*/
	uint32 BRR;              /* bit reset register */
	uint32 LCKR;             /* longer possible to modify the value of the port bit until the next reset. */
}GPIOX_REG;


/* base address */
#define GPIOA        ((volatile GPIOX_REG *)(0x40010800))
#define GPIOB        ((volatile GPIOX_REG *)(0x40010C00))
#define GPIOC        ((volatile GPIOX_REG *)(0x40011000))
#define GPIOD        ((volatile GPIOX_REG *)(0x40011400))




#define AMOUNT_PINS          48


/**
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 */
void    Port_SetPinDirection (Port_PinType Pin,Port_PinDirectionType Direction)  ;
void    Port_SetPinMode      (Port_PinType Pin,Port_PinModeType Mode)            ;

#endif /* PORT_PRIVATE_H_ */
