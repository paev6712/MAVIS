#include "hardware.h"

void ADC_Configuration(void);

/*********************************************************************************************
 * Initialize LEDs
 *********************************************************************************************/
void ledInit() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// Enable clock
	RCC_AHB1PeriphClockCmd( LED_LIGHT_CLK | LED_WIFI_CLK | LED_MODE_CLK | LED_ERROR_CLK, ENABLE);

	// Configure GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = LED_LIGHT_PINS;
	GPIO_Init(LED_LIGHT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED_WIFI_PINS;
	GPIO_Init(LED_WIFI_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED_MODE_PINS;
	GPIO_Init(LED_MODE_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED_ERROR_PIN;
	GPIO_Init(LED_ERROR_PORT, &GPIO_InitStructure);

	// Make sure all LEDs are off
	LED_LIGHT_PORT->OFF = LED_LIGHT_PINS;
	LED_WIFI_PORT->OFF = LED_WIFI_PINS;
	LED_MODE_PORT->OFF = LED_MODE_PINS;
	LED_ERROR_PORT->OFF = LED_ERROR_PIN;

	// Initialize Traffic Light timer
	blinkTrafficLight = swTimerInit( 100, REPEAT, prvblinkTrafficLightCallback );
}


/*********************************************************************************************
 * Initialize WiFi USART GPIO
 *********************************************************************************************/
void wifiInit() {
	GPIO_InitTypeDef     GPIO_InitStruct;

	// Enable clock
	RCC_AHB1PeriphClockCmd(WIFI_CLK, ENABLE);

	// Set Pins to alternate function
	GPIO_PinAFConfig(WIFI_PORT, WIFI_TX_PINSOURCE, WIFI_AF);
	GPIO_PinAFConfig(WIFI_PORT, WIFI_RX_PINSOURCE, WIFI_AF);

	// Initialize pins as alternating function
	GPIO_InitStruct.GPIO_Pin = WIFI_TX_PIN | WIFI_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(WIFI_PORT, &GPIO_InitStruct);

	wifiConfig();
}


/*********************************************************************************************
 * Initialize WiFi USART
 *********************************************************************************************/
void wifiConfig() {
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	// Enable clock
	RCC_APB1PeriphClockCmd(WIFI_USART_CLK, ENABLE);

	// Initialize USART
	USART_InitStruct.USART_BaudRate = WIFI_USART_BAUD;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Cmd(WIFI_USART, ENABLE);
	USART_Init(WIFI_USART, &USART_InitStruct);

	// Enable RX interrupt
	USART_ITConfig(WIFI_USART, USART_IT_RXNE, ENABLE);

	// Initialize NVIC
	NVIC_InitStruct.NVIC_IRQChannel = WIFI_USART_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}


/*********************************************************************************************
 * Initialize PWM GPIO
 *********************************************************************************************/
void pwmInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable clock
	RCC_AHB1PeriphClockCmd(PWM_MOTOR_CLK | PWM_SERVO_CLK, ENABLE);

	// Set alternate function
	GPIO_PinAFConfig(PWM_MOTOR_PORT, PWM_MOTOR_1_PINSOURCE, PWM_MOTOR_AF);
	GPIO_PinAFConfig(PWM_MOTOR_PORT, PWM_MOTOR_2_PINSOURCE, PWM_MOTOR_AF);
	GPIO_PinAFConfig(PWM_SERVO_PORT, PWM_SERVO_PINSOURCE, PWM_SERVO_AF);

	// Configure GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					// Set pin to alternate mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = PWM_MOTOR_1_PIN | PWM_MOTOR_2_PIN;
	GPIO_Init(PWM_MOTOR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PWM_SERVO_PIN;
	GPIO_Init(PWM_SERVO_PORT, &GPIO_InitStructure);

	pwmTimerConfig();

	// Initialize set motor timer
	set_motor = swTimerInit( 100, REPEAT, prvSetMotorCallback );
}


/*********************************************************************************************
 * Initializes Timer
 *********************************************************************************************/
void pwmTimerConfig() {
	TIM_TimeBaseInitTypeDef TIM_InitStructure;

	// Enable clock
	RCC_APB1PeriphClockCmd(PWM_TIM_CLK, ENABLE);

	// Configure TIM
	/*******************************************
	 * For servo, want PWM pulses such that:
	 * 		- 50% duty (1500us) -> 0 degrees
	 * Max values: 700us - 2300us
	 *******************************************/
	TIM_InitStructure.TIM_Prescaler = PWM_TIM_PRESCALER;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = PWM_TIM_PERIOD;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;						// Don't count repetitions
	TIM_TimeBaseInit(PWM_TIM, &TIM_InitStructure);

	// Start timer
	TIM_Cmd(PWM_TIM, ENABLE);
}


/*********************************************************************************************
 * Initializes Photo Resistors
 *********************************************************************************************/
void photoResistorInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable clock
	RCC_AHB1PeriphClockCmd(PHOTO_CLK, ENABLE);

	// Configure GPIO
	GPIO_InitStructure.GPIO_Pin = PHOTO_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;					// Set pin to analog mode
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PHOTO_PORT, &GPIO_InitStructure);

	// Configure ADC
	adcConfig();

	// Initialize read photo resistor timer
	read_photo = swTimerInit( 20, REPEAT, prvReadPhotoCallback );
}


