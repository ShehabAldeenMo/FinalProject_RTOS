/*
 * UART_cfg.c
 *
 *  Created on: Aug 21, 2023
 *      Author: Shehab El-deen Mohammed & Ali Mamdouh
 */
/**
 * \file UART_CFG.c
 * \section Brief
 * This is UART services CFG
 * \section Details
 * The Configrable File For UART
 * \section Scope
 * Public
 *
 *
 *
*/
#include "UART_Interface.h"

USART_ConfigType arrOfUART[NUMBER_OF_USED_UARTS]=
{
		/************************************ USART2 ************************************/
		{UART3, USART_STD_BAUD_9600, DATA_8_BIT, OneStopbit, USART_MODE_TXRX, Paritycontroldisabled, UE_ENABLE},
		/************************************ USART1 ************************************/
		{UART2, USART_STD_BAUD_9600, DATA_8_BIT, OneStopbit, USART_MODE_TXRX, Paritycontroldisabled, UE_DISEBLE},
		/************************************ USART3 ************************************/
		{UART1, USART_STD_BAUD_9600, DATA_8_BIT, OneStopbit, USART_MODE_TXRX, Paritycontroldisabled, UE_DISEBLE}
};
