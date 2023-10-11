/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Shehab Mohammed,
 * 					 Ali Mamdouh
 * @brief          : RTOS body
 ******************************************************************************
 *
 ******************************************************************************
 */




/****************************************************************************
 *****************************  Includes   **********************************
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


/****************************************************************************
 ******************************  Defenitions   ******************************
 *****************************************************************************/
#define MCU_RUNNABLE  0x00
#define PORT_RUNNABLE 0x01
#define DIO_RUNNABLE  0x02
#define ALL_RUNNABLE  0x03

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


/**
 * \section Service_Name
 * Main
 *
 * \section Description
 * Main function which is responsible for creating tasks.
 * 
 * \section Req_ID
 * Main_0x00
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] None
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * Sheduling and creation of tasks.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create five variables for return status of main tasks;
 *  :Create Init_Task with priority 3 and Stack = 200 and store its status in ret1;
 *  :Create ManagerTask with priority 2 and Stack = 200 and store its status in ret2; 
 *  :Create ActionTask with priority 1 and Stack = 200 and store its status in ret3; 
 *  :Create LED_UpdateTaskwith priority 0 and Stack = 100 and store its status in ret4; 
 *  :Create LCD_UpdateTask with priority 0 and Stack = 200 and store its status in ret5;  
 *  :Create Queue with size 3 bytes; 
 *  :Make sure that each task is created succefuly from ret variables;
 *  #green:Start Task Scheduler;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == RTOS functions sequence == 
 *
 * "Main\nFunction" -> "xTaskCreate\nFunction":Init_Task(address of function),"Init_Task"(function name), (configSTACK_DEPTH_TYPE)200, "Init_Task"(parameter input), 3(priority), &Init_Task_Handler)
 * "xTaskCreate\nFunction" -[#blue]-> "Main\nFunction": Return its status(if it is created successfully or not) .
 * 
 * "Main\nFunction" -> "xTaskCreate\nFunction":ManagerTask(address of function),"ManagerTask"(function name), (configSTACK_DEPTH_TYPE)200, "ManagerTask"(parameter input), 2(priority), &TaskManager_Handler)
 * "xTaskCreate\nFunction" -[#blue]-> "Main\nFunction": Return its status(if it is created successfully or not) .
 *
 * "Main\nFunction" -> "xTaskCreate\nFunction":ActionTask(address of function),"ActionTask"(function name), (configSTACK_DEPTH_TYPE)200, "ActionTask"(parameter input), 1(priority), &ActionTask_Handler)
 * "xTaskCreate\nFunction" -[#blue]-> "Main\nFunction": Return its status(if it is created successfully or not) .
 *
 *
 * "Main\nFunction" -> "xTaskCreate\nFunction":Led_Update(address of function),"Led_Update"(function name), (configSTACK_DEPTH_TYPE)100, "Led_Update"(parameter input), 0(priority), &Led_Update_Handler)
 * "xTaskCreate\nFunction" -[#blue]-> "Main\nFunction": Return its status(if it is created successfully or not) .
 *
 * "Main\nFunction" -> "xTaskCreate\nFunction":LCD_Update(address of function),"LCD_Update"(function name), (configSTACK_DEPTH_TYPE)200, "LCD_Update"(parameter input), 0(priority), &LCD_Update_Handler)
 * "xTaskCreate\nFunction" -[#blue]-> "Main\nFunction": Return its status(if it is created successfully or not).
 *
 * "Main\nFunction" -> "xQueueCreate\nFunction":(UBaseType_t) 3(three variables in the queue), (UBaseType_t) sizeof(uint8) (Byte is the size of each variable in the queue)
 * "xQueueCreate\nFunction" -[#blue]-> "Main\nFunction": Return its handler(its handler type is QueueHandle_t).
 *
 * "Main\nFunction" -> "vTaskStartScheduler\nFunction":
 * note right #green: Start Schedulering of tasks.
 *
 * @enduml
 *
 * \section State_Diagram
 *
 * @startuml
 *
 * [*] --> Init_Task
 * Init_Task #aqua -> [*] : **Init Task deletes its self**
 * Init_Task : **The task is responsible for initialization of peripherals**
 * Init_Task --> ManagerTask :  **ManagerTask comes after InitTask finish initialization**
 *
 * ManagerTask #aqua --> ActionTask : **ManagerTask Notify Action task**
 * ManagerTask --> Queue #green : **ManagerTask sends option message to Queue** 
 * ManagerTask : **Display options, receives option message and based on it Notify Action task with dedicated mode.** 
 * ManagerTask : **ManagerTask suspend its self after it Notify Action task.**
 *
 * Queue --> ActionTask #aqua : **Queue sends option message to ActionTask** 
 * 
 * ActionTask : **Waits for option message from the Queue, prints it to the UART3 port,**  
 * ActionTask : **and sends a notification to the appropriate task based on the user's selection, Then it suspend its self.**  
 * ActionTask ---> LED_UpdateTask #aqua : **ActionTask notify LED_UpdateTask if option message = '1'** 
 * ActionTask ---> LCD_UpdateTask #aqua : **ActionTask notify LCD_UpdateTask if option message = '2'**  
 *
 * LCD_UpdateTask : **The LCD_UpdateTask waits for a notification, then receives a string from UART3 and displays it on the LCD.**  
 * LCD_UpdateTask : **It resumes Action and Manager Task then Suspend its self.** 
 * LCD_UpdateTask --> ActionTask : **Resume Action task after LCD_UpdateTask finish**  
 * LCD_UpdateTask -> ManagerTask : **Resume Manager task after LCD_UpdateTask finish**   
 *
 * LED_UpdateTask : **This task controls four LEDs based on option message received from the user through UART for a desired LED shape.**  
 * LED_UpdateTask : **It resumes Action and Manager Task then Suspend its self.** 
 * LED_UpdateTask --> ActionTask : **Resume Action task after LED_UpdateTask finish**  
 * LED_UpdateTask -> ManagerTask : **Resume Manager task after LED_UpdateTask finish**   
 *
 * @enduml
 *
*/
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

