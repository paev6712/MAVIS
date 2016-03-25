/*
 * photo.h
 *
 *  Created on: Mar 2, 2016
 *      Author: Parker
 */

#ifndef PHOTO_H_
#define PHOTO_H_


#include "hardware.h"
#include "swTimer.h"


/*********************************************************************************************
 * Define Black and White
 *********************************************************************************************/
typedef enum {
	black		=		(uint8_t) 0x00,
	white		= 		(uint8_t) 0x01
} LineColor;


/*********************************************************************************************
 * Timer callback to read photo resistors
 *********************************************************************************************/
void prvReadPhotoCallback( TimerHandle_t pxTimer );


/*********************************************************************************************
 * Global varibales
 *********************************************************************************************/
TimerHandle_t read_photo;
LineColor photo_previous_line;
uint8_t photo_counter;


#endif /* PHOTO_H_ */
