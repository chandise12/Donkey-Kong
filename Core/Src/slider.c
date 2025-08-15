/*
 * slider.c
 *
 *  Created on: Aug 13, 2025
 *      Author: Chandise Anderson
 */

#include "slider.h"
#include "stdio.h"


void start_slider(){
	HAL_ADC_Start(&hadc1);
}

uint16_t get_pos(){
	uint16_t pos = conv_pos();
	return pos;
}

uint16_t conv_pos(){
	float volt = conv_ADC();

	float conv = (volt/3.3) * 128.0 + 0.5;
	return (uint16_t)conv;
}

float conv_ADC(){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	uint32_t adc = HAL_ADC_GetValue(&hadc1);


	float volt = ((float)adc / 4095.0) * 3.3;
	return volt;
}
