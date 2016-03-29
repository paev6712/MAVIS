/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/


#include "main.h"


int main(void) {

	// Create initial task to connect to Base Station
	xTaskCreate( prvSetupTask, "", 300 * sizeof(uint8_t), NULL, setupPriority, xSetupHandle );

	// Start the scheduler which begins to run the tasks
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}


/*********************************************************************************************
 * Setup hardware/software
 *********************************************************************************************/
void prvSetupTask( void *pvParameters ) {

	// Setup hardware
	prvSetupHardware();

	// Setup WiFi connection
	prvSetupWifi();

	// Set mode to allModes
	my_mode = allModes;

	// Create queue for packets
	xPacketQueue = xQueueCreate( maxPacketQueueLength, MAX_LENGTH*sizeof(uint8_t) );

	// Create initial task to connect to Base Station
	xTaskCreate( prvConnectTask, "", 300 * sizeof(uint8_t), NULL, connectPriority, xConnectHandle );

	// Delete this task
	vTaskDelete( xSetupHandle );
}


/*********************************************************************************************
 * Setup relevant hardware
 *********************************************************************************************/
static void prvSetupHardware( void ) {
	// Ensure that all 4 interrupt priority bits are used as the pre-emption priority
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	// Setup LEDs
	ledInit();

	// Setup WiFi
	wifiInit();
}


/*********************************************************************************************
 * Setup Wifi connection
 *********************************************************************************************/
static void prvSetupWifi( void ) {
	// Turn on Echo commands (for Arduino)
//	sendPacket( "ATE1", 4);

	// Connect to Wifi
//	sendPacket( "AT+CWJAP=\"OhmWreckers\",\"123\"", 28);
//	swDelay(1500);

	// Send AT
	sendPacket( "AT", 2);
	swDelay(500);

	// Set mux to be 1
	sendPacket( "AT+CIPMUX=1", 11);
	swDelay(500);

	// Start server
	sendPacket( "AT+CIPSERVER=1,1001", 19);
	swDelay(500);

	// Turn on WiFi LEDs to indicate the network is setup
	LED_WIFI_PORT->ON = LED_WIFI_PINS;
}


/*********************************************************************************************
 * Initial bootup task to connect to Base Station
 * 		* Connect to Base Station 		- Send Ping packet
 * 		* Wait for response 			- Receive Ack packet
 * 		* If successful					- Turn on WiFi LEDs
 *********************************************************************************************/
void prvConnectTask( void *pvParameters ) {

	// Let task run infinitely
	for(;;) {

		// Wait for a Ping from an SAV
		if( process_packet ) {

			// Create local string to represent the packet
			char* packet = pvPortMalloc( MAX_LENGTH*sizeof(uint8_t) );

			// Pop packet from queue
			xQueueReceive( xPacketQueue, packet, 0 );

			// Process packet
			PacketResult packet_result = handlePacket( packet );

			// Free variables
			vPortFree( packet );

			if( (packet_result.result == SUCCESS) && (packet_result.type == ping) ) {

				// Reset process_packet
				process_packet = FALSE;

				// Move on to next task
				xTaskCreate( prvModeOfOperationTask, "", 300 * sizeof(uint8_t), NULL, modeOfOperationPriority, xModeOfOperationHandle );

				// Delete this task
				vTaskDelete( xConnectHandle );
			}
		}
	}
}


/*********************************************************************************************
 * Initial bootup task to setup default mode of operation
 * 		* Indicate SAV needs to set default mode of operation
 * 		* Wait for changeMode packet
 *********************************************************************************************/
void prvModeOfOperationTask( void *pvParameters ) {

	// Let task run infinitely
	for(;;) {

		if( process_packet ) {

			// Create local string to represent the packet
			char* packet = pvPortMalloc( MAX_LENGTH*sizeof(uint8_t) );

			// Pop packet from queue
			xQueueReceive( xPacketQueue, packet, 0 );

			// Process packet
			PacketResult packet_result = handlePacket( packet );

			// Free variables
			vPortFree( packet );

			if( (packet_result.result == SUCCESS) && (packet_result.type == changeMode) ) {

				// Reset process_packet
				process_packet = FALSE;

				// If successful, move on to next task
				xTaskCreate( prvTrafficLightTask, "", ( unsigned short ) 300, NULL, trafficLightPriority, xTrafficLightHandle );

				// Delete this task
				vTaskDelete( xModeOfOperationHandle );
			} else {
				// Throw error
				LED_ERROR_PORT->ON &= LED_ERROR_PIN;
			}
		}
	}
}


/*********************************************************************************************
 * Task to handle (TrafficLight) packets
 * 		*
 *********************************************************************************************/
void prvTrafficLightTask( void *pvParameters ) {
	// Let task run infinitely
	for(;;) {

		if( process_packet ) {

			// Create local string to represent the packet
			char* packet = pvPortMalloc( MAX_LENGTH*sizeof(uint8_t) );

			// Pop packet from queue
			xQueueReceive( xPacketQueue, packet, 0 );

			// Process packet
			handlePacket( packet );

			// Free variables
			vPortFree( packet );

			// Reset process_packet
			process_packet = FALSE;
		}
	}
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Functions to handle errors if they occur
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

	for( ;; );
}


/*********************************************************************************************
 * Functions to handle errors if they occur
 *********************************************************************************************/
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	for( ;; );
}


/*********************************************************************************************
 * Functions to handle errors if they occur
 *********************************************************************************************/
void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amout of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}
