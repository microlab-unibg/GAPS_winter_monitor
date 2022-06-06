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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
	I2C_ENV = 0x01,
	I2C_GAS = 0x02,
	I2C_EXP = 0x03
} I2C_e;

typedef enum {
	SPI_BLE = 0x01,
	SPI_MOTION = 0x02,
	SPI_EXP = 0x03,
} SPI_e;
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
uint8_t I2C_Write(I2C_e i2c, uint8_t DevAddress, uint8_t* pData, uint8_t Size);
uint8_t I2C_Read(I2C_e i2c, uint8_t DevAddress, uint8_t RegAddress,
		uint8_t* pData, uint16_t Size);
uint8_t SPI_Write(SPI_e spi, uint8_t* pData, uint16_t Size);
uint8_t SPI_Read(SPI_e spi, uint8_t RegAddress, uint8_t* pData,
		uint16_t Size);
void Enable_LPR_Mode(void);
void Disable_LPR_Mode(void);
void Enter_Stop2_Mode(void);
void Exit_Stop2_Mode(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOT_CS_Pin GPIO_PIN_13
#define MOT_CS_GPIO_Port GPIOC
#define EXP_SCL_Pin GPIO_PIN_0
#define EXP_SCL_GPIO_Port GPIOC
#define EXP_SDA_Pin GPIO_PIN_1
#define EXP_SDA_GPIO_Port GPIOC
#define MISC_MISO_Pin GPIO_PIN_2
#define MISC_MISO_GPIO_Port GPIOC
#define MISC_MOSI_Pin GPIO_PIN_3
#define MISC_MOSI_GPIO_Port GPIOC
#define INT_ACC_Pin GPIO_PIN_0
#define INT_ACC_GPIO_Port GPIOA
#define INT_ACC_EXTI_IRQn EXTI0_IRQn
#define SD_CD_Pin GPIO_PIN_1
#define SD_CD_GPIO_Port GPIOA
#define BLE_IRQ_Pin GPIO_PIN_2
#define BLE_IRQ_GPIO_Port GPIOA
#define BLE_IRQ_EXTI_IRQn EXTI2_IRQn
#define EXP_CS_Pin GPIO_PIN_3
#define EXP_CS_GPIO_Port GPIOA
#define EXP_GPIO_Pin GPIO_PIN_4
#define EXP_GPIO_GPIO_Port GPIOA
#define BLE_SCK_Pin GPIO_PIN_5
#define BLE_SCK_GPIO_Port GPIOA
#define BLE_MISO_Pin GPIO_PIN_6
#define BLE_MISO_GPIO_Port GPIOA
#define BLE_MOSI_Pin GPIO_PIN_7
#define BLE_MOSI_GPIO_Port GPIOA
#define LOAD_EN_Pin GPIO_PIN_4
#define LOAD_EN_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_5
#define LED_GREEN_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_0
#define LED_RED_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_1
#define LED_BLUE_GPIO_Port GPIOB
#define BLE_CS_Pin GPIO_PIN_2
#define BLE_CS_GPIO_Port GPIOB
#define MISC_SCK_Pin GPIO_PIN_10
#define MISC_SCK_GPIO_Port GPIOB
#define BLE_RST_Pin GPIO_PIN_11
#define BLE_RST_GPIO_Port GPIOB
#define BATT_STAT_Pin GPIO_PIN_12
#define BATT_STAT_GPIO_Port GPIOB
#define GAS_SCL_Pin GPIO_PIN_13
#define GAS_SCL_GPIO_Port GPIOB
#define GAS_SDA_Pin GPIO_PIN_14
#define GAS_SDA_GPIO_Port GPIOB
#define DBG_GPIO1_Pin GPIO_PIN_15
#define DBG_GPIO1_GPIO_Port GPIOA
#define DBG_GPIO3_Pin GPIO_PIN_3
#define DBG_GPIO3_GPIO_Port GPIOB
#define DBG_GPIO2_Pin GPIO_PIN_4
#define DBG_GPIO2_GPIO_Port GPIOB
#define ENV_SCL_Pin GPIO_PIN_6
#define ENV_SCL_GPIO_Port GPIOB
#define ENV_SDA_Pin GPIO_PIN_7
#define ENV_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
