
#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "map.h"
#include "trafficLight.h"
#include "mode.h"
#include "power.h"
#include "photo.h"
#include "motor.h"

/*********************************************************************************************
 * Hardware functions
 *********************************************************************************************/
void ledInit();
void wifiInit();
void wifiConfig();
void pwmInit();
void pwmTimerConfig();
void photoResistorInit();
void adcConfig();
void measuredPowerInit();
void ultrasonicInit();
void extiConfig();
void ultrasonicTimerConfig();


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Hardware Definitions
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

///*********************************************************************************************
// * General
// *********************************************************************************************/
#define INTERRUPT_ENABLED					1
#define INTERRUPT_DISABLED					0

//#define LED2_PIN							GPIO_Pin_5
//#define LED2_PORT							GPIOA
//#define LED2_CLK							RCC_AHB1Periph_GPIOA
//
//#define SIGNAL1_PIN							GPIO_Pin_6
//#define SIGNAL1_PORT						GPIOA
//#define SIGNAL1_CLK							RCC_AHB1Periph_GPIOA
//
//#define BUTTON_PIN                   		GPIO_Pin_13
//#define BUTTON_PORT            				GPIOC
//#define BUTTON_CLK              			RCC_AHB1Periph_GPIOA
//#define BUTTON_EXTI_PORT_SOURCE      		GPIO_PortSourceGPIOA
//#define BUTTON_EXTI_PIN_SOURCE       		GPIO_PinSource0
//#define BUTTON_EXTI_LINE             		EXTI_Line0
//#define BUTTON_EXTI_IRQn             		EXTI0_IRQn
//
//#define TIMER5_PIN                   		GPIO_Pin_7
//#define TIMER5_PORT            				GPIOA
//#define TIMER5_CLK              			RCC_AHB1Periph_GPIOA
//#define TIMER5_EXTI_PORT_SOURCE      		GPIO_PortSourceGPIOA
//#define TIMER5_EXTI_PIN_SOURCE       		GPIO_PinSource0
//#define TIMER5_EXTI_LINE             		EXTI_Line0
//#define TIMER5_EXTI_IRQn             		EXTI0_IRQn

#define ON									BSRRL
#define OFF									BSRRH

//#define ADC_0_PIN							GPIO_Pin_0
//#define ADC_0_PORT							GPIOA
//#define ADC_0_CLK							RCC_AHB1Periph_GPIOA
//
//#define ADC_1_PIN							GPIO_Pin_1
//#define ADC_1_PORT							GPIOA
//#define ADC_1_CLK							RCC_AHB1Periph_GPIOA
//
//#define ADC_2_PIN							GPIO_Pin_4
//#define ADC_2_PORT							GPIOA
//#define ADC_2_CLK							RCC_AHB1Periph_GPIOA
//
//#define ADC_3_PIN							GPIO_Pin_0
//#define ADC_3_PORT							GPIOB
//#define ADC_3_CLK							RCC_AHB1Periph_GPIOB
//
//#define ADC_4_PIN							GPIO_Pin_1
//#define ADC_4_PORT							GPIOC
//#define ADC_4_CLK							RCC_AHB1Periph_GPIOC
//
//#define ADC_5_PIN							GPIO_Pin_0
//#define ADC_5_PORT							GPIOC
//#define ADC_5_CLK							RCC_AHB1Periph_GPIOC
//
//#define ADC_Periph_CLK						RCC_APB2Periph_ADC1






/*********************************************************************************************
 * Status LEDs
 *********************************************************************************************/
#define LED_LIGHT_RED_PIN					GPIO_Pin_10
#define LED_LIGHT_YELLOW_PIN				GPIO_Pin_11
#define LED_LIGHT_GREEN_PIN					GPIO_Pin_12
#define LED_LIGHT_PINS						GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
#define LED_LIGHT_PORT						GPIOC
#define LED_LIGHT_CLK						RCC_AHB1Periph_GPIOC

#define LED_WIFI_RX_PIN						GPIO_Pin_5
#define LED_WIFI_TX_PIN						GPIO_Pin_6
#define LED_WIFI_PINS						GPIO_Pin_5 | GPIO_Pin_6
#define LED_WIFI_PORT						GPIOB
#define LED_WIFI_CLK						RCC_AHB1Periph_GPIOB

#define LED_MODE_1_PIN						GPIO_Pin_4
#define LED_MODE_2_PIN						GPIO_Pin_5
#define LED_MODE_3_PIN						GPIO_Pin_6
#define LED_MODE_PINS						GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6
#define LED_MODE_PORT						GPIOC
#define LED_MODE_CLK						RCC_AHB1Periph_GPIOC

#define LED_ERROR_PIN						GPIO_Pin_7
#define LED_ERROR_PORT						GPIOB
#define LED_ERROR_CLK						RCC_AHB1Periph_GPIOB


/*********************************************************************************************
 * WiFi Module
 *********************************************************************************************/
#define WIFI_TX_PIN							GPIO_Pin_10
#define WIFI_TX_PINSOURCE					GPIO_PinSource10
#define WIFI_RX_PIN							GPIO_Pin_11
#define WIFI_RX_PINSOURCE					GPIO_PinSource11
#define WIFI_PORT							GPIOB
#define WIFI_CLK							RCC_AHB1Periph_GPIOB
#define WIFI_AF								GPIO_AF_USART3

#define WIFI_USART							USART3
#define WIFI_USART_BAUD						9600
#define WIFI_USART_CLK						RCC_APB1Periph_USART3
#define WIFI_USART_IRQ						USART3_IRQn

