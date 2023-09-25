/*
 * File        : USART_config.h
 * Author      : Yassin
 * Date        : 06/08/2023

 */
#ifndef UART_Config_h
#define UART_Config_h

/**
 * \file UART_cfg.h
 * \section Brief
 * This is UART configuration
 * \section Details
 * The available configuration allowed in the UART
 * \section Scope
 * Private
 *
 *
 *
*/

#include "UART_Private.h"

typedef enum
{
	oversamplingby16,
	oversamplingby8
	
}OVER8;

typedef enum
{
	UE_DISEBLE,
	UE_ENABLE
	
}UE;
typedef enum
{
	 DATA_8_BIT,
	 DATA_9_BIT
	
}M;
typedef enum
{
	 IdleLine,
	 AddressMark
	
}WAKE;

typedef enum
{
	Paritycontroldisabled,
	 Evenparity=1,
	 ODDparity=3
}PSnPCE;

typedef enum
{
	TE_DISEBLE,
	TE_ENABLE

}TE;
typedef enum
{
	 RE_DISEBLE,
	 RE_ENABLE

}RE;
typedef enum
{
	  OneStopbit,
	  HalfStopbit,
	  towStopbit,
	  oneAndHalfStopbit

}STOP;

typedef enum
{
	USART_MODE_ONLY_RX = 1,
	USART_MODE_ONLY_TX,
	USART_MODE_TXRX
}USART_MODE;

/* USART Number */
typedef enum
{
	UART1,
	UART2,
	UART3
}USART_ID;

/*USART Buad Rate */
typedef enum
{
   USART_STD_BAUD_2400    = 0xd05,
   USART_STD_BAUD_9600    = 0x341,
   USART_STD_BAUD_19200   = 0x1a0,
   USART_STD_BAUD_38400   = 0xd0,
   USART_STD_BAUD_57600   = 0x8a,
   USART_STD_BAUD_115200  = 0x45,
   USART_STD_BAUD_230400  = 0x22,
   USART_STD_BAUD_460800  = 0x11,
   USART_STD_BAUD_921600  = 0x8,
   USART_STD_BAUD_2250000 = 0x3,
   USART_STD_BAUD_4500000 = 0x1
}USART_BAUD;

#endif