/**
 * \section Service_Name
 * Init_Task
 *
 * \section Description
 * Init_Task which is responsible for initialization.
 * 
 * \section Req_ID
 * Init_0x01
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * initialization of Peripherals.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create four variables for reurn status of main tasks; 
 *  :Enabling RCC for dedicated peripherals; 
 *  :Intilaize GPIO, USART, LCD;
 *  :Intilaize Four Leds; 
 *  #orange:Delete the Task;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == RCC mode Setter, Runnable and Initialization functions sequence  == 
 *
 * "**Main**\n**Function**" -> "**Mcu_Init**\n**Function**":
 * note right #green: Initialize the clock of the system
 *
 * "**Main**\n**Function**" -> "**Mcu_EnumSetPerAPB2**\n**Function**": **APB2_PER_GPIOA**
 * note right #palegreen: Select GPIOA peripheral to be enabled when runnable comes.
 *
 * "**Main**\n**Function**" -> "**Mcu_EnumSetPerAPB2**\n**Function**": <b>APB2_PER_GPIOB</b>
 * note right #palegreen: Select GPIOB peripheral to be enabled when runnable comes.
 * 
 * "**Main**\n**Function**" -> "**Runnable**\n**Function**": **MCU_RUNNABLE(0x00)**
 * note right #green: Enable the peripherals selected by the setter functions.
 *
 * == Peripherals initialization functions sequence == 
 *
 * "**Main**\n**Function**" -> "**Port_VidInit**\n**Function**":
 * note right #blue: Initialize PORT deriver.
 *
 * "**Main**\n**Function**" -> "**Dio_VidInit**\n**Function**":
 * note right #blue: Initialize DIO deriver.
 *
 * "**Main**\n**Function**" -> "**USART_VidInit**\n**Function**": **arrOfUART**
 * note right #blue: Initialize UART with modes selected in arrOfUART.
 *
 * "**Main**\n**Function**" -> "**LCD_VidInit**\n**Function**":
 * note right #blue: Initialize LCD.
 *
 * == Leds' pins Mode Setter and Runnable functions sequence == 
 *
 * "**Main**\n**Function**" -> "**Port_EnumSetterPin**\n**Function**": **Port_A9, PORT_PIN_OUT|G_PUSH_PULL**
 * note right #aqua: Set PinA9 to be OUTPUT PUSH_PULL mode When runnable comes.
 *
 * "**Main**\n**Function**" -> "**Port_EnumSetterPin**\n**Function**": **Port_A10, PORT_PIN_OUT|G_PUSH_PULL**
 * note right #aqua: Set PinA10 to be OUTPUT PUSH_PULL mode When runnable comes.
 *
 * "**Main**\n**Function**" -> "**Port_EnumSetterPin**\n**Function**": **Port_A11, PORT_PIN_OUT|G_PUSH_PULL**
 * note right #aqua: Set PinA11 to be OUTPUT PUSH_PULL mode When runnable comes.
 *
 * "**Main**\n**Function**" -> "**Port_EnumSetterPin**\n**Function**": **Port_A12, PORT_PIN_OUT|G_PUSH_PULL**
 * note right #aqua: Set PinA12 to be OUTPUT PUSH_PULL mode When runnable comes.
 *
 * "**Main**\n**Function**" -> "**Runnable**\n**Function**": **PORT_RUNNABLE(0x01)**
 * note right #blue: Apply modes on the four leds' pins.
 *
 * == RTOS functions sequence == 
 * "**Main**\n**Function**" -> "**vTaskDelete**\n**Function**": **NULL_PTR**
 * note right #red: Delete the Init_Task, as its job is done
 *
 * @enduml
 *
*/
void Init_Task(void *pvParameter) {
    // Perform MCU and peripheral initialization
    Mcu_Init();
    Mcu_EnumSetPerAPB2(APB2_PER_GPIOA);
    Mcu_EnumSetPerAPB2(APB2_PER_GPIOB);
    Mcu_EnumSetPerAPB1(APB1_PER_USART3);
	Runnable(MCU_RUNNABLE);

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
	Runnable(PORT_RUNNABLE);

    // Delete the Init_Task, as its job is done
    vTaskDelete(NULL_PTR);
}


