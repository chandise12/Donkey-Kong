/*
 * game.c
 *
 *  Created on: Aug 27, 2025
 *      Author: Chandise Anderson
 */

#include "slider.h"
#include "button.h"
#include "audio.h"
#include "st7735.h"
#include "fonts.h"
#include "graphics.h"

#define FLOOR_LVL(y_ground) y_ground - 17
#define STARTUP 0
#define GAMEPLAY 1
#define LOSE	2


uint8_t state = STARTUP;

void create_map() {
	ST7735_FillScreen(ST7735_BLACK);
	for (int i = 0; i < 7; i++) {
		if (i == 0) {
			ST7735_DrawImage(0, ground_pos[i], 128, 2,
					(const uint16_t*) ground);
		} else {
			ST7735_DrawImage(0, ground_pos[i], 128, 5,
					(const uint16_t*) ground);
		}
		ST7735_DrawImage(ladder_pos[i], ground_pos[i]-19, 10, 19,
				(const uint16_t*) ladder);
	}
}


typedef struct {
	uint8_t lvl;
	uint8_t x;
	uint8_t y;
	uint8_t direction;
	uint8_t motion;
	uint8_t climb;
}player_t;

void player_Init(player_t *player) {
	player->lvl = 0;
//	player->x = 0;
//	player->y = ground_pos[0] - 17;
	player->direction = 1;
	player->motion = 0;
	player->climb = 0;


//	ST7735_DrawImage(player->x, player->y, 12, 16, (const uint16_t*) standR);
}



typedef enum {
	left, right, idle_left, idle_right
}slider_t;

typedef enum {
	not_pressed, is_pressed
} button_t;

typedef struct {
	uint8_t position;
	slider_t slider;
	button_t button_climb;
	button_t button_shield;
}input_t;

uint8_t can_climb(player_t *player, input_t *input){
	if(ladder_pos[player->lvl]-2 <= input->position && ladder_pos[player->lvl]+12 >= input->position){ // player's leftmost side is between both ladder poles
		return 1;
	}else if(input->position+14 >= ladder_pos[player->lvl]-2 && input->position+14 <= ladder_pos[player->lvl]+12 ){// player's rightmost side is between both ladder poles
		return 1;
	}
	return 0;
}

void update_input(player_t *player, input_t *input){
	input->position = get_pos();

	if(in_motion()){
		input->slider = get_dir();
	}else{
		input->slider = get_dir()+2;
	}

	input->button_climb = climb_pressed();
	if(input->button_climb && !get_climb() && can_climb(player, input)){
		set_climb(1);
	}

	shield_pressed();
	input->button_shield = get_shield();
}




