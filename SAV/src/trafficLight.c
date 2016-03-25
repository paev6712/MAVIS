/*
 * trafficLight.c
 *
 *  Created on: Mar 2, 2016
 *      Author: Parker
 */

#include "trafficLight.h"

/*********************************************************************************************
 * Timer callback to blink Traffic Light LED
 *********************************************************************************************/
void prvblinkTrafficLightCallback( TimerHandle_t pxTimer ) {

	if( traffic_time > 0 ) {
		// If there is at least 0.1 seconds until the change, toggle the LED
		LED_LIGHT_PORT->ODR ^= led_light_pin[traffic_future_state];

		// Decrease elapsed time
		traffic_time = traffic_time - 4;
	} else {
		// Turn on the new state
		LED_LIGHT_PORT->OFF = LED_LIGHT_PINS;
		LED_LIGHT_PORT->ON = led_light_pin[traffic_future_state];

		swTimerStop( pxTimer );
	}
}


/*********************************************************************************************
 * Map current state to next state
 *********************************************************************************************/
lightState next_light_state[3] = {green, red, yellow};