/**
 * \section Service_Name
 * Runnable
 *
 * \section Description
 * Runnable which is responsible for applying setter and getter requests.
 * 
 * \section Req_ID
 * Runnable_0x02
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] const uint8 value: value which contain the choice which Runnable to call, it could have these four values: 
 *                              @MCU_RUNNABLE(0x00)
 *                              @PORT_RUNNABLE(0x01)
 *                              @DIO_RUNNABLE(0x02)
 *                              @ALL_RUNNABLE(0x03)
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * Ruunable of Peripherals.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 * start
 * switch (value?)
 * case ( MCU_RUNNABLE(0x00) )
 * #green:Call MCU Runnable function;
 * case ( PORT_RUNNABLE(0x01) )
 * #green:Call Port Runnable function;
 * case ( DIO_RUNNABLE(0x02) )
 * #green:Call Dio Runnable function;
 * case ( All_RUNNABLE(0x03) )
 * #green:Call All Runnable functions;
 * endswitch
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == Functions sequence(Case MCU_RUNNABLE)  == 
 *
 * "Runnable\nFunction" -> "Mcu_VidRunnable\nFunction":
 * note right #blue: Enable dedicated peripherals that are choosen from setter functions.
 *
 * == Functions sequence(Case PORT_RUNNABLE)  == 
 *
 * "Runnable\nFunction" -> "Port_VidRunnable\nFunction":
 * note right #blue: Apply Modes on the four leds' pins that are choosen from setter functions.
 *
 * == Functions sequence(Case DIO_RUNNABLE)  == 
 *
 * "Runnable\nFunction" -> "Dio_VidRunnable\nFunction":
 * note right #blue: Apply Action on the four leds' pins that are choosen from setter functions.
 *
 * == functions sequence(Case ALL_RUNNABLE)  == 
 *
 * "Runnable\nFunction" -> "Mcu_VidRunnable\nFunction":
 * note right #blue: Enable dedicated peripherals that are choosen from setter functions.
 *
 * "Runnable\nFunction" -> "Port_VidRunnable\nFunction":
 * note right #blue: Apply Modes on the four leds' pins that are choosen from setter functions.
 *
 * "Runnable\nFunction" -> "Dio_VidRunnable\nFunction":
 * note right #blue: Apply Action on the four leds' pins that are choosen from setter functions.
 *
 *
 * @enduml
 *
*/
void Runnable(const uint8 value){
	switch (value) {
	case MCU_RUNNABLE:
		Mcu_VidRunnable();
		break;
	case PORT_RUNNABLE:
		Port_VidRunnable();
		break;
	case DIO_RUNNABLE:
		Dio_VidRunnable();
		break;
	case ALL_RUNNABLE:
		Mcu_VidRunnable();
		Port_VidRunnable();
		Dio_VidRunnable();
		break;
	}
}



/**
 * \section Service_Name
 * vApplicationIdleHook
 *
 * \section Description
 * Hook function that comes when there is no task and call all the runnables.
 * 
 * \section Req_ID
 * vApplicationIdleHook_0x03
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] None
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * To call all runnables when there is no task ready or running.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 * start
 * #green:Call All Runnable functions;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * "vApplicationIdleHook\nFunction" -> "Runnable\nFunction": ALL_RUNNABLE
 * note right #blue: Call all runnables.
 *
 * @enduml
 *
*/
void vApplicationIdleHook(void){
	Runnable(ALL_RUNNABLE);
}


