/*
 * audio.c
 *
 *  Created on: Aug 14, 2025
 *      Author: Chandise Anderson
 */

#include <stdint.h>
#include "main.h"
#include "audio.h"

TIM_HandleTypeDef htim1;
const uint8_t *current_audio = 0;
uint32_t current_length = 0;
uint32_t index_audio = 0;



void DAC_OUT(uint32_t value){
	HAL_GPIO_WritePin(GPIOC,AUDIO_B0_OUT_Pin,value&0x01);
	HAL_GPIO_WritePin(GPIOC,AUDIO_B1_OUT_Pin,value&0x02);
	HAL_GPIO_WritePin(GPIOB,AUDIO_B2_OUT_Pin,value&0x03);
	HAL_GPIO_WritePin(GPIOB,AUDIO_B3_OUT_Pin,value&0x04);
}

void play_audio(const uint8_t *audio, uint32_t length){
	if(!index_audio){//start the timer
		HAL_TIM_Base_Start_IT(&htim1);
		current_audio = audio;
		current_length = length;

	}else if(length == index_audio){ //end of audio
		HAL_TIM_Base_Stop_IT(&htim1);
		index_audio = 1;
		current_audio = 0;
		current_length = 0;
		return;
	}
		index_audio++;
		DAC_OUT(audio[index_audio]);

}
