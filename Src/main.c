/****************************************************************************
 *****************************  Includes   ***********************************
 *****************************************************************************/
#include <Mcu_Interface.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "LCD_Interface.h"
#include "Port_Interface.h"
#include "DIO_Interface.h"
#include "UART_Interface.h"
#include "queue.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Tasks */
void ManagerTask(void *pvParameter );
void ActionTask(void *pvParameter );
void Led_Update(void *pvParameter );
void LCD_Update(void *pvParameter );

/* Tasks handlers */
TaskHandle_t TaskManager_Handler ;
TaskHandle_t ActionTask_Handler  ;
TaskHandle_t Led_Update_Handler ;
TaskHandle_t LCD_Update_Handler  ;

/* global Queue */
QueueHandle_t GLobalQueue ;

/* Functions */
void LedShape1();
void LedShape2();
void LedShape3();
void LedShape4();
void TTL_ReciveString();

int main(void)
{
	BaseType_t ret1;
	BaseType_t ret2;
	BaseType_t ret3;
	BaseType_t ret4;

	Mcu_Init();
	Mcu_EnumSetPerAPB2(APB2_PER_GPIOA);
	Mcu_EnumSetPerAPB2(APB2_PER_GPIOB);
	Mcu_VidRunnable();

	USART_VidInit(arrOfUART);
	LCD_VidInit();

	ret1 = xTaskCreate(ActionTask,"ActionTask", (configSTACK_DEPTH_TYPE) 100,
			"ActionTask", (UBaseType_t) 1, &ActionTask_Handler);
	ret2 = xTaskCreate(ManagerTask, "ManagerTask", (configSTACK_DEPTH_TYPE) 100,
			"ManagerTask", (UBaseType_t) 2, &TaskManager_Handler);
	ret3 = xTaskCreate(Led_Update,"Led_Update", (configSTACK_DEPTH_TYPE) 100,
			"ActionTask", (UBaseType_t) 0, &Led_Update_Handler);
	ret4 = xTaskCreate(LCD_Update, "LCD_Update", (configSTACK_DEPTH_TYPE) 100,
			"ManagerTask", (UBaseType_t) 0, &LCD_Update_Handler);

	GLobalQueue = xQueueCreate( (UBaseType_t) 3, (UBaseType_t) sizeof(uint8) );

	configASSERT(ret1 == pdPASS);
	configASSERT(ret2 == pdPASS);
	configASSERT(ret3 == pdPASS);
	configASSERT(ret4 == pdPASS);

	vTaskStartScheduler();
    /* Loop forever */

	for(;;);
}

void ManagerTask(void *pvParameter ){
	/* to set first delay */
	vTaskDelay(pdMS_TO_TICKS(1));

	uint8 RecivedMassage ;

	for(;;){
		/* to clear recive massege with each iteration */
		RecivedMassage = 0 ;

		/* To show the first statement */
		USART_TransmitString(UART3,"   Choose option 1-Led update 2-LCD update    ");

		/*  to check on option */
		while( RecivedMassage != '1' && RecivedMassage != '2' ){
		RecivedMassage = USART_ReceiveByte(UART3) ;
		}

		xQueueSend( GLobalQueue,&RecivedMassage, portMAX_DELAY );

		xTaskNotify( ActionTask_Handler , (uint32) 0 , eIncrement );

        /* to out from scheduling */
		vTaskSuspend(NULL);
	}
	vTaskDelete(NULL_PTR);
}

void ActionTask(void *pvParameter ){
	/* to set first delay */
	vTaskDelay(pdMS_TO_TICKS(2));

	uint8 RecivedMassage ;

	for(;;){
		/* to clear recive massege with each iteration */
		RecivedMassage = 0 ;

        // Wait for a notification
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		xQueueReceive(GLobalQueue, &RecivedMassage,portMAX_DELAY );

		/* to see option on putty */
		USART_TransmitString(UART3,&RecivedMassage);

		/* to notify other tasks */
        switch ( RecivedMassage){

        case '1' :
    		xTaskNotify( Led_Update_Handler , (uint32) 0, eIncrement );
        	break ;
        case '2' :
    		xTaskNotify( LCD_Update_Handler , (uint32) 0, eIncrement );
        	break ;

                }

        /* to out from scheduling */
		vTaskSuspend(NULL);

	}
	vTaskDelete(NULL_PTR);
}