/**
 * \section Service_Name
 * ManagerTask
 *
 * \section Description
 *  Display options, receives messages and based on it Notify Action task with dedicated mode.
 * 
 * \section Req_ID
 * ManagerTask_0x04
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * The main task that handles receiving and processing user input.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create RecivedMassage variable;
 *  :Clear RecivedMassage variable each iteration; 
 *  :Show options by sending this string through UART3 "1-Led update 2-LCD update"; 
 *  while (RecivedMassage !=('1') and !=('2') ?) is (yes)
 *  :Receive data from UART3 and store it in RecivedMassage;
 *  endwhile (No)
 *  :Send RecivedMassage to GLobalQueue;
 *  :Notify ActionTask;
 *  #green:MangerTask suspend its self;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == UART functions sequence (inside loop) ==
 *
 * "ManagerTask\nTask" -[#blue]> "USART_TransmitString\nFunction": Select UART peripheral(UART3) and send String(Message options) through it
 * note right #aqua: Displaying:\n"Choose option 1-Led update 2-LCD update"
 *
 * "ManagerTask\nTask" -> "USART_ReceiveByte\nFunction": UART peripheral(UART3) Request 
 * "USART_ReceiveByte\nFunction" -[#blue]-> "ManagerTask\nTask": Return Received byte from UART peripheral(UART3)
 *
 * == RTOS functions sequence (inside loop) == 
 * "ManagerTask\nTask" -> "xQueueSend\nFunction": Queue name(GLobalQueue),RecivedMassage and max time task wait if Queue is full(PortMAX_DELAY) 
 * "xQueueSend\nFunction" -[#red]-> "ManagerTask\nTask": Block Task with the requested time if Queue is full(PortMAX_DELAY)
 *
 * "ManagerTask\nTask" -> "xTaskNotify\nFunction": Task_Handler(ActionTask),value of the notified task(0) and Type of action on that value(eIncrement)
 * "xTaskNotify\nFunction" -[#blue]-> "ActionTask\nTask": Notify Action Task and update its value
 *
 * "ManagerTask\nTask" -> "vTaskSuspend\nFunction": NULL
 * "vTaskSuspend\nFunction" -[#red]-> "ManagerTask\nTask": Suspend ManagerTask
 *
 * == Break from loop functions sequence ==
 * "ManagerTask\nTask" -> "vTaskDelete\nFunction": NULL_PTR
 * "vTaskDelete\nFunction" -[#red]-> "ManagerTask\nTask": Delete ManagerTask 
 * @enduml
 *
*/
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


/**
 * \section Service_Name
 * ActionTask
 *
 * \section Description
 *  Waits for a message from the ManagerTask task, prints it to the UART3 port, and sends a notification to the appropriate task based on the user's selection.
 * 
 * \section Req_ID
 * ActionTask_0x05
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * Decouples the message handling logic from the task that performs the actual action, making the code more modular and reusable.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create RecivedMassage variable;
 *  :Clear RecivedMassage variable each iteration; 
 *  :Wait for notification; 
 *  :Receive message from global Queue; 
 *  :Transmit RecivedMassage through UART to see it on puty; 
 *  if (RecivedMassage?) then (Equal '1')
 *  :Notify LedUpdateTask;
 *  else (Equal '2')
 *  :Notify LcdUpdateTask;
 * endif
 *  #green:ActionTask suspend its self;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == RTOS functions sequence (inside loop) == 
 *
 * "ActionTask\nTask" -> "ulTaskNotifyTake\nFunction": pdTRUE, portMAX_DELAY
 * "ulTaskNotifyTake\nFunction" -[#blue]-> "ActionTask\nTask": Clear Notify value of Action Task, block ActionTask indifinite time until notification comes
 *
 * "ActionTask\nTask" -> "xQueueReceive\nFunction": GLobalQueue, address of RecivedMassage, portMAX_DELAY 
 * "xQueueReceive\nFunction" -[#blue]-> "ActionTask\nTask": Receive from queue message and store in RecivedMassage, Block Task indifinite time if queue is empty
 *
 * == UART functions sequence (inside loop) ==
 *
 * "ActionTask\nTask" -[#blue]> "USART_TransmitString\nFunction": Select UART peripheral(UART3), send String through it
 * note right #aqua: Displaying:\nRecivedMassage
 *
 * == RTOS functions sequence case RecivedMassage = '1' (inside loop) == 
 *
 * "ActionTask\nTask" -> "xTaskNotify\nFunction": Led_Update_Handler, value of the notified task(0), Type of action on that value(eIncrement)
 * "xTaskNotify\nFunction" -[#blue]-> "Led_Update\nTask": Notify Led_Update Task and update its value
 *
 *
 * == RTOS functions sequence case RecivedMassage = '2' (inside loop) == 
 * "ActionTask\nTask" -> "xTaskNotify\nFunction": LCD_Update_Handler, value of the notified task(0), Type of action on that value(eIncrement)
 * "xTaskNotify\nFunction" -[#blue]-> "LCD_Update\nTask": Notify LCD_Update Task and update its value
 *
 * == RTOS functions sequence (inside loop) == 
 * "ActionTask\nTask" -> "vTaskSuspend\nFunction": NULL
 * "vTaskSuspend\nFunction" -[#red]-> "ActionTask\nTask": Suspend ActionTask
 *
 * == Break from loop functions sequence ==
 * "ActionTask\nTask" -> "vTaskDelete\nFunction": NULL_PTR
 * "vTaskDelete\nFunction" -[#red]-> "ActionTask\nTask": Delete ActionTask
 * @enduml
 *
*/
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




