/*
 * slider.h
 *
 *  Created on: Aug 13, 2025
 *      Author: Chandise Anderson
 */

#ifndef INC_SLIDER_H_
#define INC_SLIDER_H_

#include "main.h"

extern volatile uint8_t new_pos;

void start_slider();
int16_t get_pos();
//void conv_pos();
void get_ADC();
uint8_t get_dir();
uint8_t in_motion();



#endif /* INC_SLIDER_H_ */
