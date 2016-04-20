
#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "map.h"
#include "trafficLight.h"
#include "mode.h"

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

/*********************************************************************************************
 * General
 *********************************************************************************************/
#define INTERRUPT_ENABLED					1
#define INTERRUPT_DISABLED					0

#define ON									BSRRL
#define OFF									BSRRH


/*********************************************************************************************
 * Status LEDs
 *********************************************************************************************/
#define LED_LIGHT_NS_RED_PIN				GPIO_Pin_11
#define LED_LIGHT_NS_YELLOW_PIN				GPIO_Pin_12
#define LED_LIGHT_NS_GREEN_PIN				GPIO_Pin_13
#define LED_LIGHT_NS_PINS					GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13
#define LED_LIGHT_NS_PORT					GPIOC
#define LED_LIGHT_NS_CLK					RCC_AHB1Periph_GPIOC

#define LED_LIGHT_EW_RED_PIN				GPIO_Pin_4
#define LED_LIGHT_EW_YELLOW_PIN				GPIO_Pin_5
#define LED_LIGHT_EW_GREEN_PIN				GPIO_Pin_6
#define LED_LIGHT_EW_PINS					GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6
#define LED_LIGHT_EW_PORT					GPIOC
#define LED_LIGHT_EW_CLK					RCC_AHB1Periph_GPIOC

#define LED_WIFI_RX_PIN						GPIO_Pin_5
#define LED_WIFI_TX_PIN						GPIO_Pin_6
#define LED_WIFI_PINS						GPIO_Pin_5 | GPIO_Pin_6
#define LED_WIFI_PORT						GPIOB
#define LED_WIFI_CLK						RCC_AHB1Periph_GPIOB

#define LED_BLUETOOTH_PIN					GPIO_Pin_0
#define LED_BLUETOOTH_PORT					GPIOC
#define LED_BLUETOOTH_CLK					RCC_AHB1Periph_GPIOC

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

// Definitions for dev board
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


#endif /* HARDWARE_H_ */
