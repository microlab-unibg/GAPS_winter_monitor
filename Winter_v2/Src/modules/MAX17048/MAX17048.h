/**
 ******************************************************************************
 * @file           : MAX17048.h
 * @brief          : Header file of MAX17048 driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 14 Feb 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODULES_MAX17048_MAX17048_H_
#define MODULES_MAX17048_MAX17048_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported typedef ----------------------------------------------------------*/
typedef enum {
	MAX17048_DISABLE = 0,
	MAX17048_ENABLE = !MAX17048_DISABLE
} MAX17048_FunctionalState;

/* Private defines -----------------------------------------------------------*/
#define MAX17048_REG_VCELL		0x02 //READ - VCELL measurement between VDD and GND,#define MAX17048_REG_SOC		0x04 //READ - SOC measurement#define MAX17048_REG_MODE		0x06 //WRITE - enable quick start, sleep and hibernate status#define MAX17048_REG_VERSION	0x08 //READ - production version of the IC#define MAX17048_REG_HIBRT		0x0A //RW - config hibernate and activation status#define MAX17048_REG_CONFIG		0x0C#define MAX17048_REG_VALRT		0x14#define MAX17048_REG_CRATE		0x16#define MAX17048_REG_VRES_ID	0x18#define MAX17048_REG_STAT		0x1A#define MAX17048_REG_CMD		0xFE

/* Exported functions prototypes ---------------------------------------------*/
uint8_t MAX17048_ReadReg(uint8_t RegName, uint8_t* ReadByte, uint16_t Size);
uint8_t MAX17048_WriteReg(uint8_t Register, uint16_t Value);
uint8_t MAX17048_Read_Voltage(uint16_t* voltage);
uint8_t MAX17048_Read_SOC(uint8_t* soc);

#endif /* MODULES_MAX17048_MAX17048_H_ */