//#define WIFI_TX_PIN							GPIO_Pin_9
//#define WIFI_TX_PINSOURCE					GPIO_PinSource9
//#define WIFI_RX_PIN							GPIO_Pin_10
//#define WIFI_RX_PINSOURCE					GPIO_PinSource10
//#define WIFI_PORT							GPIOA
//#define WIFI_CLK							RCC_AHB1Periph_GPIOA
//#define WIFI_AF								GPIO_AF_USART1
//
//#define WIFI_USART							USART1
//#define WIFI_USART_BAUD						9600
//#define WIFI_USART_CLK						RCC_APB2Periph_USART1
//#define WIFI_USART_IRQ						USART1_IRQn


/*********************************************************************************************
 * PWM
 *********************************************************************************************/
#define PWM_MOTOR_1_PIN						GPIO_Pin_7
#define PWM_MOTOR_1_PINSOURCE				GPIO_PinSource7
#define PWM_MOTOR_2_PIN						GPIO_Pin_8
#define PWM_MOTOR_2_PINSOURCE				GPIO_PinSource8
#define PWM_MOTOR_PORT						GPIOC
#define PWM_MOTOR_CLK						RCC_AHB1Periph_GPIOC
#define PWM_MOTOR_AF						GPIO_AF_TIM3

#define PWM_SERVO_PIN						GPIO_Pin_9
#define PWM_SERVO_PINSOURCE					GPIO_PinSource9
#define PWM_SERVO_PORT						GPIOC
#define PWM_SERVO_CLK						RCC_AHB1Periph_GPIOC
#define PWM_SERVO_AF						GPIO_AF_TIM3

#define PWM_TIM								TIM3
#define PWM_TIM_CLK							RCC_APB1Periph_TIM3
#define PWM_TIM_PRESCALER					9
#define PWM_TIM_PERIOD						25210


/*********************************************************************************************
 * Photo Resistors
 *********************************************************************************************/
#define PHOTO_1_PIN							GPIO_Pin_0
#define PHOTO_2_PIN							GPIO_Pin_1
#define PHOTO_3_PIN							GPIO_Pin_2
#define PHOTO_PINS							GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
#define PHOTO_PORT							GPIOC
#define PHOTO_CLK							RCC_AHB1Periph_GPIOC
#define PHOTO_1_CHANNEL						ADC_Channel_10
#define PHOTO_2_CHANNEL						ADC_Channel_11
#define PHOTO_3_CHANNEL						ADC_Channel_12

#define PHOTO_ADC							ADC1
#define PHOTO_ADC_CLK						RCC_APB2Periph_ADC1

/*********************************************************************************************
 * Measured Power
 *********************************************************************************************/
#define MEASURED_POWER_PIN					GPIO_Pin_3
#define MEASURED_POWER_PORT					GPIOC
#define MEASURED_POWER_CLK					RCC_AHB1Periph_GPIOC
#define MEASURED_POWER_CHANNEL				ADC_Channel_13

#define MEASURED_POWER_ADC					ADC1
#define MEASURED_POWER_ADC_CLK				RCC_APB2Periph_ADC1


/*********************************************************************************************
 * ADC (Applies to both Photo Resistors and Measured Power)
 *********************************************************************************************/



/*********************************************************************************************
 * Ultrasonic
 *********************************************************************************************/
#define ULTRA_1_PIN							GPIO_Pin_5 //8 for dev, 5 for final
#define ULTRA_2_PIN							GPIO_Pin_6
#define ULTRA_3_PIN							GPIO_Pin_7
#define ULTRA_PINS							ULTRA_1_PIN | ULTRA_2_PIN | ULTRA_3_PIN
#define ULTRA_PORT							GPIOA
#define ULTRA_CLK							RCC_AHB1Periph_GPIOA

#define ULTRA_EXTI_PINSOURCE_1				EXTI_PinSource5 //8 for dev, 5 for final
#define ULTRA_EXTI_PINSOURCE_2				EXTI_PinSource6
#define ULTRA_EXTI_PINSOURCE_3				EXTI_PinSource7
#define ULTRA_EXTI_PINSOURCES				ULTRA_EXTI_PINSOURCE_1 | ULTRA_EXTI_PINSOURCE_2 | ULTRA_EXTI_PINSOURCE_3
#define ULTRA_EXTI_LINE_1					EXTI_Line5 //8 for dev, 5 for final
#define ULTRA_EXTI_LINE_2					EXTI_Line6
#define ULTRA_EXTI_LINE_3					EXTI_Line7
#define ULTRA_EXTI_LINES					ULTRA_EXTI_LINE_1 | ULTRA_EXTI_LINE_2 | ULTRA_EXTI_LINE_3
#define ULTRA_EXTI_PORT						EXTI_PortSourceGPIOA
#define ULTRA_EXTI_IRQ						EXTI9_5_IRQn

#define ULTRA_TIM_1							TIM2
#define ULTRA_TIM_CLK_1						RCC_APB1Periph_TIM2
#define ULTRA_TIM_PRESCALER_1				400
#define ULTRA_TIM_PERIOD_1					500

#define ULTRA_TIM_2							TIM4
#define ULTRA_TIM_CLK_2						RCC_APB1Periph_TIM4
#define ULTRA_TIM_PRESCALER_2				4000
#define ULTRA_TIM_PERIOD_2					5000

#define ULTRA_TIM_3							TIM5
#define ULTRA_TIM_CLK_3						RCC_APB1Periph_TIM5
#define ULTRA_TIM_PRESCALER_3				4000
#define ULTRA_TIM_PERIOD_3					5000
#endif /* HARDWARE_H_ */
