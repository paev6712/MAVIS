/*
 * power.c
 *
 *  Created on: Mar 2, 2016
 *      Author: Parker
 */

#include "power.h"
#include "packet.h"


/*********************************************************************************************
 * Timer callback to read power measurement
 *********************************************************************************************/
void prvReadPowerCallback( TimerHandle_t pxTimer ) {

	// Increment time based on the value the timer delays
	power_time += 1;

	// Read ADC pin
	ADC_RegularChannelConfig( MEASURED_POWER_ADC, MEASURED_POWER_CHANNEL, 1, ADC_SampleTime_15Cycles );
	ADC_SoftwareStartConv( MEASURED_POWER_ADC );
	while( ADC_GetFlagStatus(MEASURED_POWER_ADC, ADC_FLAG_EOC) == RESET );
	uint32_t current_power = (uint32_t) ADC_GetConversionValue( MEASURED_POWER_ADC );

	// Add to total power value
	power_total += current_power;

	// Send back packet every 10 readings
	if( power_time%2 == 0 ) {

		// Create header
		Header* header = pvPortMalloc( sizeof(Header) );
		header->dest = baseStation;
		header->addr = MY_ADDR;
		header->mode = allModes;
		header->type = powerConsumption;

		// Convert total power to average power
		uint16_t average_power = (uint16_t) (power_total/power_time);

		// Send packet
		sendPowerConsumption( header, average_power, power_time );
	}
}
