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
	// Prevent changes from ultrasonic happening too quickly
	static uint8_t ultra_left_counter = 5;
	static uint8_t ultra_right_counter = 5;

	// Use software offset to straighten the servo
	static int8_t offset = -1;


	// Consider current state
	if( my_mode == mode2 ) {
		switch( traffic_current_state ) {
			case green:
				motor_action = go;
				break;
			case yellow:
				if( (photo_counter == 3) && (photo_intersection == FALSE) )
					motor_action = stop;
				else if( (photo_counter > 3) && (photo_intersection == FALSE) )
					motor_action = speed;
				else if( (photo_counter < 3) && (photo_counter > 0) && (photo_intersection == FALSE) )
					motor_action = slow;
				else
					motor_action = go;
				break;
			case red:
				if( (photo_counter >= 3) && (photo_intersection == FALSE) )
					motor_action = stop;
				break;
			default:
				break;
		}
	}

	// Consider future state
	if( my_mode == 3 ) {
		switch( traffic_current_state ) {
			case green:
				if( (traffic_time > 2000) && (traffic_time < 5000) && (photo_counter < 3) && (photo_intersection == FALSE) )
					motor_action = speed;
				else if( (traffic_time < 1000) && (photo_counter < 3) && (photo_intersection == FALSE) )
					motor_action = slow;
				else
					motor_action = go;
				break;
			case yellow:
				if( (photo_counter == 3) && (photo_intersection == FALSE) )
					motor_action = stop;
				else if( (photo_counter > 3) && (photo_intersection == FALSE) )
					motor_action = speed;
				else if( (photo_counter < 3) && (photo_counter > 0) && (photo_intersection == FALSE) )
					motor_action = slow;
				else
					motor_action = go;
				break;
			case red:
				if( (traffic_time > 1000) && (photo_counter >= 3) && (photo_intersection == FALSE) )
					motor_action = stop;
				else if( (traffic_time < 1000) && (photo_counter > 0) && (photo_intersection == FALSE))
					motor_action = slow;
				break;
			default:
				break;
		}
	}


	// Set motor speed based on selected action
	switch( motor_action ) {
		case go:
			// If going around curve, go full speed
			if( (steer > 55) || (steer < 45) ) {
				motor_speed = 100;
			} else {
				motor_speed = 90;
			}
			setMotor(forward, motor_speed);
			break;
		case stop:
			setMotor(brake, 0);
			break;
		case slow:
			if( motor_speed > 89 )
				motor_speed -= 5;
			setMotor(forward, motor_speed);
			break;
		case speed:
			if( motor_speed < 96 )
				motor_speed += 5;
			setMotor(forward, motor_speed);
	}


	// Only use ultrasonic if forward ultrasonic is within range
   	if( (distCM_front < 300) && (motor_ultrasonic_enable) ) {
		LED_ERROR_PORT->ON = LED_ERROR_PIN;

		// Modulate steering based on ultrasonic:
		// Both sensors see an object
		if( (distCM_left < 22) && (distCM_right < 22) ) {
			// Do nothing
		}

		// Only the left sensor sees an object
		else if( distCM_left < 22 ) {
			// Increment counter
			ultra_left_counter++;

			// Limit extreme values
			if( (steer > 35) && (ultra_left_counter >= 3) ) {
				steer -= 9;
				ultra_left_counter = 0;
			} else if(ultra_left_counter == 1) {
				steer += 8;
			}

			// Limit extreme values
			if( steer < 38 ) {
				steer = 38;
			}

			// Check if need to still adjust steering for net movement of 1
			if( ultra_right_counter == 1 ) {
				steer -= 8;
			}

			// Since right sensor didn't see anything, reset counter
			ultra_right_counter = 5;
		}

		// Only the right sensor sees an object
		else if( distCM_right < 22 ) {
			// Increment counter
			ultra_right_counter++;

			// Limit extreme values
			if( (steer < 71) && (ultra_right_counter >= 3) ) {
				steer += 9;
				ultra_right_counter = 0;
			} else if(ultra_right_counter == 1) {
				steer -= 8;
			}

			// Limit extreme values
			if( steer > 68 ) {
				steer = 68;
			}

			// Check if need to still adjust steering for net movement of 1
			if( ultra_left_counter == 1 ) {
				steer += 8;
			}

			// Since left sensor didn't see anything, reset counter
			ultra_left_counter = 5;
		}

		// Neither sensor sees an object
		else {
			// Reset both of the counters
			ultra_right_counter = 5;
			ultra_left_counter = 5;
		}
	} else {
		LED_ERROR_PORT->OFF = LED_ERROR_PIN;
	}



	// Set steering
	pwmSet(steer + offset, servo);

	// Initialize servo position off centered so that it can straighten out next iteration
	if( motor_initialize ) {
		pwmSet(60 + offset, servo);
		motor_initialize = FALSE;
	}

}


uint8_t steer = 50;
uint8_t motor_stop = FALSE;
uint8_t motor_initialize = TRUE;
uint8_t motor_ultrasonic_enable = FALSE;
