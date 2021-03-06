/*
 * ultrasonic.c
 *
 *  Created on: Apr 12, 2016
 *      Author: Parker
 */

#include "ultrasonic.h"


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define Ultrasonic Tasks
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Ultrasonic1 data collection and processing
 *********************************************************************************************/
void prvUltrasonic1Task( void *pvParameters ) {
	uint8_t i;
	for(;;){
	//you must also change these in the interrupt handler below and in hardware.h
//		for front sensor
		setOutput(GPIO_Pin_5);
		GPIOA->ON = GPIO_Pin_5;
		for(i = 0; i < 150; i++){};//delays 10us
		GPIOA->OFF = GPIO_Pin_5;
		setInput(GPIO_Pin_5);
		__enable_irq();
		while(!done1){};
		done1 = 0;
		__disable_irq();
		distCM_front = timerValue1;//convert from timer value to cm

		if(distCM_front < 50)
		{
//			LED_MODE_PORT->ON = LED_MODE_2_PIN;
		}

		else
		{
//			LED_MODE_PORT->OFF = LED_MODE_2_PIN;
		}


		vTaskDelay(ultrasonicFreq);
	}
}


/*********************************************************************************************
 * Ultrasonic2 data collection and processing
 *********************************************************************************************/
void prvUltrasonic2Task( void *pvParameters ) {
	uint8_t i;
	for(;;){
	//		for right sensor
			setOutput(GPIO_Pin_6);
			GPIOA->ON = GPIO_Pin_6;
			for(i = 0; i < 150; i++){};//delays 10us
			GPIOA->OFF = GPIO_Pin_6;
			setInput(GPIO_Pin_6);
			__enable_irq();
			while(!done2){};
			done2 = 0;
			__disable_irq();
			distCM_right = timerValue2;//convert from timer value to cm
			if(distCM_right < 50)
			{
//				LED_MODE_PORT->ON = LED_MODE_1_PIN;
			}

			else
			{
//				LED_MODE_PORT->OFF = LED_MODE_1_PIN;
			}
			vTaskDelay(ultrasonicFreq);
	}

}

/*********************************************************************************************
 * Ultrasonic3 data collection and processing
 *********************************************************************************************/
void prvUltrasonic3Task( void *pvParameters ) {
	uint8_t i;
	for(;;){
	//		for left sensor
			setOutput(GPIO_Pin_7);
			GPIOA->ON = GPIO_Pin_7;
			for(i = 0; i < 150; i++){};//delays 10us
			GPIOA->OFF = GPIO_Pin_7;
			setInput(GPIO_Pin_7);
			__enable_irq();
			while(!done3){};
			done3 = 0;
			__disable_irq();
			distCM_left = timerValue3;//convert from timer value to cm
			if(distCM_left < 50)
			{
//				LED_MODE_PORT->ON = LED_MODE_3_PIN;
			}

			else
			{
//				LED_MODE_PORT->OFF = LED_MODE_3_PIN;
			}
			vTaskDelay(ultrasonicFreq);
	}

}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define Helper Functions
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Sets a pin on port A to input mode
 *********************************************************************************************/
void setInput(uint16_t pin){
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		// Configure GPIO
		GPIO_InitStructure.GPIO_Pin = pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/*********************************************************************************************
 * Sets a pin on port A to output mode
 *********************************************************************************************/
void setOutput(uint16_t pin){
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		// Configure GPIO
		GPIO_InitStructure.GPIO_Pin = pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/*********************************************************************************************
 * Handle interrupt for ultrasonic
 *********************************************************************************************/
void EXTI9_5_IRQHandler(void) {
	// Right sensor
    // Make sure that interrupt flag is set
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
    	// Read the GPIO pin to see if we're on a rising edge or a falling edge
    	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == (uint8_t)Bit_SET) {
    		TIM_SetCounter(TIM2, 0);
    	} else {
    		timerValue1 = TIM_GetCounter(TIM2);
    		done1 = 1;
    	}
    	// Clear interrupt flag
    	EXTI_ClearITPendingBit(EXTI_Line5);
    }

    // Front sensor
    if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
    	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == (uint8_t)Bit_SET) {
    		TIM_SetCounter(TIM4, 0);
    	} else {
    		timerValue2 = TIM_GetCounter(TIM4);
    		done2 = 1;
    	}
    	// Clear interrupt flag
    	EXTI_ClearITPendingBit(EXTI_Line6);
    }

    // Left sensor
    if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
    	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == (uint8_t)Bit_SET) {
    		TIM_SetCounter(TIM5, 0);
    	} else {
    		timerValue3 = TIM_GetCounter(TIM5);
    		done3 = 1;
    	}
    	// Clear interrupt flag
    	EXTI_ClearITPendingBit(EXTI_Line7);
    }
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Global Variables
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

TaskHandle_t xUltrasonic1Handle = NULL;
TaskHandle_t xUltrasonic2Handle = NULL;
TaskHandle_t xUltrasonic3Handle = NULL;

volatile uint8_t done1 = 0;
volatile uint8_t done2 = 0;
volatile uint8_t done3 = 0;
