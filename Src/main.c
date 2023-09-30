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
void Init_Task   (void *pvParameter);
void ManagerTask (void *pvParameter);
void ActionTask  (void *pvParameter);
void LedUpdate  (void *pvParameter);
void LcdUpdate  (void *pvParameter);

/* Tasks handlers */
TaskHandle_t Init_Task_Handler   ;
TaskHandle_t ManagerTask_Handler ;
TaskHandle_t ActionTask_Handler  ;
TaskHandle_t LedUpdate_Handler  ;
TaskHandle_t LcdUpdate_Handler  ;

/* global Queue */
QueueHandle_t GLobalQueue ;

/* Functions */
void LedShape1();
void LedShape2();
void LedShape3();
void LedShape4();
void Runnable(const uint8 value);
void TTL_ReciveString();

int main(void)
{
	BaseType_t ret1;
	BaseType_t ret2;
	BaseType_t ret3;
	BaseType_t ret4;
	BaseType_t ret5;

	ret1 = xTaskCreate(Init_Task, "Init_Task", (configSTACK_DEPTH_TYPE) 200,
			"Init_Task", (UBaseType_t) 3, &Init_Task_Handler);
	ret2 = xTaskCreate(ManagerTask, "ManagerTask", (configSTACK_DEPTH_TYPE) 200,
			"ManagerTask", (UBaseType_t) 2, &ManagerTask_Handler);
	ret3 = xTaskCreate(ActionTask,"ActionTask", (configSTACK_DEPTH_TYPE) 200,
			"ActionTask", (UBaseType_t) 1, &ActionTask_Handler);
	ret4 = xTaskCreate(LedUpdate,"Led_Update", (configSTACK_DEPTH_TYPE) 100,
			"Led_Update", (UBaseType_t) 0, &LedUpdate_Handler);
	ret5 = xTaskCreate(LcdUpdate, "LCD_Update", (configSTACK_DEPTH_TYPE) 200,
			"LCD_Update", (UBaseType_t) 0, &LcdUpdate_Handler);

	GLobalQueue = xQueueCreate( (UBaseType_t) 3, (UBaseType_t) sizeof(uint8) );

	configASSERT(ret1 == pdPASS);
	configASSERT(ret2 == pdPASS);
	configASSERT(ret3 == pdPASS);
	configASSERT(ret4 == pdPASS);
	configASSERT(ret5 == pdPASS);

	vTaskStartScheduler();
    /* Loop forever */

	for(;;);
}

void Init_Task(void *pvParameter) {
    // Perform MCU and peripheral initialization
    Mcu_Init();
    Mcu_EnumSetPerAPB2(APB2_PER_GPIOA);
    Mcu_EnumSetPerAPB2(APB2_PER_GPIOB);
    Mcu_EnumSetPerAPB1(APB1_PER_USART3);
	Runnable(0x00);

    // Perform other initializations
	Port_VidInit();
    Dio_VidInit();
    USART_VidInit();
    LCD_VidInit();

	/* Intilaize four leds */
	Port_EnumSetterPin(Port_A9, PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(Port_A10, PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(Port_A11, PORT_PIN_OUT|G_PUSH_PULL);
	Port_EnumSetterPin(Port_A12, PORT_PIN_OUT|G_PUSH_PULL);
	Runnable(0x01);

    // Delete the Init_Task, as its job is done
    vTaskDelete(NULL_PTR);
}

void Runnable(const uint8 value){
	switch (value) {
	case 0x00:
		Mcu_VidRunnable();
		break;
	case 0x01:
		Port_VidRunnable();
		break;
	case 0x02:
		Dio_VidRunnable();
		break;
	case 0x03:
		Mcu_VidRunnable();
		Port_VidRunnable();
		Dio_VidRunnable();
		break;
	}
}

void vApplicationIdleHook(void){
	Runnable(0x03);
}

void ManagerTask(void *pvParameter ){
	uint8 RecivedMassage ;

	for(;;){
		/* to clear recive massege with each iteration */
		RecivedMassage = 0 ;

		/* To show the first statement */
		USART_TransmitString(UART3,"   Choose option 1-Led update 2-LCD update:");

		/*  to check on option */
		while( RecivedMassage != '1' && RecivedMassage != '2' ){
		RecivedMassage = USART_ReceiveByte(UART3) ;
		}

		xQueueSend( GLobalQueue,&RecivedMassage, portMAX_DELAY );

		xTaskNotify( ActionTask_Handler , (uint32) 0 , eIncrement );

        /* to out from scheduling */
		vTaskSuspend(NULL_PTR);
	}
	vTaskDelete(NULL_PTR);
}

void ActionTask(void *pvParameter ){
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
    		xTaskNotify( LedUpdate_Handler , (uint32) 0, eIncrement );
        	break ;
        case '2' :
    		xTaskNotify( LcdUpdate_Handler , (uint32) 0, eIncrement );
        	break ;
                }
        /* to out from scheduling */
		vTaskSuspend(NULL);

	}
	vTaskDelete(NULL_PTR);
}