void update_player(player_t *player, input_t *input) {
	static uint8_t animate = 0;

	if(get_climb() && player->climb == 0){
		player->x = ladder_pos[player->lvl];
		player->lvl++;
		player->y--;
		player->climb++;
		player->motion ^= 1;
		if(get_shield()){
			ST7735_DrawImage(player->x , player->y, shieldR_size[0], shieldR_size[1], (const uint16_t*) shieldR);
		}else{
			ST7735_DrawImage(player->x , player->y, climb_animate[player->motion].w, climb_animate[player->motion].h, (const uint16_t*) climb_animate[player->motion].data);

		}
		return;
	}else if(get_climb() && player->y == ground_pos[player->lvl]){
		player->climb = 0;
		set_climb(0);
		return;
	}else if(get_climb()){

		if(animate >= 150){
			player->y--;
			player->motion ^= 1; //rotate through running animations
			animate = 0;
		}
		if(get_shield()){
			ST7735_DrawImage(player->x , player->y, shieldR_size[0], shieldR_size[1], (const uint16_t*) shieldR);
		}else{
			ST7735_DrawImage(player->x , player->y, climb_animate[player->motion].w, climb_animate[player->motion].h, (const uint16_t*) climb_animate[player->motion].data);

		}
		animate++;
		return;
	}

	player->x = input->position;
	player->y = FLOOR_LVL(ground_pos[player->lvl]);
	switch(input->slider){
	case left:
		if(input->button_shield){
			ST7735_DrawImage(player->x , player->y, shieldL_size[0], shieldL_size[1], (const uint16_t*) shieldL);
		}else{
			ST7735_DrawImage(player->x , player->y, runL_animate[player->motion].w, runL_animate[player->motion].h, runL_animate[player->motion].data);
			if(animate >= 200){
				player->motion ^= 1; //rotate through running animations
				animate = 0;
			}
			animate++;
		}
		break;
	case right:
		if(get_shield()){
			ST7735_DrawImage(player->x , player->y, shieldR_size[0], shieldR_size[1], (const uint16_t*) shieldR);
		}else{
			ST7735_DrawImage(player->x , player->y, runR_animate[player->motion].w, runR_animate[player->motion].h, runR_animate[player->motion].data);
			if(animate >= 200){
				player->motion ^= 1; //rotate through running animations
				animate = 0;
			}
			animate++;
		}
			break;
	case idle_left:
		if(get_shield()){
			ST7735_DrawImage(player->x , player->y, shieldL_size[0], shieldL_size[1], (const uint16_t*) shieldL);
		}else{
			ST7735_DrawImage(player->x , player->y, standL_size[0], standL_size[1], (const uint16_t*) standL);
		}
			break;
	case idle_right:
		if(get_shield()){
			ST7735_DrawImage(player->x , player->y, shieldR_size[0], shieldR_size[1], (const uint16_t*) shieldR);
		}else{
			ST7735_DrawImage(player->x , player->y, standR_size[0], standR_size[1], (const uint16_t*) standR);
		}
			break;
	}

}



void erase_player(player_t *player, input_t *input ){
	ST7735_DrawImage(player->x , player->y, 16, 16, (const uint16_t*) black);
	if(player->y == ground_pos[player->lvl]){ //redraw ladder and ground

		ST7735_DrawImage(ladder_pos[player->lvl-1], ground_pos[player->lvl-1]-19, 10, 19,
				(const uint16_t*) ladder);
		ST7735_DrawImage(0, ground_pos[player->lvl], 128, 5,
							(const uint16_t*) ground);
	}else if(can_climb(player, input)){
		ST7735_DrawImage(ladder_pos[player->lvl], ground_pos[player->lvl]-19, 10, 19,
						(const uint16_t*) ladder);
	}
}






void gameplay() {
	static player_t player;
	static input_t input;

	if (state == STARTUP) {
		create_map();
		start_slider();
		ST7735_Init();
		player_Init(&player);
		HAL_Delay(1000);
		state = GAMEPLAY;
	} else {
		update_input(&player, &input);
		update_player(&player,&input);
		erase_player(&player, &input);

	}
}


//TBD


//void draw_player(struct player_t *player) {
//	static uint8_t animate = 0;
//	if(player->direction == 0 && player->motion[0] == 0){ // facing left, not moving
//		ST7735_DrawImage(player->x, player->y, 12, 16, (const uint16_t*) standL);
//
//	}else if(player->direction == 0 && player->motion[0] == 1){ //facing left, moving
//		ST7735_DrawImage(player->x, player->y, runL_animate[player->motion[1]].w, runL_animate[player->motion[1]].h, runL_animate[player->motion[1]].data);
//		if(animate == 200){
//			player->motion[1] ^= 1; //rotate through running animations
//			animate = 0;
//		}
//		animate++;
//	}else if(player->direction == 1 && player->motion[0] == 0){ // facing right, not moving
//		ST7735_DrawImage(player->x, player->y, 12, 16, (const uint16_t*) standR);
//
//	}else{ // facing right, moving
//		ST7735_DrawImage(player->x, player->y, runR_animate[player->motion[1]].w, runR_animate[player->motion[1]].h, runR_animate[player->motion[1]].data);
//		if(animate == 200){
//			player->motion[1] ^= 1; //rotate through running animations
//			animate = 0;
//		}
//		animate++;
//	}
//
//	ST7735_FillRectangleFast(player->x, player->y, 16, 16, ST7735_BLACK);
//}
//
