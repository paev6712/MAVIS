/*
 * motor.h
 *
 *  Created on: Feb 28, 2016
 *      Author: Parker
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "FreeRTOS.h"
#include "stm32f4xx.h"
#include "general.h"
#include "hardware.h"


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define functions to control the DC motor
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Define the output of the DC motor
 * 		* IN1 = X, IN2 = X:					Coast
 * 		* IN1 = 0, IN2 = 0:					Coast
 * 		* IN1 = 0, IN2 = 1:					Reverse
 * 		* IN1 = 1, IN2 = 0:					Forward
 * 		* IN1 = 1, IN2 = 1:					Brake
 *********************************************************************************************/
typedef enum {
	coast					= 		(uint8_t) 0x00,
	reverse					= 		(uint8_t) 0x01,
	forward					= 		(uint8_t) 0x02,
	brake					= 		(uint8_t) 0x03
} MotorOutput;


/*********************************************************************************************
 * Set the motor output and speed
 *********************************************************************************************/
uint8_t setMotor( MotorOutput motor_output, uint8_t speed );


/*********************************************************************************************
 * Map pins to OCInit functions
 *********************************************************************************************/
typedef enum {
	motor1					= 		(uint8_t) 0x00,
	motor2					= 		(uint8_t) 0x01,
	servo					= 		(uint8_t) 0x02
} Motor;


/*********************************************************************************************
 * Set PWM to given duty cycle
 *********************************************************************************************/
void pwmSet(uint8_t dutyCycle, Motor motor);


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define timer callback function
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Timer callback to read photo resistors
 *********************************************************************************************/
void prvSetMotorCallback( TimerHandle_t pxTimer );

TimerHandle_t set_motor;


#endif /* MOTOR_H_ */
