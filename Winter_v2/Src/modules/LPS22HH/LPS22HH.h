/**
 ******************************************************************************
 * @file           : LPS22HH.h
 * @brief          : Header file of LPS22HH driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 17 Feb 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODULES_LPS22HH_LPS22HH_H_
#define MODULES_LPS22HH_LPS22HH_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#define LPS22HH_ADDRESS				0xB8

/* --- WHO_AM_I --------------------------------------------------------------*/
#define LPS22HH_REG_WHO_AM_I		0x0F
#define LPS22HH_WHO_AM_I_CONTENT	0xB3

/* ---- CTRL_REG1 ------------------------------------------------------------*/
#define LPS22HH_REG_CTRL_REG1		0x10

#define LPS22HH_ODR_SHIFT			4
typedef enum
{
	LPS22HH_ODR_POWER_DOWN_ONE_SHOT = 0x00,
	LPS22HH_ODR_1Hz = 0x01 << LPS22HH_ODR_SHIFT,
	LPS22HH_ODR_10Hz = 0x02 << LPS22HH_ODR_SHIFT,
	LPS22HH_ODR_25Hz = 0x03 << LPS22HH_ODR_SHIFT,
	LPS22HH_ODR_50Hz = 0x04 << LPS22HH_ODR_SHIFT,
	LPS22HH_ODR_75Hz = 0x05 << LPS22HH_ODR_SHIFT,
	LPS22HH_ODR_100Hz = 0x06 << LPS22HH_ODR_SHIFT,
	LPS22HH_ODR_200Hz = 0x07 << LPS22HH_ODR_SHIFT
} LPS22HH_ODR;

#define LPS22HH_BDU_SHIFT			1
typedef enum
{
	LPS22HH_BDU_CONTINUOUS = 0x00,
	LPS22HH_BDU_BLOCKING = 0x01 << LPS22HH_BDU_SHIFT,
} LPS22HH_BDU;

/* ---- CTRL_REG2 ------------------------------------------------------------*/
#define LPS22HH_REG_CTRL_REG2		0x11

#define LPS22HH_IF_ADD_INC_SHIFT	4
typedef enum
{
	LPS22HH_IF_ADD_INC_DISABLED = 0x00,
	LPS22HH_IF_ADD_INC_ENABLED = 0x01 << LPS22HH_IF_ADD_INC_SHIFT,
} LPS22HH_IF_ADD_INC;

#define LPS22HH_LOW_NOISE_SHIFT		1
typedef enum
{
	LPS22HH_LOW_NOISE_DISABLED = 0x00,
	LPS22HH_LOW_NOISE_ENABLED = 0x01 << LPS22HH_LOW_NOISE_SHIFT,
} LPS22HH_LOW_NOISE;

/* --- Other registers -------------------------------------------------------*/
#define LPS22HH_REG_PRESS_OUT_XL	0x28
#define LPS22HH_REG_PRESS_OUT_L		0x29
#define LPS22HH_REG_PRESS_OUT_H		0x2A
#define LPS22HH_REG_TEMP_OUT_L		0x2B
#define LPS22HH_REG_TEMP_OUT_H		0x2C

/* --- Base sensitivities ----------------------------------------------------*/
#define LPS22HH_PRESS_SENSITIVITY	4096
#define LPS22HH_TEMP_SENSITIVITY	100

/* --- Structure definition --------------------------------------------------*/
typedef struct {
	LPS22HH_ODR odr;
	LPS22HH_BDU bdu;
	LPS22HH_IF_ADD_INC auto_inc_en;
	LPS22HH_LOW_NOISE low_noise_en;
} LPS22HH_Init_t;

/* Exported functions prototypes ---------------------------------------------*/
uint8_t LPS22HH_Read_Reg(uint8_t Register, uint8_t* pReadByte, uint16_t Size);
uint8_t LPS22HH_Write_Reg(uint8_t Register, uint8_t Value);
uint8_t LPS22HH_Config(LPS22HH_Init_t* pNewConfig);
uint8_t LPS22HH_Read_Pressure(float* pPressure);
uint8_t LPS22HH_Set_ODR(LPS22HH_ODR NewODR);
uint8_t LPS22HH_PowerDown(uint8_t arg);
uint8_t LPS22HH_Check_WhoAmI(void);

#endif /* MODULES_LPS22HH_LPS22HH_H_ */
