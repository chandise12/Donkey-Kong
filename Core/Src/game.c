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

#define STARTUP 0
#define GAMEPLAY 1

uint8_t state = STARTUP;


void create_map(){
	ST7735_FillScreen(ST7735_BLACK);
	for(int i = 0; i < 7; i++){
		if(i == 0){
			ST7735_DrawImage(0, ground_pos[i], 128, 2, (const uint16_t*)ground);
			ST7735_DrawImage(0,ground_pos[i]-17, 12, 16, (const uint16_t*)standR);
		}else{
			ST7735_DrawImage(0, ground_pos[i], 128, 5, (const uint16_t*)ground);
		}
		ST7735_DrawImage(ladder_pos[i], ground_pos[i]+5, 10, 19, (const uint16_t*)ladder);
	}
}

typedef enum {
	dir_left,dir_right
}dir_t;

typedef enum {
	motion_moving, motion_still
} motion_t;



struct {
	uint8_t lvl;
	uint8_t x;
	uint8_t y;
	dir_t;
	motion_t;
} player_t;

void player_Init(player_t *player){
	lvl = 0;
	player->x = 0;
	player->y = ground_pos[0]-17;
	player->dir_t = dir_right;
	player->motion_t = motion_still;

	ST7735_DrawImage(player->x,player->y, 12, 16, (const uint16_t*)standR);
}

//void update_player



void gameplay(){
	if(state == STARTUP){
		create_map();
		player_t player;
		player_Init(player);
		HAL_Delay(1000);
		state = GAMEPLAY;
	}else{

	}
}


