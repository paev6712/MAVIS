/*
 * trafficLight.c
 *
 *  Created on: Mar 2, 2016
 *      Author: Parker
 */

#include "trafficLight.h"
#include "packet.h"

/*********************************************************************************************
 * Timer callback to blink Traffic Light LED
 *********************************************************************************************/
void prvSendTrafficLightCallback( TimerHandle_t pxTimer ) {

	// Decrement timers
	timer_NS -= 5;
	timer_EW -= 5;

	// Check if timer is 0
	if( (timer_NS == 0) || (timer_EW == 0) ) {
		// Transition to next light system state
		light_system_state = next_system_state[light_system_state];

		// Set timers to associated values
		timer_NS = current_system_state[light_system_state].time_NS;
		timer_EW = current_system_state[light_system_state].time_EW;
	}

	// Light states
	systemState current_lights = current_system_state[light_system_state];

	// Turn on corresponding NS LEDs
	LED_LIGHT_NS_PORT->OFF = LED_LIGHT_NS_PINS;
	LED_LIGHT_NS_PORT->ON = led_light_ns_pin[current_lights.northSouth];

	// Turn on corresponding EW LEDs
	LED_LIGHT_EW_PORT->OFF = LED_LIGHT_EW_PINS;
	LED_LIGHT_EW_PORT->ON = led_light_ew_pin[current_lights.eastWest];

	// Indicate traffic packet needs to be sent
	update_traffic = TRUE;


//	// Light states
//	systemState current_lights = current_system_state[light_system_state];
//
//	// Create Header
//	Header* traffic_header = pvPortMalloc( sizeof(Header) );
//	traffic_header->addr = MY_ADDR;
//	traffic_header->mode = allModes;
//
//	// Iterate through all SAVs
//	uint8_t sav;
//	for(sav=0; sav<NUMBER_SAV; sav++){
//		// Check if active
//		if( wifi_channel_active[sav] == TRUE ) {
//			// Define destination
//			traffic_header->dest = sav;
//
//			// Send current state packet
//			traffic_header->type = trafficLightCurrent;
//			sendTrafficLightCurrent( traffic_header, current_lights.northSouth, current_lights.eastWest );
//
//			// Send future state packet
//			traffic_header->type = trafficLightFuture;
//			sendTrafficLightFuture( traffic_header, next_light_state[current_lights.northSouth], timer_NS,
//													next_light_state[current_lights.eastWest], timer_EW );
//		}
//	}
//
//	// Turn on corresponding NS LEDs
//	LED_LIGHT_NS_PORT->OFF = LED_LIGHT_NS_PINS;
//	LED_LIGHT_NS_PORT->ON = led_light_ns_pin[current_lights.northSouth];
//
//	// Turn on corresponding EW LEDs
//	LED_LIGHT_EW_PORT->OFF = LED_LIGHT_EW_PINS;
//	LED_LIGHT_EW_PORT->ON = led_light_ew_pin[current_lights.eastWest];
//
//	// Decrement timers
//	timer_NS -= 5;
//	timer_EW -= 5;
//
//	// Check if timer is 0
//	if( (timer_NS == 0) || (timer_EW == 0) ) {
//		// Transition to next light system state
//		light_system_state = next_system_state[light_system_state];
//
//		// Set timers to associated values
//		timer_NS = current_system_state[light_system_state].time_NS;
//		timer_EW = current_system_state[light_system_state].time_EW;
//	}
//
//	// Free variables
//	vPortFree( traffic_header );
}


/*********************************************************************************************
 * Map current state to next state
 *********************************************************************************************/
lightState next_light_state[3] = {green, red, yellow};


/*********************************************************************************************
 * Map current system state to the next system state
 *********************************************************************************************/
lightSystemState next_system_state[4] = {state_2, state_3, state_4, state_1};


/*********************************************************************************************
 * Map current system state to the appropriate light state
 *********************************************************************************************/
systemState current_system_state[4] = {{green, 30, red, 35}, {yellow, 5, red, 5}, {red, 35, green, 30}, {red, 5, yellow, 5}};


/*********************************************************************************************
 * Flag to indicate packets need to be sent
 *********************************************************************************************/
uint8_t update_traffic = FALSE;
