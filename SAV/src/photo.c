/*
 * photo.c
 *
 *  Created on: Mar 2, 2016
 *      Author: Parker
 */

#include "photo.h"


/*********************************************************************************************
 * Timer callback to read photo resistors
 *********************************************************************************************/
void prvReadPhotoCallback( TimerHandle_t pxTimer ) {

	static uint16_t photo1;
	static uint16_t photo2;
	static uint16_t photo3;
	static uint16_t photo_average;

	// Read photo1
	ADC_RegularChannelConfig( PHOTO_ADC, PHOTO_1_CHANNEL, 1, ADC_SampleTime_15Cycles );
	ADC_SoftwareStartConv( PHOTO_ADC );
	while( ADC_GetFlagStatus(PHOTO_ADC, ADC_FLAG_EOC) == RESET );
	photo1 = ADC_GetConversionValue( PHOTO_ADC );

	// Read photo2
	ADC_RegularChannelConfig( PHOTO_ADC, PHOTO_2_CHANNEL, 1, ADC_SampleTime_15Cycles );
	ADC_SoftwareStartConv( PHOTO_ADC );
	while( ADC_GetFlagStatus(PHOTO_ADC, ADC_FLAG_EOC) == RESET );
	photo2 = ADC_GetConversionValue( PHOTO_ADC );

	// Read photo3
	ADC_RegularChannelConfig( PHOTO_ADC, PHOTO_3_CHANNEL, 1, ADC_SampleTime_15Cycles );
	ADC_SoftwareStartConv( PHOTO_ADC );
	while( ADC_GetFlagStatus(PHOTO_ADC, ADC_FLAG_EOC) == RESET );
	photo3 = ADC_GetConversionValue( PHOTO_ADC );

	// Average values
	photo_average = (photo1 + photo2 + photo3)/3;

	// Check if over a white line
	if( (photo_average > 3200) && (photo_previous_line == black) ) {
		photo_previous_line = white;
//		photo_counter += 1;
		LED_MODE_PORT->OFF = LED_MODE_1_PIN;
	}

	// Check if over a black line
	else if( (photo_average <= 3200) && (photo_previous_line == white) ) {
		photo_previous_line = black;
		photo_counter += 1;
		LED_MODE_PORT->ON = LED_MODE_1_PIN;
		if( (photo_counter == 1) && (photo_intersection == FALSE) ) {
			// Adjust steering
//			if( photo_direction == ns ) {
//				steer = 48;
//			} else {
//				steer = 54;
//			}
			// Straighten steering
			default_steer = 50;

			// Update steering based on new value
			ultra_reset_counter = 2;
		}
	}

	if( photo_counter >= 5 ) {

		if( photo_intersection == TRUE ) {
			// Leaving the intersection
			photo_intersection = FALSE;

			// Adjust steering
			if( photo_direction == ns ) {
				default_steer = 63;
			} else {
				default_steer = 41;
			}

			// Update steering based on new value
			ultra_reset_counter = 2;
		} else {
			// Entering intersection
			photo_intersection = TRUE;

			// Update direction SAV is traveling
			photo_direction = next_direction[photo_direction];
		}

		// Reset the counter
		photo_counter = 0;
	}
}


/*********************************************************************************************
 * Initialize Global varibales
 *********************************************************************************************/
uint8_t photo_counter = 0;
LineColor photo_previous_line = white;
uint8_t photo_intersection = TRUE;
Direction photo_direction = ns;


/*********************************************************************************************
 * Map direction transitions
 *********************************************************************************************/
Direction next_direction[2] = {ew, ns};
