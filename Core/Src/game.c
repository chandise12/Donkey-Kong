/*
 * game.c
 *
 *  Created on: Aug 27, 2025
 *      Author: Chandise Anderson
 */

#include "game.h"
#include "slider.h"
#include "button.h"
#include "audio.h"
#include "st7735.h"
#include "fonts.h"
#include "graphics.h"
#include "random.h"
#include <stdbool.h>

uint8_t state = STARTUP;
const uint8_t *files_audio[7] = {0, ready, punch, win, lose, shield_out, shield_in};
const uint32_t fileslen_audio[7] = {0, ready_length, punch_length, win_length, lose_length, shield_out_length, shield_in_length};

sound_t audios[5] = {0};
uint8_t count = 0;

void create_map() {
	ST7735_FillScreen(ST7735_BLACK);
	for (int i = 0; i < 7; i++) {
		if (i == 0) {
			ST7735_DrawImage(0, ground_pos[i], 128, 1,
					(const uint16_t*) ground);
		} else {
			ST7735_DrawImage(0, ground_pos[i], 128, 5,
					(const uint16_t*) ground);
		}
		ST7735_DrawImage(ladder_pos[i], ground_pos[i]-19, 10, 19,
				(const uint16_t*) ladder);
	}
}






void player_Init(player_t *player) {
	player->lvl = 0;
	player->x = 0;
	player->y = ground_pos[0];
	player->direction = 1;
	player->motion = 0;
	player->climb = 0;
	player->hit = 0;
	player->shield = 0;
}




uint8_t can_climb(player_t *player, input_t *input){
	if(ladder_pos[player->lvl]-2 <= input->position && ladder_pos[player->lvl]+12 >= input->position){ // player's leftmost side is between both ladder poles
		return 1;
	}else if(input->position+14 >= ladder_pos[player->lvl]-2 && input->position+14 <= ladder_pos[player->lvl]+12 ){// player's rightmost side is between both ladder poles
		return 1;
	}
	return 0;
}