void Led_Update(void *pvParameter ){
	/* to set first delay */
	vTaskDelay(pdMS_TO_TICKS(3));

	uint8 RecivedMassage ;

	/* Intilaize four leds */
	Port_EnumSetterPin(Port_A9, PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(Port_A10, PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(Port_A11, PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(Port_A12, PORT_PIN_OUT|G_PUSH_PULL);
	Port_VidRunnable();

	for(;;){
		/* to clear recive massege with each iteration */
		RecivedMassage = 0 ;

        // Wait for a notification
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		USART_TransmitString(UART3,"   Wanted shape (1:4) = ");

		/*  to check on option */
		while (RecivedMassage != '1' && RecivedMassage != '2'
				&& RecivedMassage != '3' && RecivedMassage != '4') {
		RecivedMassage = USART_ReceiveByte(UART3) ;
		}

		/* to see option on putty */
		USART_TransmitString(UART3,&RecivedMassage);

		/* to notify other tasks */
        switch ( RecivedMassage){
        case '1' :
        	LedShape1();
        	break ;
        case '2' :
        	LedShape2();
        	break ;
        case '3' :
        	//LedShape3();
        	break ;
        case '4' :
        	//LedShape4();
        	break ;
                }
	}
	vTaskDelete(NULL_PTR);
}

void LCD_Update(void *pvParameter ){
	/* to set first delay */
	vTaskDelay(pdMS_TO_TICKS(3));

	for(;;){
        // Wait for a notification
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		TTL_ReciveString();

		vTaskResume(TaskManager_Handler);
		vTaskResume(ActionTask_Handler);
		vTaskSuspend(NULL_PTR);
	}

	vTaskDelete(NULL_PTR);
}

void LedShape1(){
	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_EnumSetterPin(Dio_A9, STD_ON);
		Dio_EnumSetterPin(Dio_A10, STD_OFF);
		Dio_EnumSetterPin(Dio_A11, STD_OFF);
		Dio_EnumSetterPin(Dio_A12, STD_ON);
		Dio_VidRunnable();
		vTaskDelay(pdMS_TO_TICKS(250));
		Dio_EnumSetterPin(Dio_A9, STD_OFF);
		Dio_EnumSetterPin(Dio_A10, STD_ON);
		Dio_EnumSetterPin(Dio_A11, STD_ON);
		Dio_EnumSetterPin(Dio_A12, STD_OFF);
		Dio_VidRunnable();
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(TaskManager_Handler);
	vTaskResume(ActionTask_Handler);
}
void LedShape2(){
	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_EnumSetterPin(Dio_A9, STD_ON);
		Dio_EnumSetterPin(Dio_A10, STD_OFF);
		Dio_EnumSetterPin(Dio_A11, STD_ON);
		Dio_EnumSetterPin(Dio_A12, STD_OFF);
		Dio_VidRunnable();
		vTaskDelay(pdMS_TO_TICKS(250));
		Dio_EnumSetterPin(Dio_A9, STD_OFF);
		Dio_EnumSetterPin(Dio_A10, STD_ON);
		Dio_EnumSetterPin(Dio_A11, STD_OFF);
		Dio_EnumSetterPin(Dio_A12, STD_ON);
		Dio_VidRunnable();
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(TaskManager_Handler);
	vTaskResume(ActionTask_Handler);
}
/*
void LedShape3(){
	Dio_EnumSetterPin(Dio_A9, STD_ON);
	Dio_EnumSetterPin(Dio_A10, STD_ON);
	Dio_EnumSetterPin(Dio_A11, STD_OFF);
	Dio_EnumSetterPin(Dio_A12, STD_OFF);
	Dio_VidRunnable();
	vTaskDelay(pdMS_TO_TICKS(250));

	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_FlipChannel(Dio_A9);
		Dio_FlipChannel(Dio_A10);
		Dio_FlipChannel(Dio_A11);
		Dio_FlipChannel(Dio_A12);
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(TaskManager_Handler);
	vTaskResume(ActionTask_Handler);
}
void LedShape4(){
	Dio_WriteChannel(Dio_A9, STD_OFF);
	Dio_WriteChannel(Dio_A10, STD_OFF);
	Dio_WriteChannel(Dio_A11, STD_ON);
	Dio_WriteChannel(Dio_A12, STD_ON);
	vTaskDelay(pdMS_TO_TICKS(250));

	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_FlipChannel(Dio_A9);
		Dio_FlipChannel(Dio_A10);
		Dio_FlipChannel(Dio_A11);
		Dio_FlipChannel(Dio_A12);
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(TaskManager_Handler);
	vTaskResume(ActionTask_Handler);
}*/

void TTL_ReciveString(){
	/* declare buffer */
	uint8 RecivedMassage ;
	USART_TransmitString(UART3,"     ");

	/*  to recive string from putty using TTL module */
	while ( RecivedMassage != '#') {

		RecivedMassage = USART_ReceiveByte(UART3) ;

		if ((RecivedMassage >= 'A' && RecivedMassage <= 'Z')
			|| (RecivedMassage >= 'a' && RecivedMassage <= 'z') ){

			/* to transmit data */
			USART_TransmitString(UART3,&RecivedMassage);
			LCD_VidSendChar(RecivedMassage);
		}//end if
	}//end while
}
