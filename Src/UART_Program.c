/*
 * File        : USART_program.c
 * Author      : Yassin - RABIE - ALIAA
 * Date        : 25/08/2023
 * Version     : 5

 */

/**
 * \file UART_Program.c
 * \section Brief
 * This is UART services Prg
 * \section Details
 * The module drives the UART functionality for SW
 * \section Scope
 * Public
 *
 *
 *
*/
#include <Mcu_Interface.h>
#include "UART_Interface.h"
#include "UART_Private.h"
#include "UART_Config.h"
#include "Port_Interface.h"
#include "DIO_Interface.h"

/*
DIV BY 16
ENABLE USART
CHOICE 8 BIT Copy_U8Data
NO ADRESS AND NO LINE
ParitycontrolEnabled
CHOICE EVEN PARITY BIT
TRANSIMATE IS ENABLE
BOUD RATE 9600

 */

uint8 received_string[MAX_STRING_TO_BE_RECEIVED_USART1];

volatile USART_t *UART_PTR = NULL ;


/***********************************[1]***********************************************
 * Service Name: UART_VidInit
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Renterancy: Reentrant
 * Parameters (in): Constant USART_ConfigType
 * Parameters (InOut): None
 * Parameters (Out): None
 * Return value: None
 * Description: Set the configration of the given UART(s) to be enabled.
 ************************************************************************************/
 
 
 /**
 * \section Service_Name
 * UART_VidInit
 *
 * \section Service_ID_hex
 * 0x01
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters_in
 * \param[in] Constant USART_ConfigType(Pointer of Arrays)\PeripheralID--->Global Array to configrate the used UART(S).
 * \param[out] None
 * \param[in-out] None
 *
 * \section Return_value
 * None
 *
 * \section Description
 *  Set the configration of the given UART(s) to be enabled.
 * 
 * \section Activity_diagram
 *
 * \startuml 
 *   start
 *   :Get all the configurations from The Global Array to Struct;
 *	if(Is there any UARTS configurations in the global Array?) then (Yes)
 *	repeat
 *  if (UART is enabled to Initalize) then (Yes)
 *  switch (Which UART is used?)
 *	case ( A)
 *	:UART1;
 *	case ( B)
 *	:UART2;
 *	case ( C)
 *	:UART3;
 *	endswitch
 *	:Enable the RCC for the UART;
 *  :Configurate the UART Pins;
 *	:Set the UART WordSize;
 *	:Set the UART Stop-Bits;
 *  :Set UART Mode;
 *  if (ParityControl Bit is Enabled) then (Yes)
 *	switch (ParitySelection)
 *	case ( A)
 * 	:EVEN;
 *	case ( B)
 *	:ODD;
 *	endswitch
 *  else (no)
 *  endif
 *	:Set the buadrate;
 *	:ENABLE THE UART;
 * 	else (no)
 * endif
 *	repeat while (There's other UARTs left to Initalize?) is (Yes) not (no)
 *	else (no)
 *	endif
 * end
 * \enduml
 */

extern USART_ConfigType arrOfUART[NUMBER_OF_USED_UARTS];