uint8_t is_touching(uint8_t sprite_pos, uint8_t sprite_len, uint8_t item_pos, uint8_t item_len){
	if(item_pos <= sprite_pos && sprite_pos <= item_pos+item_len){
		return 1;
	}else if(item_pos <= sprite_pos+sprite_len && sprite_pos+sprite_len <= item_pos+item_len){
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

	if(shield_pressed()){ //if shield was just pressed
		load_audio(play_shield_out);
		player->shield = 1;
	}
	if(player->shield && !get_shield()){ // if shield was just unequipped
		load_audio(play_shield_in);
		player->shield = 0;
	}


}



void update_DK(DK_t *DK, bullet_t *bullet, player_t *player){

	ST7735_DrawImage(DK->x, DK->y, DKong_size[0], DKong_size[1], (const uint16_t*) black);
	switch(DK->state){
		case getting_pos:
			if(bullet[DK->bullet_index].state == selected){
				DK->bullet_pos = bullet[DK->bullet_index].x;
				DK->state++;
			}else{
				DK->bullet_index= (DK->bullet_index+1)%5;
			}
			break;
		case found_pos:
			if(DK->x < DK->bullet_pos){
				DK->x++;
			}else if(DK->x > DK->bullet_pos){
				DK->x--;
			}else{
				DK->state++;
			}
			break;
		case at_pos:
			bullet[DK->bullet_index].state = moving;
			DK->bullet_index = (DK->bullet_index+1)%5;
			DK->state = getting_pos;

			break;
	}

	if(player->lvl == LAST_LVL(ground_pos) && is_touching(player->x, 16,DK->x, DKong_size[0])){
		state = WIN;
		load_audio(play_win);
	}


	ST7735_DrawImage(DK->x , DK->y, DKong_size[0], DKong_size[1], (const uint16_t*) DKong);
}


void update_player(player_t *player, input_t *input, bullet_t *bullet) {
	erase_player(player, input);
	static uint8_t animate = 0;

//	//lose animation
	if(player->hit && !get_shield()){
		load_audio(play_lose);
		state = LOSE;
		return;
	}else if( player->hit ){
		load_audio(play_punch);
		player->hit = 0;

	}

	//climbing animations
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

	//walking animations
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



void erase_bullet(bullet_t *bullet){
	for(int i = 0; i < 5; i++){
		ST7735_DrawImage(bullet[i].x , bullet[i].y, 4, 5, (const uint16_t*) black);

		if(bullet[i].y+bulletD_size[1] > ground_pos[bullet[i].lvl] && bullet[i].lvl == 0){
					ST7735_DrawImage(0, ground_pos[bullet[i].lvl], 128, 1,
													(const uint16_t*) ground);
		}else if(bullet[i].y+bulletD_size[1] > ground_pos[bullet[i].lvl]){
			ST7735_DrawImage(0, ground_pos[bullet[i].lvl], 128, 5,
											(const uint16_t*) ground);
		}

		if(ladder_pos[bullet[i].lvl] <= bullet[i].x && ladder_pos[ladder_pos[bullet[i].lvl]]+12 >=  bullet[i].x){ // player's leftmost side is between both ladder poles
			ST7735_DrawImage(ladder_pos[bullet[i].lvl], ground_pos[bullet[i].lvl]-19, 10, 19,
							(const uint16_t*) ladder);
		}else if(bullet[i].x+bulletD_size[0]  >= ladder_pos[bullet[i].lvl] && bullet[i].x+bulletD_size[0]<= ladder_pos[bullet[i].lvl]+12 ){// player's rightmost side is between both ladder poles
				ST7735_DrawImage(ladder_pos[bullet[i].lvl], ground_pos[bullet[i].lvl]-19, 10, 19,
											(const uint16_t*) ladder);
		}
	}
}



void update_bullet(bullet_t *bullet, player_t *player){
	static uint8_t animate = 0;

	for(int i = 0; i < 5; i++){

		switch(bullet[i].state){
			case unselected:
				bullet[i].x = generate_number();
				bullet[i].state++;
				break;
			case selected:
				bullet[i].lvl = LAST_LVL(ground_pos);
				bullet[i].y = ground_pos[ LAST_LVL(ground_pos) ];
				break;
			case moving:
				if(bullet[i].y > 160){
					bullet[i].y = ground_pos[ LAST_LVL(ground_pos) ];
					bullet[i].state = unselected;
				}else{

					if(animate == 200){

						erase_bullet(bullet);
						bullet[i].y++;

						ST7735_DrawImage(bullet[i].x, bullet[i].y, bulletD_size[0], bulletD_size[1],
											    		   			(const uint16_t*) bulletD);
						animate = 0;
					}

					if(bullet[i].y > ground_pos[bullet[i].lvl]+5){ // if bullet has passed a ground lvl
						bullet[i].lvl--;
					}

					animate++;
				}
				break;
		}

		if(is_touching(bullet[i].x, bulletD_size[0],player->x, 16) && is_touching( bullet[i].y, bulletD_size[1], player->y, 16 )){
					player->hit = 1;
		}


	}
}


void update_audio(){
	for(int i = 0; i < count; i++){
		play_audio(  files_audio[ audios[i] ], fileslen_audio[ audios[i] ] );
	}
	count = 0;

}

void load_audio(sound_t audio){
	audios[count++] = audio;
}






void gameplay() {
	static player_t player;
	static input_t input;
	static bullet_t bullet[5];
	static DK_t donkey_kong;

	if (state == STARTUP) {
		create_map();
		start_slider();
		ST7735_Init();
		player_Init(&player);
		load_audio(play_ready);
		state = GAMEPLAY;
	} else if(state == GAMEPLAY){
		update_input(&player, &input);
		update_player(&player,&input, bullet);
		update_bullet(bullet, &player);
		update_DK(&donkey_kong, bullet, &player);
		update_audio();
	}else if(state == WIN){
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_WriteString(20, 80, "YOU WIN", Font_11x18, ST7735_GREEN, ST7735_BLACK);
		HAL_Delay(700);
		state = !climb_pressed() ? state: STARTUP;

	}else{
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_WriteString(20, 80, "YOU LOSE", Font_11x18, ST7735_RED, ST7735_BLACK);
		HAL_Delay(700);
		state = !climb_pressed() ? state: STARTUP;
	}
}

