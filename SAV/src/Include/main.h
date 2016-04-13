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
#include "ultrasonic.h"


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
 * Blink LEDs on board
 *********************************************************************************************/
static void prvBlinkLeds( void );


/*********************************************************************************************
 * Setup Wifi connection
 *********************************************************************************************/
static void prvSetupWifi( void );


#endif /* MAIN_H_ */
