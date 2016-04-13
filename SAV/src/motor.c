/*
 * motor.c
 *
 *  Created on: Feb 28, 2016
 *      Author: Parker
 */

#include "motor.h"


/*********************************************************************************************
 * Set the motor output and speed
 *********************************************************************************************/
uint8_t setMotor( MotorOutput motor_output, uint8_t speed ) {

	switch( motor_output ) {
			case coast:
				pwmSet(0, motor1);
				pwmSet(0, motor2);
				break;
			case reverse:
				pwmSet(0, motor1);
				pwmSet(speed, motor2);
				break;
			case forward:
				pwmSet(speed, motor1);
				pwmSet(0, motor2);
				break;
			case brake:
				pwmSet(1, motor1);
				pwmSet(1, motor2);
				break;
	}

	return SUCCESS;
}


/*********************************************************************************************
 * Set PWM to given duty cycle
 *********************************************************************************************/
void pwmSet(uint8_t dutyCycle, Motor motor) {
	// Timer output compare initialization
	TIM_OCInitTypeDef TIM_OC_InitStructure;

	// Configure TIM OC
	TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM2;					// PWM 1 = Clear on match, PWM 2 = Set on match
	TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	// Convert duty cycle to TIM pulse
	uint32_t duty = (PWM_TIM_PERIOD + 1) * dutyCycle;
	duty = (uint32_t) ((duty/100));

	// Set duty cycle
	TIM_OC_InitStructure.TIM_Pulse = duty;

	// Select the corresponding OCInit function
	switch( motor ) {
		case motor1:
			TIM_OC2Init(PWM_TIM, &TIM_OC_InitStructure);
			TIM_OC2PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
			break;
		case motor2:
			TIM_OC3Init(PWM_TIM, &TIM_OC_InitStructure);
			TIM_OC3PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
			break;
		case servo:
			TIM_OC4Init(PWM_TIM, &TIM_OC_InitStructure);
			TIM_OC4PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
			break;
		default:
			break;
	}
}


/*********************************************************************************************
 * Timer callback to read photo resistors
 *********************************************************************************************/
void prvSetMotorCallback( TimerHandle_t pxTimer ) {

	// Keep track of servo position
	static uint8_t steer = 50;

//	// Adjust steering based on ultrasonic values
//	if( distCM_right < 20 ) {
//		// Make sure steer is valid
//		if( steer < 68 ) {
//			steer += 2;
//		}
//	} else if( distCM_left < 20 ) {
//		// Make sure steer is valid
//		if( steer > 35 ) {
//			steer -= 2;
//		}
//	} else {
//		// Reset back to nominal
//		if( steer < 50 ) {
//			steer += 2;
//		} else if( steer > 50 ) {
//			steer -= 2;
//		}
//	}

	if( distCM_right < 20 ) {
		if( steer < 70 ) {
			steer += 1;
		}
	} else {
		if( steer > 50 ) {
			steer -= 1;
		}
	}

	pwmSet(steer, servo);
//	setMotor(forward, 90);
}