/**
 * \section Service_Name
 * Led_UpdateTask
 *
 * \section Description
 *  This task controls four LEDs based on a message received from a UART. It waits for a notification from another task, then asks the user for a desired LED shape.
 * 
 * \section Req_ID
 * Led_UpdateTask_0x06
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * The task Led_Update allows the LEDs to be controlled from an external source.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create RecivedMassage variable**(before task loop)**;
 *  :Clear RecivedMassage variable **each iteration**;
 *  :Wait for notification;  
 *  :Show options by sending this string through UART3 **"Wanted shape (1:4) ="**; 
 *  while (RecivedMassage **!=('1')& !=('2')& !=('3')& !=('4')** ?) is (yes)
 *  :Receive data from UART3 and store it in RecivedMassage;
 *  endwhile (No)
 *  :Display RecivedMassage on puty through UART3;	 
 * switch (RecivedMassage?)
 * case ( '1' )
 *   #green:Calls LedShape1 fucntion;
 * case ( '2' ) 
 *   #green:Calls LedShape2 fucntion;
 * case ( '3' )
 *   #green:Calls LedShape3 fucntion;
 * case ( '4' )
 *   #green:Calls LedShape4 fucntion;
 * endswitch
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == RTOS functions sequence (inside loop) == 
 * "Led_UpdateTask\nTask" -> "ulTaskNotifyTake\nFunction": pdTRUE, portMAX_DELAY
 * "ulTaskNotifyTake\nFunction" -[#blue]-> "Led_UpdateTask\nTask": Clear Notify value of Led_UpdateTask Task, block Led_UpdateTaskTask indifinite time until notification comes
 *
 * == UART functions sequence (inside loop) ==
 *
 * "Led_UpdateTask\nTask" -[#blue]> "USART_TransmitString\nFunction": Select UART peripheral(UART3) and send String through it
 * note right #blue: Displaying:\n"Wanted shape (1:4)"
 *
 *
 * "Led_UpdateTask\nTask" -> "USART_ReceiveByte\nFunction": UART peripheral(UART3) Request 
 * "USART_ReceiveByte\nFunction" -[#blue]-> "Led_UpdateTask\nTask": Return Received byte from UART peripheral(UART3)
 *
 * "Led_UpdateTask\nTask" -[#blue]> "USART_TransmitString\nFunction": Select UART peripheral(UART3) and send String through it
 * note right #aqua: Displaying:\nRecivedMassage
 *
 * == Functions sequence case RecivedMassage = '1' (inside loop) == 
 *
 * "Led_UpdateTask\nTask" -[#blue]> "LedShape1\nFunction":
 * note right #green: Turns on and off the four LEDs in a specific pattern1,\nwith a delay of 250 milliseconds between each change.
 *
 * == Functions sequence case RecivedMassage = '2' (inside loop) == 
 *
 * "Led_UpdateTask\nTask" -[#blue]> "LedShape2\nFunction":
 * note right #green: Turns on and off the four LEDs in a specific pattern2,\nwith a delay of 250 milliseconds between each change.
 *
 * == Functions sequence case RecivedMassage = '3' (inside loop) == 
 *
 * "Led_UpdateTask\nTask" -[#blue]> "LedShape3\nFunction":
 * note right #green: Turns on and off the four LEDs in a specific pattern3,\nwith a delay of 250 milliseconds between each change.
 *
 * == Functions sequence case RecivedMassage = '4' (inside loop) ==  
 *
 * "Led_UpdateTask\nTask" -[#blue]> "LedShape4\nFunction":
 * note right #green: Turns on and off the four LEDs in a specific pattern4,\nwith a delay of 250 milliseconds between each change.
 *
 * == Break from loop functions sequence ==
 * "Led_UpdateTask\nTask" -> "vTaskDelete\nFunction": NULL_PTR
 * "vTaskDelete\nFunction" -[#red]-> "Led_UpdateTask\nTask": Delete Led_UpdateTask 
 * @enduml
 *
*/
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




/**
 * \section Service_Name
 * LCD_UpdateTask
 *
 * \section Description
 * The LCD_UpdateTask waits for a notification, then receives a string from UART3 and displays it on the LCD.It resumes Action and Manager Task then Suspend its self.
 * 
 * \section Req_ID
 * LCD_UpdateTask_0x07
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * Allows the LCD display to be updated from UART.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Wait for notification; 
 *  :Call TTL_ReciveString function(To Update LCD using UART);  
 *  :Resume ManagerTask; 
 *  :Resume ActionTask; 
 *  #green:LCD_UpdateTask suspend its self;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == RTOS functions sequence (inside Task loop) == 
 *
 * "LCD_UpdateTask\nTask" -> "ulTaskNotifyTake\nFunction": pdTRUE, portMAX_DELAY
 * "ulTaskNotifyTake\nFunction" -[#blue]-> "LCD_UpdateTask\nTask": Clear Notify value of LCD_Update Task, block LCD_UpdateTask indifinite time until notification comes
 *
 * == Functions sequence (inside Task loop) ==
 *
 * "LCD_UpdateTask\nTask" -[#blue]> "TTL_ReciveString\nFunction":
 * note right #aqua: This function receives a string from the UART\nand displays it on the LCD until the # character is received.
 *
 * == RTOS functions sequence (inside Task loop) == 
 *
 * "LCD_UpdateTask\nTask" -[#blue]> "vTaskResume\nFunction": TaskManager_Handler
 * note right #green: Resume ManagerTask
 *
 * "LCD_UpdateTask\nTask" -[#blue]> "vTaskResume\nFunction": ActionTask_Handler
 * note right #green: Resume ActionTask
 *
 * "LCD_UpdateTask\nTask" -> "vTaskSuspend\nFunction": NULL
 * "vTaskSuspend\nFunction" -[#red]-> "LCD_UpdateTask\nTask": Suspend LCD_UpdateTask
 *
 * ==  functions sequence(after Task loop) ==
 * "LCD_UpdateTask\nTask" -> "vTaskDelete\nFunction": NULL_PTR
 * "vTaskDelete\nFunction" -[#red]-> "LCD_UpdateTask\nTask": Delete LCD_UpdateTask
 * @enduml
 *
*/
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