void USART_VidInit() {
	if (arrOfUART == NULL) {
		return;
	}

	for (int i = 0; i < NUMBER_OF_USED_UARTS; i++) {
		if (arrOfUART->UART_Array[i].USARTEnableType == UE_DISEBLE)
			continue;

		switch (arrOfUART->UART_Array[i].USARTid) {
		case UART1:
			Port_EnumSetterPin(Port_A9, PORT_PIN_OUT | A_PUSH_PULL);
			Port_EnumSetterPin(Port_A10, PORT_PIN_IN | FLOATING_INPUT);
			Port_VidRunnable();
			UART_PTR = MUART1;
			break;
		case UART2:
			Port_EnumSetterPin(Port_A2, PORT_PIN_OUT | A_PUSH_PULL);
			Port_EnumSetterPin(Port_A3, PORT_PIN_IN | FLOATING_INPUT);
			Port_VidRunnable();
			UART_PTR = MUART2;
			break;
		case UART3:
			Port_EnumSetterPin(Port_B10, PORT_PIN_OUT | A_PUSH_PULL);
			Port_EnumSetterPin(Port_B11, PORT_PIN_IN | FLOATING_INPUT);
			Port_VidRunnable();
			UART_PTR = MUART3;
			break;
		}

		UART_PTR->BR.R = arrOfUART->UART_Array[i].USARTBaud;
		UART_PTR->CR1.B.M = arrOfUART->UART_Array[i].USARTWordLength;
		UART_PTR->CR2.B.STOP = arrOfUART->UART_Array[i].USARTStopBits;
		UART_PTR->CR1.B.REnTE = arrOfUART->UART_Array[i].USARTMode;
		UART_PTR->CR1.B.PSnPCE = arrOfUART->UART_Array[i].USARTParitySelection;
		/*Start the USART*/
		UART_PTR->CR1.B.UE = UE_ENABLE;
	}
}
/***********************************[2]***********************************************
 * Service Name: USART_ReceiveByte
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Renterancy: Non-Reentrant
 * Parameters (in): UART_ID
 * Parameters (InOut): None
 * Parameters (Out): None
 * Return value: Copy_U8Data
 * Description: Recieve the Sent Data by the user.
 ************************************************************************************/
 
/**
 * \section Service_Name
 * USART_ReceiveByte
 *
 * \section Service_ID_hex
 * 0x02
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Non-Reentrant
 *
 * \section Parameters_in
 * \param[in] UART_ID
 * \param[out] None
 * \param[in-out] None
 *
 * \section Return_value
 * Copy_U8Data
 *
 * \section Description
 *  Recieve the Sent Data by the user.
 * 
 * \section Sequence_diagram
 *
 * @startuml
 * participant USART1_ReceiveByte as "USART1_ReceiveByte()"
 * participant UART_ID as "UART_ID"
 * participant Copy_U8Data as "Copy_U8Data"
 * participant Copy_U32TimeOut as "Copy_U32TimeOut"
 *
 * USART_ReceiveByte -> UART_ID : Read UART ID
 * activate UART_ID
 * UART_ID -> Copy_U8Data : Initialize Copy_U8Data variable
 * activate Copy_U8Data
 *
 *
 * @enduml
 * \section Activity_diagram
 * \startuml 
 *   start
 *  :Initialize local variables;
 *  switch (Which UART is used?)
 *	case ( A)
 *	:UART1;
 *	case ( B)
 *	:UART2;
 *	case ( C)
 *	:UART3;
 *	endswitch
 *  while (Data has been sent) is (No)
 *  if(Timeout is less than 5s) then (Yes)
 *  :Increment timeout;
 *	else (no)
 *  :Data equals rublish;
 *	end
  *	endif
 *  endwhile (Yes)
 *  :Data is received;
 *  :timeout equals 0;
 *	:return recievedData;
 * end
 * \enduml
*/
uint8 USART_ReceiveByte(USART_ID UART_ID) {
	uint8 Copy_U8Data;
	uint32 Copy_U32TimeOut = 0;
	//UART_PTR->DR.R = 0;
	switch (UART_ID) {
	case UART1:
		UART_PTR = MUART1;
		break;
	case UART2:
		UART_PTR = MUART2;
		break;
	case UART3:
		UART_PTR = MUART3;
		break;
	}
	while ((UART_PTR->SR.B.RXNE == 0) && (Copy_U32TimeOut < THRESHOLD_VALUE)) {
		Copy_U32TimeOut++;
	}

	if (Copy_U32TimeOut == THRESHOLD_VALUE) {
		Copy_U8Data = 255;
	} else {
		Copy_U8Data = UART_PTR->DR.R;
		Copy_U32TimeOut = 0;

	}
	UART_PTR->SR.B.RXNE = 0;
	return Copy_U8Data;
}
/***********************************[3]***********************************************
 * Service Name: USART_VidSendChar
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Renterancy: Non-Reentrant
 * Parameters (in): UART_ID, Copy_U8Data
 * Parameters (InOut): None
 * Parameters (Out): None
 * Return value: void
 * Description: send char .
 ************************************************************************************/
  /**
 * \section Service_Name
 * USART_VidSendChar
 *
 *
 * \section Service_ID_hex
 * 0x06
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Non-Reentrant
 *
 * \section Parameters_in
 * \param[in] UART_ID, Copy_U8Data
 * \param[out] None
 * \param[in-out] None
 *
 * \section Return_value
 * Void
 *
 * \section Description
 *  send char.
 *
 * \section Sequence_diagram
 *
 * \startuml
 *  actor User
 *
 *	User -> Code: USART_VidSendChar(Copy_U8Data)
 *	activate Code
 *
 *	Code -> MUART: Choose which UART is used
 *	Code -> MUART: Set MUART->SR.B.TC to 0
 *	activate MUART
 *	MUART --> Code: MUART->SR.B.TC set to 0
 *	deactivate MUART
 *
 *	Code -> MUART: Assign Copy_U8Data to MUART->DR.R
 *	activate MUART
 *	MUART --> Code: Copy_U8Data assigned to MUART->DR.R
 *	deactivate MUART
 *
 *	Code -> MUART: Check if MUART->SR.B.TC is false
 *	activate MUART
 *	alt (Condition is true)
 *	  MUART --> Code: Condition is true
 *	  Code -> Code: Continue loop
 *	  loop
 *	else (Condition is false)
 *	  MUART --> Code: Condition is false
 *	  Code -> Code: Break loop
 *	end
 *	deactivate MUART
 *
 *	Code -> MUART: Set MUART->SR.B.TC to 0
 *	activate MUART
 *	MUART --> Code: MUART->SR.B.TC set to 0
 *	deactivate MUART
 *
 *	deactivate Code
 *
 * \enduml
 *
 * \section Activity_diagram
 *
 * \startuml
 *  start
 *  switch (Which UART is used?)
 *	case ( A)
 *	:UART1;
 *	case ( B)
 *	:UART2;
 *	case ( C)
 *	:UART3;
 *	endswitch
 *  :Initialize MUART->SR.B.TC to 0;
 *  :MUART->DR.R = Copy_U8Data;
 *	repeat
 *	  :Check if MUART->SR.B.TC is false;
 *	  if (Condition)
 *		--> repeat
 *	  else
 *		break
 *   endif
 *	repeat while (Condition)
 *	:MUART->SR.B.TC = 0;
 * end
 * \enduml
 */
