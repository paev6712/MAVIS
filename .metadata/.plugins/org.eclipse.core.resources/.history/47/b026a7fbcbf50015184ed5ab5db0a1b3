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

	// Check if over a black line
	if( (photo_average > 1800) && (photo_previous_line == white) ) {
		photo_previous_line = black;
		photo_counter += 1;
		// Toggle LED
		LED_MODE_PORT->ODR ^= LED_MODE_1_PIN;
	} else if( (photo_average <= 1800) && (photo_previous_line == black) ) {
		photo_previous_line = white;
		photo_counter += 1;
		// Toggle LED
		LED_MODE_PORT->ODR ^= LED_MODE_1_PIN;
	}

	if( photo_counter >= 5 ) {
		// Turn on second LED
		LED_MODE_PORT->ODR ^= LED_MODE_2_PIN;
		photo_counter = 0;
	}
}


/*********************************************************************************************
 * Initialize Global varibales
 *********************************************************************************************/
uint8_t photo_counter = 0;
LineColor photo_previous_line = white;