/**
 * \section Service_Name
 * LedShape1Function
 *
 * \section Description
 * Turns on and off the four LEDs in a specific pattern1, with a delay of 250 milliseconds between each change.
 * 
 * \section Req_ID
 * LedShape1Func_0x08
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * This function allows the four LEDs to be controlled in a specific pattern1.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create i with initial value = 0; 
 *  while (i < 10 ?) is (yes)
 *  :Set pinA9;	 
 *  :Reset pinA10;	
 *  :Reset pinA11;	
 *  :Set pinA12;
 *  :Call runnable Dio; 
 *  :Delay for 250ms; 
 *  :Reset pinA9;	 
 *  :Set pinA10;	
 *  :Set pinA11;	
 *  :Reset pinA12;
 *  :Call runnable Dio;  
 *  :Delay for 250ms;  
 *  :i++;
 *  endwhile (No)
 *  #green:Resume ManagerTask;
 *  #green:Resume ActionTask;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == Dio functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A9, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Set Dio_A9 pin
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A10, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Reset Dio_A10 pin
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A11, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Reset Dio_A11 pin
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A12, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Set Dio_A12 pin
 *
 * "LedShape1\nFunction" -> "Runnable\nFunction": **DIO_RUNNABLE(0x02)**
 * note right #blue: Apply Action on the four leds' pins.
 *
 * == RTOS functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape1\nFunction" -> "vTaskDelay\nFunction": Time to be blocked(250ms) Request
 * "vTaskDelay\nFunction" -[#blue]-> "LedShape1\nFunction": Block Task with the requested time(250ms)
 *
 * == Dio functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A9, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Reset Dio_A9 pin
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A10, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Set Dio_A10 pin
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A11, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Set Dio_A11 pin
 *
 * "LedShape1\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A12, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape1\nFunction": Reset Dio_A12 pin
 *
 * "LedShape1\nFunction" -> "Runnable\nFunction": **DIO_RUNNABLE(0x02)**
 * note right #blue: Apply Action on the four leds' pins.
 *
 * == RTOS functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape1\nFunction" -> "vTaskDelay\nFunction": Time to be blocked(250ms) Request
 * "vTaskDelay\nFunction" -[#blue]-> "LedShape1\nFunction": Block Task with the requested time(250ms)
 *
 * == Out of loop RTOS functions sequence ==
 * "LedShape1\nFunction" -[#blue]> "vTaskResume\nFunction": TaskManager_Handler
 * note right #green: Resume TaskManager
 *
 * "LedShape1\nFunction" -[#blue]> "vTaskResume\nFunction": ActionTask_Handler
 * note right #green: Resume ActionTask
 * @enduml
 *
*/
void LedShape1(){
	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_EnumSetterPin(Dio_A9, STD_ON);
		Dio_EnumSetterPin(Dio_A10, STD_OFF);
		Dio_EnumSetterPin(Dio_A11, STD_OFF);
		Dio_EnumSetterPin(Dio_A12, STD_ON);
		Runnable(DIO_RUNNABLE);
		vTaskDelay(pdMS_TO_TICKS(250));
		Dio_EnumSetterPin(Dio_A9, STD_OFF);
		Dio_EnumSetterPin(Dio_A10, STD_ON);
		Dio_EnumSetterPin(Dio_A11, STD_ON);
		Dio_EnumSetterPin(Dio_A12, STD_OFF);
		Runnable(DIO_RUNNABLE);
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}


