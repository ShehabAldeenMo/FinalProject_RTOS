/**
*@file       DIO_Interface.h
*@version    1.0.0
*@brief      AUTOSAR Base - MCAL General purpose input output.
*@details    It contains all prototypes of used functions and states
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
#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

/****************************************************************************
*****************************  Includes   ***********************************
*****************************************************************************/
#include <Det.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Dio_Cfg.h"

/****************************************************************************
****************************  typedef   *************************************
*****************************************************************************/

/**
 * Cover_req_[SWS_Dio_00182]
 * Cover_req_[SWS_Dio_00015]
 * Cover_req_[SWS_Dio_00017]
 */
/* we need to Port_PinType with port driver */
typedef enum {
	Dio_A0   = 0x00 ,
	Dio_A1   = 0x01 ,
	Dio_A2   = 0x02 ,
	Dio_A3   = 0x03 ,
	Dio_A4   = 0x04 ,
	Dio_A5   = 0x05 ,
	Dio_A6   = 0x06 ,
	Dio_A7   = 0x07 ,
	Dio_A8   = 0x08 ,
	Dio_A9   = 0x09 ,
	Dio_A10  = 0x0A ,
	Dio_A11  = 0x0B ,
	Dio_A12  = 0x0C ,
	Dio_A14  = 0x0E ,
	Dio_A15  = 0x0F ,

	Dio_B0   = 0x10 ,
	Dio_B1   = 0x11 ,
	Dio_B3   = 0x13 ,
	Dio_B4   = 0x14 ,
	Dio_B5   = 0x15 ,
	Dio_B6   = 0x16 ,
	Dio_B7   = 0x17 ,
	Dio_B8   = 0x18 ,
	Dio_B9   = 0x19 ,
	Dio_B10  = 0x1A ,
	Dio_B11  = 0x1B ,
	Dio_B12  = 0x1C ,
	Dio_B13  = 0x1D ,
	Dio_B14  = 0x1E ,
	Dio_B15  = 0x1F ,

	Dio_C0   = 0x20 ,
	Dio_C1   = 0x21 ,
	Dio_C3   = 0x23 ,
	Dio_C4   = 0x24 ,
	Dio_C5   = 0x25 ,
	Dio_C6   = 0x26 ,
	Dio_C7   = 0x27 ,
	Dio_C8   = 0x28 ,
	Dio_C9   = 0x29 ,
	Dio_C10  = 0x2A ,
	Dio_C11  = 0x2B ,
	Dio_C12  = 0x2C ,
	Dio_C13  = 0x2D ,
	Dio_C14  = 0x2E ,
	Dio_C15  = 0x2F
}Dio_ChannelType;

/**
 * Cover_req_[SWS_Dio_00183]
 * Cover_req_[SWS_Dio_00018]
 * Cover_req_[SWS_Dio_00181]
 * Cover_req_[SWS_Dio_00020]
 */
typedef enum {
	Dio_GPIO_A ,
	Dio_GPIO_B ,
	Dio_GPIO_C ,
}Dio_PortType;

/**
 * Cover_req_[SWS_Dio_00185]
 * Cover_req_[SWS_Dio_00186]
 */
typedef uint8  Dio_LevelType;            /* Range: STD_LOW, STD_HIGH */

typedef uint8 Dio_PortLevelType;        /* Range:If the µC owns ports of different port widths
                                           (e.g. 4, 8,16...Bit) Dio_PortLevelType inherits the size
                                           of the largest port*/


/*====================================================================================*/

typedef struct {
	Dio_PortType                Port_Num       ;
	Dio_PortLevelType           Level          ;
}Dio_Cfg;

/*====================================================================================*/



/****************************************************************************
********************* Function definitions **********************************
*****************************************************************************/

/*====================================================================================*/
void              Dio_VidInit     (void)                                                         ;
Error_State       Dio_EnumSetterPin(Dio_ChannelType Copy_ChannelId ,Dio_LevelType Copy_Level)    ;
Error_State       Dio_EnumGetterPin(Dio_ChannelType Copy_ChannelId ,Dio_LevelType* Copy_Level)   ;
void              Dio_VidRunnable (void)                                                         ;
void              Dio_GetVersionInfo(Std_VersionInfoType* versioninfo)                           ;
/*====================================================================================*/

/**
 * Cover_req_[SWS_Dio_00133]
 * Cover_req_[SWS_Dio_00028]
 * Cover_req_[SWS_Dio_00029]
 * Cover_req_[SWS_Dio_00079]
 * Cover_req_[SWS_Dio_00136]
 * Cover_req_[SWS_Dio_00139]
 * Cover_req_[SWS_Dio_00190]
 */

/*    Supporting functions     */
Dio_LevelType     Dio_ReadChannel (Dio_ChannelType ChannelId)                         ;
void              Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level)     ;
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)                                   ;
void              Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level)          ;
Dio_LevelType     Dio_FlipChannel(Dio_ChannelType ChannelId)                          ;



#endif /* DIO_INTERFACE_H_ */