void LedUpdate(void *pvParameter ){
	uint8 RecivedMassage ;
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
        	LedShape3();
        	break ;
        case '4' :
        	LedShape4();
        	break ;
                }
	}
	vTaskDelete(NULL_PTR);
}

void LcdUpdate(void *pvParameter ){
	for(;;){
        // Wait for a notification
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		TTL_ReciveString();

		vTaskResume(ManagerTask_Handler);
		vTaskResume(ActionTask_Handler);
	}
	vTaskDelete(NULL_PTR);
}

void LedShape1(){
	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_EnumSetterPin(Dio_A9, STD_ON);
		Dio_EnumSetterPin(Dio_A10, STD_OFF);
		Dio_EnumSetterPin(Dio_A11, STD_OFF);
		Dio_EnumSetterPin(Dio_A12, STD_ON);
		Runnable(0x02);
		vTaskDelay(pdMS_TO_TICKS(250));
		Dio_EnumSetterPin(Dio_A9, STD_OFF);
		Dio_EnumSetterPin(Dio_A10, STD_ON);
		Dio_EnumSetterPin(Dio_A11, STD_ON);
		Dio_EnumSetterPin(Dio_A12, STD_OFF);
		Runnable(0x02);
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}
void LedShape2(){
	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_EnumSetterPin(Dio_A9, STD_ON);
		Dio_EnumSetterPin(Dio_A10, STD_OFF);
		Dio_EnumSetterPin(Dio_A11, STD_ON);
		Dio_EnumSetterPin(Dio_A12, STD_OFF);
		Runnable(0x02);
		vTaskDelay(pdMS_TO_TICKS(250));
		Dio_EnumSetterPin(Dio_A9, STD_OFF);
		Dio_EnumSetterPin(Dio_A10, STD_ON);
		Dio_EnumSetterPin(Dio_A11, STD_OFF);
		Dio_EnumSetterPin(Dio_A12, STD_ON);
		Runnable(0x02);
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}
void LedShape3(){
	Dio_EnumSetterPin(Dio_A9, STD_ON);
	Dio_EnumSetterPin(Dio_A10, STD_ON);
	Dio_EnumSetterPin(Dio_A11, STD_OFF);
	Dio_EnumSetterPin(Dio_A12, STD_OFF);
	Runnable(0x02);
	vTaskDelay(pdMS_TO_TICKS(2500));
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}
void LedShape4(){
	Dio_EnumSetterPin(Dio_A9, STD_OFF);
	Dio_EnumSetterPin(Dio_A10, STD_OFF);
	Dio_EnumSetterPin(Dio_A11, STD_ON);
	Dio_EnumSetterPin(Dio_A12, STD_ON);
	Runnable(0x02);
	vTaskDelay(pdMS_TO_TICKS(2500));
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}

void TTL_ReciveString(){
	/* declare buffer */
	uint8 RecivedMassage ;
	LCD_VidClear();
	USART_TransmitString(UART3,"     ");

	/*  to recive string from putty using TTL module */
	while (RecivedMassage != '#') {
		RecivedMassage = USART_ReceiveByte(UART3);
		if ((RecivedMassage >= 'A' && RecivedMassage <= 'Z')
				|| (RecivedMassage >= 'a' && RecivedMassage <= 'z')) {
			/* to transmit data */
			USART_TransmitString(UART3, &RecivedMassage);
			LCD_VidSendChar(RecivedMassage);
		} //end if
	} //end while
}