/**
 * \section Service_Name
 * LedShape2Function
 *
 * \section Description
 * Turns on and off the four LEDs in a specific pattern2, with a delay of 250 milliseconds between each change.
 * 
 * \section Req_ID
 * LedShape2Func_0x09
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * This function allows the four LEDs to be controlled in a specific pattern2.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create i with initial value = 0; 
 *  while (i < 10 ?) is (yes)
 *  :Set pinA9;	 
 *  :Reset pinA10;	
 *  :Set pinA11;	
 *  :Reset pinA12;
 *  :Call runnable Dio;
 *  :Delay for 250ms; 
 *  :Reset pinA9;	 
 *  :Set pinA10;	
 *  :Reset pinA11;	
 *  :Set pinA12;
 *  :Call runnable Dio;
 *  :Delay for 250ms;  
 *  :i++;
 *  endwhile (No)
 *  #green:Resume ManagerTask;
 *  #green:Resume ActionTask;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == Dio functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A9, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Set Dio_A9 pin
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A10, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Reset Dio_A10 pin
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A11, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Set Dio_A11 pin
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A12, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Reset Dio_A12 pin
 *
 * "LedShape2\nFunction" -> "Runnable\nFunction": **DIO_RUNNABLE(0x02)**
 * note right #blue: Apply Action on the four leds' pins. 
 *
 * == RTOS functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape2\nFunction" -> "vTaskDelay\nFunction": Time to be blocked(250ms) Request
 * "vTaskDelay\nFunction" -[#blue]-> "LedShape2\nFunction": Block Task with the requested time(250ms)
 *
 * == Dio functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A9, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Reset Dio_A9 pin
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A10, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Set Dio_A10 pin
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A11, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Reset Dio_A11 pin
 *
 * "LedShape2\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A12, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape2\nFunction": Set Dio_A12 pin
 *
 * "LedShape2\nFunction" -> "Runnable\nFunction": **DIO_RUNNABLE(0x02)**
 * note right #blue: Apply Action on the four leds' pins. 
 *
 * == RTOS functions sequence (inside loop of sequence = 10) == 
 *
 * "LedShape2\nFunction" -> "vTaskDelay\nFunction": Time to be blocked(250ms) Request
 * "vTaskDelay\nFunction" -[#blue]-> "LedShape2\nFunction": Block Task with the requested time(250ms)
 *
 * == Out of loop RTOS functions sequence ==
 * "LedShape2\nFunction" -[#blue]> "vTaskResume\nFunction": TaskManager_Handler
 * note right #green: Resume TaskManager
 *
 * "LedShape2\nFunction" -[#blue]> "vTaskResume\nFunction": ActionTask_Handler
 * note right #green: Resume ActionTask
 * @enduml
 *
*/
void LedShape2(){
	for (uint8 i = 0 ; i < 10 ; i++){
		Dio_EnumSetterPin(Dio_A9, STD_ON);
		Dio_EnumSetterPin(Dio_A10, STD_OFF);
		Dio_EnumSetterPin(Dio_A11, STD_ON);
		Dio_EnumSetterPin(Dio_A12, STD_OFF);
		Runnable(DIO_RUNNABLE);
		vTaskDelay(pdMS_TO_TICKS(250));
		Dio_EnumSetterPin(Dio_A9, STD_OFF);
		Dio_EnumSetterPin(Dio_A10, STD_ON);
		Dio_EnumSetterPin(Dio_A11, STD_OFF);
		Dio_EnumSetterPin(Dio_A12, STD_ON);
		Runnable(DIO_RUNNABLE);
		vTaskDelay(pdMS_TO_TICKS(250));
	}
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}



/**
 * \section Service_Name
 * LedShape3Function
 *
 * \section Description
 * Turns on and off the four LEDs in a specific pattern3.
 * 
 * \section Req_ID
 * LedShape3Func_0x0A
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * This function allows the four LEDs to be controlled in a specific pattern3.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Set pinA9;	 
 *  :Set pinA10;	
 *  :Reset pinA11;	
 *  :Reset pinA12; 
 *  :Call runnable Dio;  
 *  :Delay for 2500ms;  
 *  #green:Resume ManagerTask;
 *  #green:Resume ActionTask;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == Dio functions sequence == 
 *
 * "LedShape3\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A9, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape3\nFunction": Reset Dio_A9 pin
 *
 * "LedShape3\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A10, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape3\nFunction": Reset Dio_A10 pin
 *
 * "LedShape3\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A11, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape3\nFunction": Set Dio_A11 pin
 *
 * "LedShape3\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A12, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape3\nFunction": Set Dio_A12 pin
 *
 * "LedShape3\nFunction" -> "Runnable\nFunction": **DIO_RUNNABLE(0x02)**
 * note right #blue: Apply Action on the four leds' pins.   
 *
 * == RTOS functions sequence == 
 *
 * "LedShape3\nFunction" -> "vTaskDelay\nFunction": Time to be blocked(2500ms) Request
 * "vTaskDelay\nFunction" -[#blue]-> "LedShape3\nFunction": Block Task with the requested time(2500ms)
 *
 * "LedShape3\nFunction" -[#blue]> "vTaskResume\nFunction": TaskManager_Handler
 * note right #green: Resume TaskManager
 *
 * "LedShape3\nFunction" -[#blue]> "vTaskResume\nFunction": ActionTask_Handler
 * note right #green: Resume ActionTask
 * @enduml
 *
*/
void LedShape3(){
	Dio_EnumSetterPin(Dio_A9, STD_ON);
	Dio_EnumSetterPin(Dio_A10, STD_ON);
	Dio_EnumSetterPin(Dio_A11, STD_OFF);
	Dio_EnumSetterPin(Dio_A12, STD_OFF);
	Runnable(DIO_RUNNABLE);
	vTaskDelay(pdMS_TO_TICKS(2500));
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}


