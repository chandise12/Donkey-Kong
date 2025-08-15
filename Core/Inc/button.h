/*
 * button.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Chandise Anderson
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include <stdbool.h>
#include "main.h"


extern bool is_climb;
extern bool is_shield;
extern bool is_timeout;
extern uint32_t shield_time;
extern uint32_t timeout_time;
extern uint32_t shield_count;
extern uint32_t timeout_count;

void climb_pressed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void shield_pressed(uint16_t GPIO_Pin);
void shield_counter();
void timeout_counter();
void set_shield_time(uint32_t time);
void set_timeout_time(uint32_t time);


#endif /* INC_BUTTON_H_ */
