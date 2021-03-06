/*
 * trafficLight.h
 *
 *  Created on: Feb 18, 2016
 *      Author: Parker
 */

#ifndef TRAFFICLIGHT_H_
#define TRAFFICLIGHT_H_

#include "hardware.h"
#include "swTimer.h"


/*********************************************************************************************
 * Define the states of the light
 *********************************************************************************************/
typedef enum {
	red						= 		(uint8_t) 0x00,
	yellow					=		(uint8_t) 0x01,
	green					=		(uint8_t) 0x02,
	off						= 		(uint8_t) 0x03
} lightState;


/*********************************************************************************************
 * Define the states of the state machine to control the lights
 *********************************************************************************************/
typedef enum {
	state_1					=		(uint8_t) 0x00,
	state_2					=		(uint8_t) 0x01,
	state_3					= 		(uint8_t) 0x02,
	state_4					=		(uint8_t) 0x03
} lightSystemState;


typedef struct {
	lightState northSouth;
	uint8_t time_NS;
	lightState eastWest;
	uint8_t time_EW;
} systemState;


/*********************************************************************************************
 * Timer callback to send appropriate TrafficLight packet(s)
 *********************************************************************************************/
void prvSendTrafficLightCallback( TimerHandle_t pxTimer );

TimerHandle_t sendTrafficLight;


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Global Variables
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Keep track of current system state
 *********************************************************************************************/
lightSystemState light_system_state;


/*********************************************************************************************
 * Map colors to specific pins
 *********************************************************************************************/
uint16_t led_light_ns_pin[3];
uint16_t led_light_ew_pin[3];


/*********************************************************************************************
 * Map current light state to next light state
 *********************************************************************************************/
lightState next_light_state[3];


/*********************************************************************************************
 * Keep track of the time between states
 *********************************************************************************************/
uint8_t timer_NS;
uint8_t timer_EW;


/*********************************************************************************************
 * Map current system state to the appropriate light state and timers
 *********************************************************************************************/
systemState current_system_state[4];


/*********************************************************************************************
 * Map current system state to the next system state
 *********************************************************************************************/
lightSystemState next_system_state[4];


/*********************************************************************************************
 * Flag to indicate packets need to be sent
 *********************************************************************************************/
uint8_t update_traffic;

#endif /* TRAFFICLIGHT_H_ */
