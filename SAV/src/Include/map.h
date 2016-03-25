/*
 * map.h
 *
 *  Created on: Dec 3, 2015
 *      Author: paev6712
 */

#ifndef MAP_H_
#define MAP_H_

/*********************************************************************************************
 * Enumerate ADCs
 *********************************************************************************************/
// Define ADC parameters
typedef enum {
    ADC_0 = 0,
    ADC_1 = 1,
    ADC_2 = 2,
    ADC_3 = 3,
    ADC_4 = 4,
    ADC_5 = 5
} adc_TypeDef;


/*********************************************************************************************
 * Arrays containing specific information for each enumerated ADC
 *********************************************************************************************/
uint16_t adcPin[6];
GPIO_TypeDef* adcPort[6];
uint32_t adcClk[6];


#endif /* MAP_H_ */
