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
extern uint8_t shield_time;
extern uint8_t timeout_time;
extern uint8_t shield_count;
extern uint8_t timeout_count;

uint8_t climb_pressed();
uint8_t shield_pressed();
void shield_counter();
void timeout_counter();
void set_climb(uint8_t value);
uint8_t get_climb();
uint8_t get_shield();

#endif /* INC_BUTTON_H_ */
