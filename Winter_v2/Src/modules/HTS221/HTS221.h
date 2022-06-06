/**
 ******************************************************************************
 * @file           : HTS221.h
 * @brief          : Header file of HTS221 driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODULES_HTS221_HTS221_H_
#define MODULES_HTS221_HTS221_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Private defines -----------------------------------------------------------*/
#define HTS221_ADDRESS 				0xBE

#define HTS221_REG_WHO_AM_I			0x0F
#define HTS221_WHO_AM_I_CONTENT		0xBC

#define HTS221_REG_AV_CONF	0x10

#define HTS221_REG_CTRL1	0x20
#define HTS221_REG_CTRL2	0x21
#define HTS221_REG_CTRL3	0x22
#define HTS221_REG_STATUS	0x27

#define HTS221_REG_HUM_OUT_L	0x28
#define HTS221_REG_HUM_OUT_H	0x29

#define HTS221_REG_TEMP_OUT_L	0x2A
#define HTS221_REG_TEMP_OUT_H	0x2B

#define HTS221_REG_H0_rH_x2	0x30
#define HTS221_REG_H1_rH_x2	0x31

#define HTS221_REG_T0_degC_x8	0x32
#define HTS221_REG_T1_degC_x8	0x33

#define HTS221_REG_T1_T0_msb	0x35

#define HTS221_REG_H0_T0_OUT_L	0x36
#define HTS221_REG_H0_T0_OUT_H	0x37

#define HTS221_REG_H1_T0_OUT_L	0x3A
#define HTS221_REG_H1_T0_OUT_H	0x3B

#define HTS221_REG_T0_OUT_L	0x3C
#define HTS221_REG_T0_OUT_H	0x3D

#define HTS221_REG_T1_OUT_L	0x3E
#define HTS221_REG_T1_OUT_H	0x3F

/* Exported functions prototypes ---------------------------------------------*/
uint8_t HTS221_Read_Reg(uint8_t Register, uint8_t* ReadByte, uint16_t Size);
uint8_t HTS221_Write_Reg(uint8_t RegName, uint8_t RegValue);
uint8_t HTS221_Config(void);
uint8_t HTS221_Read_Temperature(float* temperature);
uint8_t HTS221_Read_Humidity(float* humidity);
uint8_t HTS221_PowerDown(uint8_t arg);
uint8_t HTS221_Check_WhoAmI(void);

#endif /* MODULES_HTS221_HTS221_H_ */
