/*
 * swTimer.h
 *
 *  Created on: Dec 10, 2015
 *      Author: Parker
 */

#ifndef SWTIMER_H_
#define SWTIMER_H_


// Leverage included timers.h to set timers that call functions after a certain amount of time has passed
#include "FreeRTOS.h"
#include "general.h"
#include "timers.h"


/*********************************************************************************************
 * Define constants to repeat timer
 *********************************************************************************************/
#define REPEAT 									pdTRUE
#define NOREPEAT								pdFALSE


/*********************************************************************************************
 * Initialize SW Timer
 *********************************************************************************************/
TimerHandle_t swTimerInit( uint16_t timerPeriod, UBaseType_t repeat, TimerCallbackFunction_t pxCallbackFunction );


/*********************************************************************************************
 * Start SW Timer
 *********************************************************************************************/
uint8_t swTimerStart( TimerHandle_t handle, uint8_t timerWait );


/*********************************************************************************************
 * Stop SW Timer
 *********************************************************************************************/
uint8_t swTimerStop( TimerHandle_t handle );


/*********************************************************************************************
 * Check if SW Timer is active
 *********************************************************************************************/
uint8_t swIsTimerActive( TimerHandle_t handle );

#endif /* SWTIMER_H_ */
