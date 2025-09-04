/*
 * game.h
 *
 *  Created on: Aug 27, 2025
 *      Author: Chandise Anderson
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_


void create_map();
void gameplay();
void player_Init(struct player_t *player);
void update_player(struct player_t *player);
void draw_player(struct player_t *player);



#endif /* INC_GAME_H_ */
