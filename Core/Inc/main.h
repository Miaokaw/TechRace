/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "I2c.h"
#include "../../User/MPU6050.h"
#include "../../User/OLED_I2C.h"
  /* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */

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
#define BEEP_Pin GPIO_PIN_2
#define BEEP_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOE
#define INT_6050_Pin GPIO_PIN_4
#define INT_6050_GPIO_Port GPIOE
#define NRF_IRQ_Pin GPIO_PIN_5
#define NRF_IRQ_GPIO_Port GPIOE
#define NRF_CE_Pin GPIO_PIN_6
#define NRF_CE_GPIO_Port GPIOE
#define JDY_STAT_Pin GPIO_PIN_0
#define JDY_STAT_GPIO_Port GPIOF
#define JDY_PWRC_Pin GPIO_PIN_1
#define JDY_PWRC_GPIO_Port GPIOF
#define LORA_NSS_Pin GPIO_PIN_2
#define LORA_NSS_GPIO_Port GPIOF
#define LORA_BUSY_Pin GPIO_PIN_3
#define LORA_BUSY_GPIO_Port GPIOF
#define LORA_RXEN_Pin GPIO_PIN_4
#define LORA_RXEN_GPIO_Port GPIOF
#define LORA_DIO1_Pin GPIO_PIN_5
#define LORA_DIO1_GPIO_Port GPIOF
#define GRAY_L2_Pin GPIO_PIN_6
#define GRAY_L2_GPIO_Port GPIOF
#define GRAY_L1_Pin GPIO_PIN_7
#define GRAY_L1_GPIO_Port GPIOF
#define GRAY_M_Pin GPIO_PIN_8
#define GRAY_M_GPIO_Port GPIOF
#define GRAY_R1_Pin GPIO_PIN_9
#define GRAY_R1_GPIO_Port GPIOF
#define GRAY_R2_Pin GPIO_PIN_10
#define GRAY_R2_GPIO_Port GPIOF
#define LORA_NRST_Pin GPIO_PIN_8
#define LORA_NRST_GPIO_Port GPIOG
#define M2_BIN2_Pin GPIO_PIN_3
#define M2_BIN2_GPIO_Port GPIOD
#define M2_BIN1_Pin GPIO_PIN_4
#define M2_BIN1_GPIO_Port GPIOD
#define M2_AIN2_Pin GPIO_PIN_9
#define M2_AIN2_GPIO_Port GPIOG
#define M2_AIN1_Pin GPIO_PIN_10
#define M2_AIN1_GPIO_Port GPIOG
#define M1_BIN2_Pin GPIO_PIN_11
#define M1_BIN2_GPIO_Port GPIOG
#define M1_BIN1_Pin GPIO_PIN_12
#define M1_BIN1_GPIO_Port GPIOG
#define M1_AIN2_Pin GPIO_PIN_13
#define M1_AIN2_GPIO_Port GPIOG
#define M1_AIN1_Pin GPIO_PIN_14
#define M1_AIN1_GPIO_Port GPIOG

  /* USER CODE BEGIN Private defines */

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
