/*
 * button.c
 *
 *  Created on: Aug 14, 2025
 *      Author: Chandise Anderson
 */

#include "button.h"



TIM_HandleTypeDef htim5;
bool is_climb = 0;
bool is_shield = 0;
bool is_timeout = 0;
uint8_t shield_time = 4; // (seconds)
uint8_t timeout_time = 4; // (seconds)
uint8_t shield_count = 0;
uint8_t timeout_count = 0;

uint8_t climb_pressed() {
	if (HAL_GPIO_ReadPin(GPIOC, BUTTON0_IN_Pin)) { // pin is high, was just pressed
		return 1;
	}
	return 0;
}
void shield_pressed() {
	if (HAL_GPIO_ReadPin(GPIOC, BUTTON1_IN_Pin) && !is_shield && !is_timeout) {
		is_shield = 1;
		HAL_TIM_Base_Start_IT(&htim5);
	}
}

void shield_counter() {
	if (shield_count == shield_time) {
		is_shield = 0;
		is_timeout = 1;
		shield_count = 0;
		timeout_counter();
	} else {
		shield_count++;

		HAL_TIM_Base_Start_IT(&htim5);
	}

}

void timeout_counter() {
	if (timeout_count == timeout_time) {
		is_timeout = 0;
		timeout_count = 0;
	} else {
		timeout_count++;

		HAL_TIM_Base_Start_IT(&htim5);
	}
}


void set_climb(uint8_t value) {
	is_climb = value;
}

uint8_t get_climb() {
	return is_climb;
}

uint8_t get_shield() {
	return is_shield;
}
