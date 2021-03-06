/*
 * swTimer.c
 *
 *  Created on: Dec 10, 2015
 *      Author: Parker
 */

#include "swTimer.h"

/*********************************************************************************************
 * Initialize SW Timer
 *		* timerPeriod: 			Period before timer goes off
 *   	* repeat:				Should timer be reset after it expires
 *    	* pxCallbackFunction: 	Function called when timer expires
 *********************************************************************************************/
TimerHandle_t swTimerInit( uint16_t timer_period, UBaseType_t repeat, TimerCallbackFunction_t pxCallbackFunction ) {

	// Convert period in milliseconds to tick counts
	TickType_t timer_tick = timer_period * ( 1 / portTICK_PERIOD_MS);

	// Create timer
	// Note that pvTimerID is set to NULL so that if the same callback function is assigned to more than one timer it can't tell between them
	TimerHandle_t handle = xTimerCreate("", timer_tick, repeat, NULL, pxCallbackFunction);

	return handle;
}


/*********************************************************************************************
 * Start SW Timer
 * 		* handle:				Pointer to the timer created using swTimerInit
 * 		* timeWait:				Blocking time until timer starts (can be set to 0)
 *********************************************************************************************/
uint8_t swTimerStart( TimerHandle_t handle, uint8_t timeWait ) {

	// If handle is NULL, timer was never properly created
	if( handle == NULL ) {
		return FAILURE;
	}

	// Convert period in milliseconds to wait to tick counts
	TickType_t tickWait = timeWait * (1 / portTICK_PERIOD_MS);
	if( xTimerStart(handle, tickWait) != pdPASS ) {
		return FAILURE;
	}

	return SUCCESS;
}


/*********************************************************************************************
 * Stop SW Timer
 * 		* handle:				Pointer to the timer created using swTimerInit
 *********************************************************************************************/
uint8_t swTimerStop( TimerHandle_t handle ) {

	// If handle is NULL, timer was never properly created
	if( handle == NULL ) {
		return FAILURE;
	}

	if( xTimerStop(handle, 0) != pdPASS ) {
		return FAILURE;
	}

	return SUCCESS;
}


/*********************************************************************************************
 * Check if SW Timer is active
 * 		* handle:				Pointer to the timer created using swTimerInit
 *********************************************************************************************/
uint8_t swIsTimerActive( TimerHandle_t handle ) {

	if( xTimerIsTimerActive( handle ) != pdPASS ) {
		return FAILURE;
	}

	return SUCCESS;
}


/*********************************************************************************************
 * Local timer callback function
 *********************************************************************************************/
void prvDelayCallback( TimerHandle_t pxTimer ) {
	delay_done = TRUE;
}


/*********************************************************************************************
 * Delay for given amount of time (in ms)
 *********************************************************************************************/
uint8_t swDelay( uint16_t delay_time ) {

	// Set global false
	delay_done = FALSE;

	// Create and start local swTimer
	TimerHandle_t delay = swTimerInit( delay_time, NO_REPEAT, prvDelayCallback );
	if( !swTimerStart( delay, 0 ) ) {
		return FAILURE;
	}

	// Wait until timer completes
	while( !delay_done );

	return SUCCESS;
}