/*********************************************************************************************
 * Initializes ADC
 *********************************************************************************************/
void adcConfig() {
	ADC_InitTypeDef  ADC_InitStructure;

	// Enable clock
	RCC_APB2PeriphClockCmd(PHOTO_ADC_CLK, ENABLE);

	// Put everything back to default values
	ADC_DeInit();

	// Configure ADC
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion= 1;
	ADC_Init(PHOTO_ADC, &ADC_InitStructure);

	// Enable ADC
	ADC_Cmd(PHOTO_ADC, ENABLE);
}


/*********************************************************************************************
 * Initializes Measured Power GPIO
 *********************************************************************************************/
void measuredPowerInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable clock
	RCC_AHB1PeriphClockCmd(MEASURED_POWER_CLK, ENABLE);

	// Configure GPIO
	GPIO_InitStructure.GPIO_Pin = MEASURED_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;					// Set pin to analog mode
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MEASURED_POWER_PORT, &GPIO_InitStructure);

	// Note: Since both get initialized and use the same ADC, don't need to call it twice
	// adcConfig();

	// Initialize read measured power timer
	read_power = swTimerInit( 5000, REPEAT, prvReadPowerCallback );
}


/*********************************************************************************************
 * Initializes Ultrasonic GPIO
 *********************************************************************************************/
void ultrasonicInit() {
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable clock
    RCC_AHB1PeriphClockCmd(ULTRA_CLK, ENABLE);

    // Configure GPIO
    GPIO_InitStruct.GPIO_Pin = ULTRA_PINS;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(ULTRA_PORT, &GPIO_InitStruct);

	extiConfig();

	ultrasonicTimerConfig();
}


/*********************************************************************************************
 * Initializes Ultrasonic EXTI
 *********************************************************************************************/
void extiConfig() {
    EXTI_InitTypeDef EXTI_InitStruct;

    // Set pinsources
    SYSCFG_EXTILineConfig(ULTRA_EXTI_PORT, ULTRA_EXTI_PINSOURCES);

    // Configure EXTI
    EXTI_InitStruct.EXTI_Line = ULTRA_EXTI_LINES;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;

	// Configure Interrupt
	NVIC_InitStruct.NVIC_IRQChannel = ULTRA_EXTI_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


void ultrasonicTimerConfig() {
	TIM_TimeBaseInitTypeDef timerInitStructure_1;

	// Enable clock
	RCC_APB1PeriphClockCmd(ULTRA_TIM_CLK_1, ENABLE);

	// Configure timer
	timerInitStructure_1.TIM_Prescaler = ULTRA_TIM_PRESCALER_1;
	timerInitStructure_1.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure_1.TIM_Period = ULTRA_TIM_PERIOD_1;
	timerInitStructure_1.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure_1.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ULTRA_TIM_1, &timerInitStructure_1);

	// Enable timer
	TIM_Cmd(ULTRA_TIM_1, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure_2;

	// Enable clock
	RCC_APB1PeriphClockCmd(ULTRA_TIM_CLK_2, ENABLE);

	// Configure timer
	timerInitStructure_2.TIM_Prescaler = ULTRA_TIM_PRESCALER_2;
	timerInitStructure_2.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure_2.TIM_Period = ULTRA_TIM_PERIOD_2;
	timerInitStructure_2.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure_2.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ULTRA_TIM_2, &timerInitStructure_2);

	// Enable timer
	TIM_Cmd(ULTRA_TIM_2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure_3;

	// Enable clock
	RCC_APB1PeriphClockCmd(ULTRA_TIM_CLK_3, ENABLE);

	// Configure timer
	timerInitStructure_3.TIM_Prescaler = ULTRA_TIM_PRESCALER_3;
	timerInitStructure_3.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure_3.TIM_Period = ULTRA_TIM_PERIOD_3;
	timerInitStructure_3.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure_3.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(ULTRA_TIM_3, &timerInitStructure_3);

	// Enable timer
	TIM_Cmd(ULTRA_TIM_3, ENABLE);
}


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define Pin Mappings
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * LED pin map
 *********************************************************************************************/
uint16_t led_light_pin[3] = {LED_LIGHT_RED_PIN, LED_LIGHT_YELLOW_PIN, LED_LIGHT_GREEN_PIN};


/*********************************************************************************************
 * LED pin map
 *********************************************************************************************/
uint16_t led_mode_pin[4] = {LED_MODE_PINS, LED_MODE_1_PIN, LED_MODE_2_PIN, LED_MODE_3_PIN};