void USART_VidSendChar(USART_ID UART_ID, uint8 Copy_U8Data) {
	switch (UART_ID) {
	case UART1:
		UART_PTR = MUART1;
		break;
	case UART2:
		UART_PTR = MUART2;
		break;
	case UART3:
		UART_PTR = MUART3;
		break;
	}

	UART_PTR->SR.B.TC = 0;
	UART_PTR->DR.R = Copy_U8Data;
	while (!UART_PTR->SR.B.TC);
}

 /***********************************[4]***********************************************
  * Service Name: USART_TransmitString
  * Service ID[hex]: 0x04
  * Sync/Async: Synchronous
  * Renterancy: Non-Reentrant
  * Parameters (in): UART_ID, Pointer to string
  * Parameters (InOut): None
  * Parameters (Out): None
  * Return value: None
  * Description: Transmit the  DataString .
  ************************************************************************************/
  /**
  * \section Service_Name
  * USART_TransmitString
  *
  *
  * \section Service_ID_hex
  * 0x08
  *
  * \section Sync_Async
  * Synchronous
  *
  * \section Renterancy
  * Non-Reentrant
  *
 * \section Parameters_in
 * \param[in] UART_ID, Pointer to string
 * \param[out] None
 * \param[in-out] None
  * \section Return_value
  * Void
  *
  * \section Description
  *  send a string of data.
  *
  * \section Sequence_diagram
  * \startuml
  *  actor User
  *
  * User -> Code: SendStringWithHash(string_Copy_U8Data)
  * activate Code
  *
  * Code -> Code: Initialize i to 0
  * Code -> Code: Check if string_Copy_U8Data[i] is not null
  * activate Code
  *
  * Code -> UART: Send character string_Copy_U8Data[i]
  * activate UART
  * UART --> Code: Character sent
  *
  * Code -> Code: Increment i
  * Code --> Code: Check if string_Copy_U8Data[i] is not null
  * deactivate Code
  *
  * Code -> Code: Check if string_Copy_U8Data[i] is not null
  * activate Code
  * Code -> UART: Choose UART_ID
  * Code -> UART: Send character string_Copy_U8Data[i]
  * activate UART
  * UART --> Code: Character sent
  *
  * Code -> UART: Send character '#'
  * activate UART
  * UART --> Code: Character sent
  *
  * deactivate Code
  * \enduml
  *
  * \section Activity_diagram
  *
  * \startuml
  *  start
  * :Initialize i to 0;
  *  repeat
  * :Check if string_Copy_U8Data[i] is not null;
  * if (Condition)
  *  :Send string_Copy_U8Data[i] over UART;
  *     --> Continue
  *    else
  *      break
  *    endif
  *     --> repeat
  *  repeat while (Condition)
  * :Send '#' over UART;
  * stop
  * \enduml
  */

