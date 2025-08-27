/*
 * slider.c
 *
 *  Created on: Aug 13, 2025
 *      Author: Chandise Anderson
 */

#include "slider.h"
#include "stdio.h"
#include <stdbool.h>

float volt = 0.0;
float prev_volt = 0.0;
bool dir = 1; // direction that player is facing; 0 is left, 1 is right

void start_slider(){
	HAL_ADC_Start(&hadc1);
}

uint16_t get_pos(){
	uint16_t pos = conv_pos();
	return pos;
}

uint16_t conv_pos(){
	volt = conv_ADC();


	float conv = (volt/3.3) * 128.0 + 0.5;
	return (uint16_t)conv;
}

float conv_ADC(){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	uint32_t adc = HAL_ADC_GetValue(&hadc1);

	prev_volt = volt;
	volt = ((float)adc / 4095.0) * 3.3;

	return volt;
}

uint8_t get_dir(){
	if(volt > prev_volt){
		dir = 1;
	}else if(volt < prev_volt){
		dir = 0;
	}
	return dir;
}

uint8_t is_still(){
	if(volt == prev_volt){
		return 1;
	}
	return 0;
}
