/**
*@file       MGPIO_Private.h
*@version    1.0.0
*@brief      AUTOSAR Base - Platform dependend data type defination.
*@details    It has the private data like address and bits
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
#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_

#include "Std_Types.h"


/*============================================================================*/

#define NUM_OF_PINS    48

/*============================================================================*/

/****************************************************************************
************************  Version info  *************************************
*****************************************************************************/
#define VENDOR_ID                  150
#define MODULE_ID                  120
#define PORT_MAJOR_VERSION         1
#define PORT_MINOR_VERSION         0
#define PORT_PATCH_VERSION         0


/****************************************************************************
************************  Development Errors Id   ***************************
*****************************************************************************/

/**
 * Cover_req_[SWS_Dio_00175]
 * Cover_req_[SWS_Dio_00177]
 * Cover_req_[SWS_Dio_00178]
 * Cover_req_[SWS_Dio_00188]
 */
#define DIO_E_PARAM_INVALID_CHANNEL_ID        0x0A      /* Invalid channel requested         */
#define DIO_E_PARAM_INVALID_PORT_ID           0x14      /* Invalid port requested            */
#define DIO_E_PARAM_INVALID_GROUP             0x1F      /* Invalid channel group requested   */
#define DIO_E_PARAM_POINTER                   0x20      /* API service called with a NULL pointer */



#define BASE_CRL_CRH          4
#define BASE_BSRR             16
/* address */

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






/****************************************************************************
***************************  define API IDs  ********************************
*****************************************************************************/
#define Dio_ReadChannel_ID          0x00
#define Dio_WriteChannel_ID         0x01
#define Dio_ReadPort_ID             0x02
#define Dio_WritePort_ID            0x03
#define Dio_GetVersionInfo_ID       0x12
#define Dio_FlipChannel_ID          0x11



#define DIO_IN                 0x00
#define DIO_OUT                0x02



#endif /* DIO_PRIVATE_H_ */
