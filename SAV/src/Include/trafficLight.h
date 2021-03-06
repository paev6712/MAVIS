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
 * Timer callback to blink Traffic Light LED
 *********************************************************************************************/
void prvblinkTrafficLightCallback( TimerHandle_t pxTimer );


/*********************************************************************************************
 * Global varibales
 *********************************************************************************************/
TimerHandle_t blinkTrafficLight;
lightState traffic_future_state;
lightState traffic_current_state;
uint16_t traffic_time;


/*********************************************************************************************
 * Map colors to specific pins
 *********************************************************************************************/
uint16_t led_light_pin[3];


/*********************************************************************************************
 * Map current state to next state
 *********************************************************************************************/
lightState next_light_state[3];

#endif /* TRAFFICLIGHT_H_ */
