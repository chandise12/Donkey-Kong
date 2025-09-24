/*
 * game.h
 *
 *  Created on: Aug 27, 2025
 *      Author: Chandise Anderson
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include <stdint.h>

#define FLOOR_LVL(y_ground) y_ground - 17
#define LAST_LVL(arr) (sizeof(arr))/(sizeof(arr[0])) - 1
#define STARTUP	 0
#define GAMEPLAY 1
#define LOSE	 2
#define WIN		 3





//structs/enums



typedef enum {
	left, right, idle_left, idle_right
}slider_t;

typedef enum {
	not_pressed, is_pressed
} button_t;

typedef enum {
	unselected, selected, moving
}bullet_state_t;

typedef struct {
	uint8_t position;
	slider_t slider;
	button_t button_climb;
	button_t button_shield;
}input_t;

typedef enum{
	getting_pos, found_pos, at_pos
}DK_state_t;

typedef enum{
	play_none, play_ready, play_punch, play_win, play_lose, play_shield_out, play_shield_in
}sound_t;

typedef struct {
	uint8_t lvl;
	uint8_t x;
	uint8_t y;
	uint8_t direction;
	uint8_t motion;
	uint8_t climb;
	uint8_t hit;
	uint8_t shield;
}player_t;

typedef struct {
	uint8_t x;
	uint8_t y;
	DK_state_t state;
	uint8_t bullet_index;
	uint8_t bullet_pos;
}DK_t;

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t lvl;
	bullet_state_t state;
}bullet_t;

typedef struct{
	uint8_t count;
	sound_t audios[];
}audio_t;


void create_map();
void gameplay();

void player_Init(player_t *player);
void update_player(player_t *player, input_t *input, bullet_t *bullet);
uint8_t can_climb(player_t *player, input_t *input);
uint8_t is_touching(uint8_t sprite_pos, uint8_t sprite_len, uint8_t item_pos, uint8_t item_len);
void erase_player(player_t *player, input_t *input );
void update_input(player_t *player, input_t *input);

void update_DK(DK_t *DK, bullet_t *bullet, player_t *player);

void update_bullet(bullet_t *bullet, player_t *player);
void erase_bullet(bullet_t *bullet);

void update_audio();
void load_audio(sound_t audio);


void load_audio(sound_t audio);


#endif /* INC_GAME_H_ */
