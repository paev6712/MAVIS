/*
 * main.h
 *
 *  Created on: Feb 21, 2016
 *      Author: Parker
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "freeRTOS.h"
#include "stm32f4xx.h"
#include "task.h"
#include "queue.h"
#include "general.h"
#include "hardware.h"
#include "swTimer.h"
#include "packet.h"


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define FreeRTOS Tasks and associated handles, if appropriate
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Setup hardware/software
 *********************************************************************************************/
void prvSetupTask( void *pvParameters );

TaskHandle_t xSetupHandle = NULL;
#define	setupPriority						( tskIDLE_PRIORITY + 1 )


/*********************************************************************************************
 * Initial bootup task to connect to Base Station
 *********************************************************************************************/
void prvConnectTask( void *pvParameters );

TaskHandle_t xConnectHandle = NULL;
#define	connectPriority						( tskIDLE_PRIORITY + 1 )

/*********************************************************************************************
 * Task Priorities
 *********************************************************************************************/
#define	connectPriority						( tskIDLE_PRIORITY + 1 )
#define	ultrasonicPriority					( tskIDLE_PRIORITY + 1 )

/*********************************************************************************************
 * Task Frequencies
 *********************************************************************************************/
#define ultrasonicFreq						( 500 / portTICK_PERIOD_MS)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Global Variables
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

int distCM_front;
int distCM_left;
int distCM_right;
volatile int timerValue; //timer value for ultrasonic counters
volatile int done = 0; //boolean for indicating when a timer value is ready to be read

/*********************************************************************************************
 * Initial bootup task to setup default mode of operation
 *********************************************************************************************/
void prvModeOfOperationTask( void *pvParameters );

TaskHandle_t xModeOfOperationHandle = NULL;
#define	modeOfOperationPriority				( tskIDLE_PRIORITY + 1 )


/*********************************************************************************************
 * Task to handle TrafficLight packets
 *********************************************************************************************/
void prvTrafficLightTask( void *pvParameters );

TaskHandle_t xTrafficLightHandle = NULL;
#define	trafficLightPriority				( tskIDLE_PRIORITY + 1 )


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define Functions
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Setup relevant hardware
 *********************************************************************************************/
static void prvSetupHardware( void );


/*********************************************************************************************
 * Setup Wifi connection
 *********************************************************************************************/
static void prvSetupWifi( void );


/*********************************************************************************************
 * Ultrasonic data collection and processing task
 *********************************************************************************************/
void prvUltrasonic( void *pvParameters );

/*********************************************************************************************
 * Ultrasonic helper functions to change pin modes (input/output)
 *********************************************************************************************/
void setInput(uint16_t pin);
void setOutput(uint16_t pin);


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * SW Timer Callback Functions
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Test
 *********************************************************************************************/
void prvSWTimerCallback( TimerHandle_t pxTimer );


/*********************************************************************************************
 * Send Ping packet
 *********************************************************************************************/
void prvSWTimerPingCallback( TimerHandle_t pxTimer );


#endif /* MAIN_H_ */
