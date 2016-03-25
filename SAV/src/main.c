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

	// Setup hardware
	prvSetupHardware();

	// Setup WiFi connection
	prvSetupWifi();

	// Set initial mode to allModes
	my_mode = allModes;

	// Read Measured Power
//	swTimerStart( read_power, 0 );

	// Read Photo Resistor
	swTimerStart( read_photo, 0 );


	// Create queue for packets
	xPacketQueue = xQueueCreate( maxPacketQueueLength, MAX_LENGTH*sizeof(uint8_t) );

	// Create initial task to connect to Base Station
	xTaskCreate( prvConnectTask, "", 300 * sizeof(uint8_t), NULL, connectPriority, xConnectHandle );

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
 * Setup relevant hardware
 *********************************************************************************************/
static void prvSetupHardware( void ) {
	// Ensure that all 4 interrupt priority bits are used as the pre-emption priority
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	// Setup LEDs
	ledInit();

	// Setup WiFi
	wifiInit();

	// Setup PWM
	pwmInit();

	// Setup Photo Resistors
	photoResistorInit();

	// Setup Measured Power
	measuredPowerInit();

	// Setup Ultrasonic
	ultrasonicInit();
}


/*********************************************************************************************
 * Setup Wifi connection
 *********************************************************************************************/
static void prvSetupWifi( void ) {
	// Turn on Echo commands (for Arduino)
//	sendPacket( "ATE1", 4);

	sendPacket( "AT", 2);

	uint32_t t = 10000000;
	while( t-- );

	// Connect to Wifi
//	sendPacket( "AT+CWJAP=\"OhmWreckers\",\"123\"", 28);
//
//	t = 50000000;
//	while( t-- );

	// Connect to IP and set up TCP connection
	sendPacket( "AT+CIPSTART=\"TCP\",\"192.168.101.101\",1001", 40);

	t = 10000000;
	while( t-- );
}


/*********************************************************************************************
 * Initial bootup task to connect to Base Station
 * 		* Connect to Base Station 		- Send Ping packet
 * 		* Wait for response 			- Receive Ack packet
 * 		* If successful					- Turn on WiFi LEDs
 *********************************************************************************************/
void prvConnectTask( void *pvParameters ) {

	// Setup local variables
	uint8_t send_ping = TRUE;

	// Let task run infinitely
	for(;;) {

		if( send_ping ) {
			// Setup header to Base Station
			Header* header = pvPortMalloc( sizeof(Header) );
			header->dest = baseStation;
			header->addr = MY_ADDR;
			header->mode = allModes;
			header->type = ping;

			sendPing( header );

			vPortFree(header);

			send_ping = FALSE;
		}

		// Continue to send Ping packet until get an Ack
		if( process_packet ) {

			// Create local string to represent the packet
			char* packet = pvPortMalloc( MAX_LENGTH*sizeof(uint8_t) );

			// Pop packet from queue
			xQueueReceive( xPacketQueue, packet, 0 );

			// Process packet
			PacketResult packet_result = handlePacket( packet );

			// Free variables
			vPortFree( packet );

			if( (packet_result.result == SUCCESS) && (packet_result.type == ack) ) {
				// Turn on LEDs to indicate success
				LED_WIFI_PORT->ON = LED_WIFI_TX_PIN | LED_WIFI_RX_PIN;
				LED2_PORT->ODR ^= LED2_PIN;

				// Reset process_packet
				process_packet = FALSE;

				// If successful, move on to next task
				xTaskCreate( prvModeOfOperationTask, "", 300 * sizeof(uint8_t), NULL, modeOfOperationPriority, xModeOfOperationHandle );

				// Delete this task
				vTaskDelete( xConnectHandle );
			}
			 else {
				 send_ping = TRUE;
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

	// Send Ack
	Header* header = pvPortMalloc( sizeof(Header) );
	header->dest = baseStation;
	header->addr = MY_ADDR;
	header->mode = allModes;
	header->type = setMode;
	sendSetMode( header );

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



/*********************************************************************************************
 * Send Ping packet
 *********************************************************************************************/
void prvSWTimerPingCallback( TimerHandle_t pxTimer ) {

	// Setup header to Base Station
	Header* header = pvPortMalloc( sizeof(Header) );
	header->dest = baseStation;
	header->addr = MY_ADDR;
	header->mode = allModes;
	header->type = ping;

	sendPing( header );

	vPortFree(header);
}


/*********************************************************************************************
 * Callback function for the test SW timer
 *********************************************************************************************/
void prvSWTimerCallback( TimerHandle_t pxTimer ){
	LED2_PORT->ODR ^= LED2_PIN;

	// Create fake header
	Header* header = pvPortMalloc( sizeof(Header) );
	header->dest = baseStation;
	header->addr = MY_ADDR;
	header->mode = allModes;
	header->type = ack;

	sendAck( header, SUCCESS );

	vPortFree(header);
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
