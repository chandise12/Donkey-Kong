/*
 * random.c
 *
 *  Created on: Sep 4, 2025
 *      Author: chandisecherine
 */

#include "random.h"


uint8_t generate_number(){
	uint32_t rand = __HAL_TIM_GET_COUNTER(&htim2);
	uint8_t number = (uint8_t)(rand % 100);
	return number;
}


