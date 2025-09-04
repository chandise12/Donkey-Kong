/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim5;
extern SPI_HandleTypeDef hspi2;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define BUTTON0_IN_Pin GPIO_PIN_2
#define BUTTON0_IN_GPIO_Port GPIOC
#define BUTTON0_IN_EXTI_IRQn EXTI2_IRQn
#define BUTTON1_IN_Pin GPIO_PIN_3
#define BUTTON1_IN_GPIO_Port GPIOC
#define BUTTON1_IN_EXTI_IRQn EXTI3_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SLIDER_IN_Pin GPIO_PIN_4
#define SLIDER_IN_GPIO_Port GPIOC
#define AUDIO_B1_OUT_Pin GPIO_PIN_5
#define AUDIO_B1_OUT_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SCK_Pin GPIO_PIN_13
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_AO_Pin GPIO_PIN_14
#define LCD_AO_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_15
#define LCD_SDA_GPIO_Port GPIOB
#define AUDIO_B0_OUT_Pin GPIO_PIN_6
#define AUDIO_B0_OUT_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_7
#define LCD_RESET_GPIO_Port GPIOB
#define AUDIO_B2_OUT_Pin GPIO_PIN_8
#define AUDIO_B2_OUT_GPIO_Port GPIOB
#define AUDIO_B3_OUT_Pin GPIO_PIN_9
#define AUDIO_B3_OUT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
