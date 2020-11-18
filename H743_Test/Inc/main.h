/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_mpu.h"
#include "fonts.h"
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
#define LTDC_B6_Pin GPIO_PIN_5
#define LTDC_B6_GPIO_Port GPIOK
#define LTDC_B2_Pin GPIO_PIN_10
#define LTDC_B2_GPIO_Port GPIOG
#define LTDC_R2_Pin GPIO_PIN_10
#define LTDC_R2_GPIO_Port GPIOC
#define LTDC_B5_Pin GPIO_PIN_4
#define LTDC_B5_GPIO_Port GPIOK
#define LTDC_B3_Pin GPIO_PIN_11
#define LTDC_B3_GPIO_Port GPIOG
#define LTDC_G7_Pin GPIO_PIN_3
#define LTDC_G7_GPIO_Port GPIOD
#define LTDC_B7_Pin GPIO_PIN_6
#define LTDC_B7_GPIO_Port GPIOK
#define LTDC_B4_Pin GPIO_PIN_3
#define LTDC_B4_GPIO_Port GPIOK
#define LTDC_B1_Pin GPIO_PIN_12
#define LTDC_B1_GPIO_Port GPIOG
#define LTDC_G0_Pin GPIO_PIN_5
#define LTDC_G0_GPIO_Port GPIOE
#define LTDC_B0_Pin GPIO_PIN_4
#define LTDC_B0_GPIO_Port GPIOE
#define LTDC_DE_Pin GPIO_PIN_7
#define LTDC_DE_GPIO_Port GPIOK
#define LTDC_R5_Pin GPIO_PIN_9
#define LTDC_R5_GPIO_Port GPIOA
#define LTDC_G1_Pin GPIO_PIN_6
#define LTDC_G1_GPIO_Port GPIOE
#define LTDC_R6_Pin GPIO_PIN_8
#define LTDC_R6_GPIO_Port GPIOA
#define LTDC_R4_Pin GPIO_PIN_11
#define LTDC_R4_GPIO_Port GPIOA
#define LTDC_G6_Pin GPIO_PIN_11
#define LTDC_G6_GPIO_Port GPIOI
#define LTDC_HSYNC_Pin GPIO_PIN_6
#define LTDC_HSYNC_GPIO_Port GPIOC
#define LTDC_CLK_Pin GPIO_PIN_7
#define LTDC_CLK_GPIO_Port GPIOG
#define LTDC_VSYNC_Pin GPIO_PIN_13
#define LTDC_VSYNC_GPIO_Port GPIOI
#define LTDC_G5_Pin GPIO_PIN_0
#define LTDC_G5_GPIO_Port GPIOK
#define LTDC_G4_Pin GPIO_PIN_11
#define LTDC_G4_GPIO_Port GPIOJ
#define LTDC_G3_Pin GPIO_PIN_10
#define LTDC_G3_GPIO_Port GPIOJ
#define LTDC_G2_Pin GPIO_PIN_9
#define LTDC_G2_GPIO_Port GPIOJ
#define LTDC_R1_Pin GPIO_PIN_0
#define LTDC_R1_GPIO_Port GPIOJ
#define LTDC_R7_Pin GPIO_PIN_6
#define LTDC_R7_GPIO_Port GPIOJ
#define LTDC_R0_Pin GPIO_PIN_15
#define LTDC_R0_GPIO_Port GPIOI
#define LTDC_BL_Pin GPIO_PIN_13
#define LTDC_BL_GPIO_Port GPIOD
#define LTDC_R3_Pin GPIO_PIN_2
#define LTDC_R3_GPIO_Port GPIOJ
#define LED_R_Pin GPIO_PIN_0
#define LED_R_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LCD_DEFAULT_FONT Font24

/* ???? */
#define LCD_COLOR_BLUE ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT ((uint32_t)0xFF000000)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
