/**
*@file       Port_Interface.h
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

#ifndef PORT_INTERFACE_H_
#define PORT_INTERFACE_H_

/****************************************************************************
*****************************  Includes   ***********************************
*****************************************************************************/
#include <Det.h>
#include "Std_Types.h"
#include "Bit_Math.h"

/****************************************************************************
****************************  typedef   *************************************
*****************************************************************************/

/**
 * Cover_req_[SWS_Port_00220]
 * Cover_req_[SWS_Port_00013]
 * Cover_req_[SWS_Port_00219]
 * Cover_req_[SWS_Port_00229]
 */
/* we need to Port_PinType with port driver */
typedef enum {
	Port_A0   = 0x00 ,
	Port_A1   = 0x01 ,
	Port_A2   = 0x02 ,
	Port_A3   = 0x03 ,
	Port_A4   = 0x04 ,
	Port_A5   = 0x05 ,
	Port_A6   = 0x06 ,
	Port_A7   = 0x07 ,
	Port_A8   = 0x08 ,
	Port_A9   = 0x09 ,
	Port_A10  = 0x0A ,
	Port_A11  = 0x0B ,
	Port_A12  = 0x0C ,
	Port_A14  = 0x0E ,
	Port_A15  = 0x0F ,

	Port_B0   = 0x10 ,
	Port_B1   = 0x11 ,
	Port_B3   = 0x13 ,
	Port_B4   = 0x14 ,
	Port_B5   = 0x15 ,
	Port_B6   = 0x16 ,
	Port_B7   = 0x17 ,
	Port_B8   = 0x18 ,
	Port_B9   = 0x19 ,
	Port_B10  = 0x1A ,
	Port_B11  = 0x1B ,
	Port_B12  = 0x1C ,
	Port_B13  = 0x1D ,
	Port_B14  = 0x1E ,
	Port_B15  = 0x1F ,

	Port_C0   = 0x20 ,
	Port_C1   = 0x21 ,
	Port_C3   = 0x23 ,
	Port_C4   = 0x24 ,
	Port_C5   = 0x25 ,
	Port_C6   = 0x26 ,
	Port_C7   = 0x27 ,
	Port_C8   = 0x28 ,
	Port_C9   = 0x29 ,
	Port_C10  = 0x2A ,
	Port_C11  = 0x2B ,
	Port_C12  = 0x2C ,
	Port_C13  = 0x2D ,
	Port_C14  = 0x2E ,
	Port_C15  = 0x2F
}Port_PinType;


/**
 * Cover_req_[SWS_Port_00231]
 * Cover_req_[SWS_Port_00231]
 * Cover_req_[SWS_Port_00212]
 */
typedef enum {
	INPUT            =     0x0 , /*00: Input mode*/
	OUTPUT_10        =     0x1 , /*01 : Output mode, max speed 10 MHz. */
	OUTPUT_2         =     0x2 , /*10 : Output mode, max speed 2 MHz. */
	OUTPUT_50        =     0x3 , /*11 : Output mode, max speed 50 MHz. */

	/* In output mode (MODE[1:0] > 00) : */
	G_PUSH_PULL      =     0x0 , /*00: General purpose output push-pull */
	G_OPEN_DRAIN     =     0x4 , /*01: General purpose output Open-drain*/
	A_PUSH_PULL      =     0x8 , /*10: Alternative purpose output push-pull */
	A_OPEN_DRAIN     =     0xC , /*11: Alternative purpose output Open-drain*/
	NONE_OUTPUT      =     0x0 ,

	/*  In input mode (MODE[1:0]=00): */
	ANA_MODE         =     0x0  ,
	FLOATING_INPUT   =     0x4  ,
	INPUT_PULL_DOWN  =     0x8  ,
	NONE_INPUT       =     0x0
}Port_PinModeType;


typedef enum {
	Port_GPIO_A ,
	Port_GPIO_B ,
	Port_GPIO_C ,
}Port_PortType;

/**
 * Cover_req_[SWS_Port_00230]
 * Cover_req_[SWS_Port_00046]
 * Cover_req_[SWS_Port_00220]
 */
typedef uint8 Port_PinDirectionType;     /* PORT_PIN_IN , PORT_PIN_OUT*/




typedef struct {
	Port_PortType               Port_Num       ;
	Port_PinModeType            Mode           ;
}Port_Cfg;



/****************************************************************************
********************* Function definitions **********************************
*****************************************************************************/

/**
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 * Cover_req_[SWS_Port_00141]
 */
void              Port_VidInit     (void)                                                         ;
Error_State       Port_EnumSetterPin(Port_PinType Copy_ChannelId ,Port_PinModeType Copy_Mode)     ;
void              Port_VidRunnable (void)                                                         ;
void              Port_GetVersionInfo  (Std_VersionInfoType* versioninfo)                         ;



/****************************************************************************
***************************  defines  ***************************************
*****************************************************************************/
/**
 * Cover_req_[SWS_Port_00230]
 */
#define PORT_PIN_IN                 0x00
#define PORT_PIN_OUT                0x02

#endif /* PORT_INTERFACE_H_ */