/**
 * \section Service_Name
 * LedShape4Function
 *
 * \section Description
 * Turns on and off the four LEDs in a specific pattern4.
 * 
 * \section Req_ID
 * LedShape4Func_0x0B
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * This function allows the four LEDs to be controlled in a specific pattern3.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Reset pinA9;	 
 *  :Reset pinA10;	
 *  :Set pinA11;	
 *  :Set pinA12; 
 *  :Call runnable Dio;  
 *  :Delay for 2500ms;  
 *  #green:Resume ManagerTask;
 *  #green:Resume ActionTask;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * == Dio functions sequence == 
 *
 * "LedShape4\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A9, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape4\nFunction": Reset Dio_A9 pin
 *
 * "LedShape4\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A10, STD_OFF
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape4\nFunction": Reset Dio_A10 pin
 *
 * "LedShape4\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A11, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape4\nFunction": Set Dio_A11 pin
 *
 * "LedShape4\nFunction" -> "Dio_WriteChannel\nFunction": Dio_A12, STD_ON
 * "Dio_WriteChannel\nFunction" -[#blue]-> "LedShape4\nFunction": Set Dio_A12 pin
 *
 * "LedShape4\nFunction" -> "Runnable\nFunction": **DIO_RUNNABLE(0x02)**
 * note right #blue: Apply Action on the four leds' pins.   
 *
 * == RTOS functions sequence == 
 *
 * "LedShape4\nFunction" -> "vTaskDelay\nFunction": Time to be blocked(2500ms) Request
 * "vTaskDelay\nFunction" -[#blue]-> "LedShape4\nFunction": Block Task with the requested time(2500ms)
 *
 * "LedShape4\nFunction" -[#blue]> "vTaskResume\nFunction": TaskManager_Handler
 * note right #green: Resume TaskManager
 *
 * "LedShape4\nFunction" -[#blue]> "vTaskResume\nFunction": ActionTask_Handler
 * note right #green: Resume ActionTask
 * @enduml
 *
*/
void LedShape4(){
	Dio_EnumSetterPin(Dio_A9, STD_OFF);
	Dio_EnumSetterPin(Dio_A10, STD_OFF);
	Dio_EnumSetterPin(Dio_A11, STD_ON);
	Dio_EnumSetterPin(Dio_A12, STD_ON);
	Runnable(DIO_RUNNABLE);
	vTaskDelay(pdMS_TO_TICKS(2500));
	vTaskResume(ManagerTask_Handler);
	vTaskResume(ActionTask_Handler);
}


/**
 * \section Service_Name
 * TTL_ReciveStringFunction
 *
 * \section Description
 * The function TTL_ReciveString receives a string from the UART and displays it on the LCD until the # character is received.
 * 
 * \section Req_ID
 * TTL_ReciveStringFunc_0x0C
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] pvParameter (pointer to void)\ pvParameter--->Hold the Task's variable
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * This function allows the microcontroller to receive a string of text from UART and display it on the LCD.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Create RecivedMassage variable; 
 *  :Clear LCD ;  
 *  :Print space on UART3;   
 *  while (RecivedMassage != # ?) is (yes)
 *  :Receive byte from UART3 in RecivedMassage;	 
 *  if (RecivedMassage contains alphabet?) then (Yes)
 *  #green:Send RecivedMassage to UART3;
 *  #green:Send RecivedMassage to LCD;
 *  else (No)
 * endif
 *  endwhile (No)
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 *
 * == LCD functions sequence (before loop) == 
 *
 * "TTL_ReciveString\nFunction" -> "LCD_VidClear\nFunction":
 * note right #green: Display: Clear screen of LCD
 *
 * == UART functions sequence (before loop) == 
 *
 * "TTL_ReciveString\nFunction" -> "USART_TransmitString\nFunction": UART3, "   "
 * note right #aqua: Display: "   "
 *
 * == UART functions sequence (inside loop of condition RecivedMassage != '#') == 
 *
 * "TTL_ReciveString\nFunction" -> "USART_ReceiveByte\nFunction": UART peripheral(UART3) Request 
 * "USART_ReceiveByte\nFunction" -[#blue]-> "TTL_ReciveString\nFunction": Return Received byte from UART peripheral(UART3)
 *
 * == UART functions sequence (inside condition RecivedMassage is alphabetic) == 
 *
 * "TTL_ReciveString\nFunction" -> "USART_TransmitString\nFunction": UART3, RecivedMassage
 * note right #aqua: Display: RecivedMassage
 *
 * == LCD functions sequence (inside condition RecivedMassage is alphabetic) == 
 * 
 * "TTL_ReciveString\nFunction" -> "LCD_VidSendChar\nFunction": RecivedMassage
 * note right #green: Display: RecivedMassage
 *
 * @enduml
 *
*/
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