void USART_TransmitString(USART_ID UART_ID, uint8 *string_Copy_U8Data) {
	for (int i = 0; string_Copy_U8Data[i] != '\0'; i++)
		USART_VidSendChar(UART_ID, string_Copy_U8Data[i]);
}

 /***********************************[5]***********************************************
  * Service Name: USART_ReceiveString
  * Service ID[hex]: 0x05
  * Sync/Async: Synchronous
  * Renterancy: Non-Reentrant
  * Parameters (in): UART_ID
  * Parameters (InOut): None
  * Parameters (Out): None
  * Return value: pointer to string
  * Description: Recieve the Sent DataString by the user.
  ************************************************************************************/

 /**
  * \section Service_Name
  * USART5_ReceiveString
  *
  *
  * \section Service_ID_hex
  * 0x05
  *
  * \section Sync_Async
  * Synchronous
  *
  * \section Renterancy
  * Non-Reentrant
  *
 * \section Parameters_in
 *
 * \param[in] UART_ID
 *
 * \param[out] None
 *
 * \param[in-out] None
 *
  * \section Return_value
  * pointer to string
  *
  * \section Description
  *  Recieve the Sent DataString by the user.
  *
  * \section Sequence_diagram
  * \startuml
  *  actor User
  *
  * User -> Code: ReceiveString(UART_ID)
  * activate Code
  *
  * Code -> Code: Initialize i to 0
  * activate Code
  *
  * Code -> UART: Receive character
  * activate UART
  * UART --> Code: Character Received
  *
  * Code -> Code: Increment i
  * Code --> Code: Check if received_string[i] is not null
  * deactivate Code
  *
  * Code -> Code: Check if received_string1[i] is not null
  * activate Code
  * Code -> UART: Choose UART_ID
  * Code -> UART: receive character received_string1[i]
  * activate UART
  * UART --> Code: Character Recieved
  *
  * Code -> UART: Set Last Element in Array '\0'
  * activate UART
  * UART --> Code: Character received
  *
  * deactivate Code
  * \enduml
  * \section Activity_diagram
  *
  * \startuml
  *  start
  *  :Initialize local variables;
  *  switch (Which UART is used?)
  *	case ( A)
  *	:UART1;
  *	case ( B)
  *	:UART2;
  *	case ( C)
  *	:UART3;
  *	endswitch
  *  while (Reach end of the string) is (NO)
  *  :store the received data in the string;
  *  endwhile (YES)
  *  :put null terminator at the end of the string;
  *  :return recievedDataString;
  * end
  * \enduml
  */
uint8* USART_ReceiveString(USART_ID UART_ID) {
	uint8 Copy_U8Data;
	uint8 i = 0;
	received_string[i] = '\0';
	for (i = 0; (Copy_U8Data = USART_ReceiveByte(UART_ID)) != '\0'; i++)
		received_string[i] = Copy_U8Data;
	return (received_string);
}
