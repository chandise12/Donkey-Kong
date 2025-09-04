/*
 * slider.c
 *
 *  Created on: Aug 13, 2025
 *      Author: Chandise Anderson
 */

#include "slider.h"
#include <stdlib.h>
#define RESOLUTION 2

TIM_HandleTypeDef htim2;

volatile uint8_t new_adc = 0;
uint32_t adc = 0;
int16_t pos = 0;
int16_t prev_pos = 0;
int16_t pos_conv = 0;

uint8_t dir = 1; // direction that player is facing; 0 is left, 1 is right

void start_slider() {
	HAL_TIM_Base_Start_IT(&htim2);
}


int16_t get_pos() {
	if(new_adc){
		float volt = ((float) adc / 4095.0) * 3.3;

		float conv = (volt / 3.3) * 116.0 + 0.5;
		int16_t conv_int = (int16_t) conv;

		int16_t diff = conv_int - pos;
		if(diff >= RESOLUTION || diff <= -RESOLUTION){
			prev_pos = pos;
			pos = conv_int;
		}
		new_adc = 0;
	}

	return pos;
}

void get_ADC() {
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);  // wait until conversion finishes
	adc = HAL_ADC_GetValue(&hadc1);
	new_adc = 1;
	return;
}


uint8_t in_motion() {
	int16_t diff = pos - prev_pos;
	if ( diff > RESOLUTION || diff < -RESOLUTION ) {
		return 1;
	}

	return 0;
}
uint8_t get_dir() {

	if (pos - prev_pos > RESOLUTION ) {
		dir = 1;
	} else if (prev_pos - pos > RESOLUTION) {
		dir = 0;
	}
	return dir;
}
